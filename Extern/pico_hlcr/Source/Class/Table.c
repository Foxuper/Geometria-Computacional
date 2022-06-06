// Include | Header
#include <pico_hlcr/Class/Table.h>

// Structure | Private | Table Node
typedef struct Table_Node
{
	var key;
	var value;
	uint64 hash;
	bool is_null;
}
Table_Node;

// Constant | Private | Array of Primes
static const uint64 primes[] =
{
	0,       1,       2,       3,       5,
	7,       11,      17,      23,      29,
	37,      47,      59,      79,      101,
	127,     163,     211,     269,     337,
	421,     541,     677,     853,     1069,
	1361,    1709,    2137,    2671,    3343,
	4201,    5261,    6577,    8221,    10289,
	12889,   16111,   20143,   25183,   31481,
	39359,   49199,   61507,   76907,   96137,
	120181,  150239,  187823,  234781,  293483,
	366859,  458593,  573247,  716563,  895709,
	1119649, 1399577, 1749491, 2186869, 2733589,
	3416993, 4271251, 5339077, 6673847, 8342329
};

// Constant | Private | Number of Primes
static const uint64 num_primes = sizeof(primes) / sizeof(uint64);

// Functions | Private | Table
static var        memory_at   (Table this, uint64 index);
static var        key_at      (Table this, uint64 index);
static var        value_at    (Table this, uint64 index);
static uint64    *hash_at     (Table this, uint64 index);
static bool      *is_null_at  (Table this, uint64 index);
static void       memory_init (Table this, uint64 index);
static Table_Node node_at     (Table this, uint64 index);
static void       assign_at   (Table this, uint64 index, Table_Node node);
static bool       can_swap    (uint64 index_a, uint64 index_b, uint64 desired_index);
static void       insert_node (Table this, Table_Node node);
static void       erase_node  (Table this, uint64 index);
static void       ref_destroy (Table this);
static bool       rehash      (Table this, uint64 size);
static uint64     ideal_size  (uint64 size, double load_factor);
static bool       adjust_size (Table this);
static bool       search_key  (Table this, var key, uint64 *index);

// Method | Public | New -> build_with
void Table_build_with (var this_ptr, Tuple args)
{
	THIS(Table);
	Type key_type = get(args, I(0));
	Type value_type = get(args, I(1));
	key_type = (key_type is VAR) ? TYPE(Ptr) : key_type;
	value_type = (value_type is VAR) ? TYPE(Ptr) : value_type;

	this->table = NULL;
	this->key_type = NULL;
	this->value_type = NULL;
	this->key_size = 0;
	this->value_size = 0;
	this->len = 0;
	this->slots = 0;
	this->slot_size = 0;
	this->load_factor = LOAD_FACTOR;

	if (not is_type(key_type))
		key_type = TYPE(Ptr);

	if (not is_type(value_type))
		value_type = key_type;

	uint64 key_size = size(key_type);
	uint64 value_size = size(value_type);
	uint64 key_offset = HEADER_SIZE + key_size;
	uint64 value_offset = HEADER_SIZE + value_size;

	this->key_type = key_type;
	this->value_type = value_type;
	this->key_size = key_size;
	this->value_size = value_size;
	this->slot_size = key_offset + value_offset + sizeof(uint64) + sizeof(bool);
}

// Method | Public | New -> destroy
void Table_destroy (var this_ptr)
{
	THIS(Table);

	for (uint64 index = 0; index < this->slots; index++)
	{
		if (not *is_null_at(this, index))
		{
			destroy(key_at(this, index));
			destroy(value_at(this, index));
		}
	}

	free(this->table);
}

// Method | Public | Assign -> assign
void Table_assign (var this_ptr, var other)
{
	THIS(Table);
	Type key_t = key_type(other);
	Type value_t = value_type(other);

	rehash(this, 0);
	this->key_type = (key_t is VAR) ? TYPE(Ptr) : key_t;
	this->value_type = (value_t is VAR) ? TYPE(Ptr) : value_t;
	this->key_size = size(this->key_type);
	this->value_size = size(this->value_type);

	if (iter_type(other) isnt Table_iter_type(this))
	{
		foreach (var, value in other)
		{
			with (var, key as index_of(other, value))
				Table_insert_at(this, key, value);
		}
	}
	else
	{
		foreach (var, key in other)
			Table_insert_at(this, key, get(other, key));
	}
}

// Method | Public | Compare -> cmp
int64 Table_cmp (var this, var other)
{
	var key_a = Table_init(this);
	var key_b = init(other);

	while (true)
	{
		if (key_a is ITER_STOP and key_b is ITER_STOP) return 0;
		if (key_a is ITER_STOP) return -1;
		if (key_b is ITER_STOP) return 1;

		int64 difference = cmp(key_a, key_b);

		if (difference is 0)
		{
			var value_a = Table_get(this, key_a);
			var value_b = get(other, key_b);
			difference = cmp(value_a, value_b);

			if (difference is 0)
			{
				key_a = Table_next(this, key_a);
				key_b = next(other, key_b);
				continue;
			}
		}

		return difference;
	}
}

// Method | Public | Hash -> hash
uint64 Table_hash (var this_ptr)
{
	THIS(Table);
	uint64 h = 0;

	for (uint64 index = 0; index < this->slots; index++)
	{
		if (not *is_null_at(this, index))
			h = h ^ *hash_at(this, index) ^ hash(value_at(this, index));
	}

	return h;
}

// Method | Public | Len -> len
uint64 Table_len (var this_ptr)
{
	THIS(Table);
	return this->len;
}

// Method | Public | Iter -> init
var Table_init (var this_ptr)
{
	THIS(Table);
	return (this->len > 0) ? Table_first(this) : ITER_STOP;
}

// Method | Public | Iter -> next
var Table_next (var this_ptr, var current)
{
	THIS(Table);

	uint64 offset = this->key_size + HEADER_SIZE + this->value_size + sizeof(uint64);
	current = memory_offset(current, this->slot_size);
	var last = key_at(this, this->slots - 1);

	while (true)
	{
		if (current > last) return ITER_STOP;
		bool *is_null = memory_offset(current, offset);

		if (not *is_null)
			return current;

		current = memory_offset(current, this->slot_size);
	}
}

// Method | Public | Iter -> iter_type
Type Table_iter_type (var this_ptr)
{
	THIS(Table);
	return this->key_type;
}

// Method | Public | Cast -> cast_to
String Table_to_string (var this)
{
	String casted = new(String);
	var last = Table_last(this);
	concat(casted, S("{"));

	foreach (var, key in this)
	{
		var value = get(this, key);
		concat(casted, S("("));

		with (String, string as str(key))
			concat(casted, string);

		concat(casted, S(" -> "));

		with (String, string as str(value))
			concat(casted, string);

		concat(casted, S(")"));

		if (key isnt last)
			concat(casted, S(", "));
	}

	concat(casted, S("}"));
	return casted;
}

// Method | Public | Concat -> concat
void Table_concat (var this_ptr, var other)
{
	THIS(Table);
	Type key_t = key_type(other);
	Type value_t = value_type(other);
	key_t = (key_t is VAR) ? TYPE(Ptr) : key_t;
	value_t = (value_t is VAR) ? TYPE(Ptr) : value_t;

	if (key_t is this->key_type and value_t is this->value_type)
	{
		if (iter_type(other) isnt Table_iter_type(this))
		{
			foreach (var, value in other)
			{
				with (var, key as index_of(other, value))
					Table_insert_at(this, key, value);
			}
		}
		else
		{
			foreach (var, key in other)
				Table_insert_at(this, key, get(other, key));
		}
	}
}

// Method | Public | Access -> first
var Table_first (var this_ptr)
{
	THIS(Table);

	for (uint64 index = 0; index < this->slots; index++)
	{
		if (not *is_null_at(this, index))
			return key_at(this, index);
	}

	return NULL;
}

// Method | Public | Access -> last
var Table_last (var this_ptr)
{
	THIS(Table);

	for (uint64 index = 1; index <= this->slots; index++)
	{
		if (not *is_null_at(this, this->slots - index))
			return key_at(this, this->slots - index);
	}

	return NULL;
}

// Method | Public | Access -> get
var Table_get (var this, var key)
{
	uint64 index;

	if (search_key(this, key, &index))
		return value_at(this, index);

	return NULL;
}

// Method | Public | Access -> set
void Table_set (var this, var key, var value)
{
	assign(Table_get(this, key), value);
}

// Method | Public | Content -> contains
bool Table_contains (var this_ptr, var key)
{
	THIS(Table);
	return Table_get(this, key);
}

// Method | Public | Content -> index_of
var Table_index_of (var this_ptr, var value)
{
	THIS(Table);

	for (uint64 index = 0; index < this->slots; index++)
	{
		if (not *is_null_at(this, index) and eq(value_at(this, index), value))
			return copy(key_at(this, index));
	}

	return NULL;
}

// Method | Public | Content -> key_type
Type Table_key_type (var this_ptr)
{
	THIS(Table);
	return this->key_type;
}

// Method | Public | Content -> value_type
Type Table_value_type (var this_ptr)
{
	THIS(Table);
	return this->value_type;
}

// Method | Public | Insert -> erase
void Table_erase (var this_ptr, var value)
{
	THIS(Table);

	for (uint64 index = 0; index < this->slots; index++)
	{
		if (not *is_null_at(this, index) and eq(value_at(this, index), value))
		{
			Table_erase_at(this, key_at(this, index));
			return;
		}
	}
}

// Method | Public | Insert -> insert_at
void Table_insert_at (var this_ptr, var key, var value)
{
	THIS(Table);
	this->len++;

	if (this->slots > 0 and search_key(this, key, NULL))
	{
		this->len--;
		return;
	}

	if (adjust_size(this))
	{
		Table_Node node = {key, value, hash(key), false};
		insert_node(this, node);
		return;
	}

	this->len--;
}

// Method | Public | Insert -> erase_at
void Table_erase_at (var this_ptr, var key)
{
	THIS(Table);
	uint64 index;

	if (this->len is 0 or not search_key(this, key, &index))
		return;

	Table_Node node = node_at(this, index);
	erase_node(this, index);
	this->len--;

	if (adjust_size(this))
		return;

	insert_node(this, node);
	this->len++;
}

// Function | Private | Memory at Index
static var memory_at (Table this, uint64 index)
{
	return (uint8 *) this->table + (index * this->slot_size);
}

// Function | Private | Key at Index
static var key_at (Table this, uint64 index)
{
	return memory_offset(memory_at(this, index), HEADER_SIZE);
}

// Function | Private | Value at Index
static var value_at (Table this, uint64 index)
{
	uint64 offset = HEADER_SIZE + this->key_size;
	return memory_offset(memory_at(this, index), offset + HEADER_SIZE);
}

// Function | Private | Hash at Index
static uint64 *hash_at (Table this, uint64 index)
{
	uint64 offset = 2 * HEADER_SIZE + this->key_size + this->value_size;
	return memory_offset(memory_at(this, index), offset);
}

// Function | Private | Is Null at Index
static bool *is_null_at (Table this, uint64 index)
{
	uint64 offset = 2 * HEADER_SIZE + this->key_size + this->value_size;
	return memory_offset(memory_at(this, index), offset + sizeof(uint64));
}

// Function | Private | Node At
static Table_Node node_at (Table this, uint64 index)
{
	var key = key_at(this, index);
	var value = value_at(this, index);
	uint64 hash = *hash_at(this, index);
	bool is_null = *is_null_at(this, index);
	return (Table_Node) {key, value, hash, is_null};
}

// Function | Private | Assign At
static void assign_at (Table this, uint64 index, Table_Node node)
{
	assign(key_at(this, index), node.key);
	assign(value_at(this, index), node.value);
	*hash_at(this, index) = node.hash;
	*is_null_at(this, index) = node.is_null;
}

// Function | Private | Initialize Memory at Index
static void memory_init (Table this, uint64 index)
{
	var memory = memory_at(this, index);
	var key = header_init(memory, this->key_type, ALLOC_DATA);
	memset(key, 0, this->key_size);
	build(key);

	memory = memory_offset(key, this->key_size);
	var value = header_init(memory, this->value_type, ALLOC_DATA);
	memset(value, 0, this->value_size);
	build(value);

	uint64 *hash = memory_offset(value, this->value_size);
	memset(hash, 0, sizeof(uint64));

	bool *is_null = memory_offset(hash, sizeof(uint64));
	memset(is_null, true, sizeof(bool));
}

// Function | Private | Can Swap
static bool can_swap (uint64 index_a, uint64 index_b, uint64 desired_index)
{
	if (index_b > index_a)
	{
		if ((desired_index <= index_a) or (desired_index > index_b))
			return true;
	}

	if (index_b < index_a)
	{
		if ((desired_index <= index_a) and (desired_index > index_b))
			return true;
	}

	return false;
}

// Function | Private | Insert Node
static void insert_node (Table this, Table_Node node)
{
	uint64 index = node.hash % this->slots;

	while (true)
	{
		if (*is_null_at(this, index))
		{
			assign_at(this, index, node);
			return;
		}

		index = (index + 1) % this->slots;
	}
}

// Function | Private | Erase Node
static void erase_node (Table this, uint64 index)
{
	uint64 current_index, desired_index;
	current_index = (index + 1) % this->slots;

	for (uint64 i = 0; i < this->slots - 1; i++)
	{
		if (*is_null_at(this, current_index))
			break;

		desired_index = *hash_at(this, current_index) % this->slots;

		if (can_swap(index, current_index, desired_index))
		{
			assign_at(this, index, node_at(this, current_index));
			index = current_index;
		}

		current_index = (current_index + 1) % this->slots;
	}

	destroy(key_at(this, index));
	destroy(value_at(this, index));
	*is_null_at(this, index) = true;
}

// Function | Private | Reference Destroy
static void ref_destroy (Table this)
{
	bool key_isnt_ref = this->key_type isnt TYPE(Ref);
	bool value_isnt_ref = this->value_type isnt TYPE(Ref);

	for (uint64 index = 0; index < this->slots; index++)
	{
		if (not *is_null_at(this, index))
		{
			if (key_isnt_ref)
				destroy(key_at(this, index));

			if (value_isnt_ref)
				destroy(value_at(this, index));
		}
	}

	free(this->table);
}

// Function | Private | Rehash Table
static bool rehash (Table this, uint64 size)
{
	if (size is 0)
	{
		Table_destroy(this);
		this->table = NULL;
		this->slots = 0;
		this->len = 0;
		return true;
	}

	uint64 old_size = this->slots;
	struct Table old_table = *this;
	var *new_table = malloc(size * this->slot_size);

	if (not new_table)
		return false;

	this->table = new_table;
	this->slots = size;

	for (uint64 index = 0; index < this->slots; index++)
		memory_init(this, index);

	for (uint64 index = 0; index < old_size; index++)
	{
		if (not *is_null_at(&old_table, index))
			insert_node(this, node_at(&old_table, index));
	}

	ref_destroy(&old_table);
	return true;
}

// Function | Private | Ideal Table Size
static uint64 ideal_size (uint64 size, double load_factor)
{
	double division;
	uint64 index, last;
	static uint64 max_size = UINT64_MAX;

	division = size / load_factor;
	size = (division < max_size) ? division : max_size;

	for (index = 0; index < num_primes; index++)
	{
		if (primes[index] >= size)
			return primes[index];
	}

	last = primes[index - 1];

	while (true)
	{
		last = (last < max_size / 2) ? last * 2 : max_size;

		if (last >= size)
			return last;
	}

	return 0;
}

// Function | Private | Adjust Table Size
static bool adjust_size (Table this)
{
	uint64 old_size = this->slots;
	uint64 new_size = ideal_size(this->len, this->load_factor);

	if (new_size != old_size)
		return rehash(this, new_size);

	return true;
}

// Function | Private | Search Key
static bool search_key (Table this, var key, uint64 *index)
{
	uint64 search_index = (this->slots > 0) ? (hash(key) % this->slots) : 0;

	for (uint64 i = 0; i < this->slots; i++)
	{
		if (*is_null_at(this, search_index))
			return false;

		if (eq(key_at(this, search_index), key))
		{
			if (index)
				*index = search_index;
			return true;
		}

		search_index = (search_index + 1) % this->slots;
	}

	return false;
}