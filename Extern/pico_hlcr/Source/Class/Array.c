// Include | Header
#include <pico_hlcr/Class/Array.h>

// Functions | Private | Array
static var memory_at   (Array this, uint64 index);
static var value_at    (Array this, uint64 index);
static var memory_init (Array this, uint64 index);

// Function | Private | Memory at Index
static var memory_at (Array this, uint64 index)
{
	return (uint8 *) this->array + (index * this->slot_size);
}

// Function | Private | Value at Index
static var value_at (Array this, uint64 index)
{
	return memory_offset(memory_at(this, index), HEADER_SIZE);
}

// Function | Private | Initialize Memory at Index
static var memory_init (Array this, uint64 index)
{
	var memory = memory_at(this, index);
	var value = header_init(memory, this->type, ALLOC_DATA);
	memset(value, 0, size(this->type));
	return build(value);
}

// Method | Public | New -> build_with
void Array_build_with (var this_ptr, Tuple args)
{
	THIS(Array);
	var value = init(args);

	this->array = NULL;
	this->type = NULL;
	this->len = 0;
	this->slots = 0;
	this->slot_size = 0;

	if (value is ITER_STOP)
	{
		this->type = TYPE(Ptr);
		this->slot_size = size(this->type) + HEADER_SIZE;
		return;
	}

	if (not is_type(value))
	{
		Array_assign(this, args);
		return;
	}

	this->type = (value is VAR) ? TYPE(Ptr) : value;
	this->len = len(args) - 1;
	this->slots = this->len;
	this->slot_size = size(this->type) + HEADER_SIZE;
	this->array = calloc(1, this->slots * this->slot_size);

	for (uint64 index = 0; index < this->len; index++)
	{
		var current = memory_init(this, index);
		value = next(args, value);
		assign(current, value);
	}
}

// Method | Public | New -> destroy
void Array_destroy (var this_ptr)
{
	THIS(Array);

	for (uint64 index = 0; index < this->slots; index++)
		destroy(value_at(this, index));

	free(this->array);

	this->array = NULL;
	this->len = 0;
	this->slots = 0;
}

// Method | Public | Assign -> assign
void Array_assign (var this_ptr, var other)
{
	THIS(Array);
	Array_destroy(this);

	uint64 index = 0;
	Type type = value_type(other);

	this->type = (type is VAR) ? TYPE(Ptr) : type;
	this->len = len(other);
	this->slots = this->len;
	this->slot_size = size(this->type) + HEADER_SIZE;
	this->array = calloc(1, this->slots * this->slot_size);

	foreach (var, value in other)
	{
		var current = memory_init(this, index++);
		assign(current, value);
	}
}

// Method | Public | Compare -> cmp
int64 Array_cmp (var this, var other)
{
	var a = Array_init(this);
	var b = init(other);

	while (true)
	{
		if (a is ITER_STOP and b is ITER_STOP) return 0;
		if (a is ITER_STOP) return -1;
		if (b is ITER_STOP) return 1;

		int64 difference = cmp(a, b);

		if (difference is 0)
		{
			a = Array_next(this, a);
			b = next(other, b);
		}
		else
			return difference;
	}
}

// Method | Public | Hash -> hash
uint64 Array_hash (var this_ptr)
{
	THIS(Array);
	uint64 h = 0;

	for (uint64 index = 0; index < this->len; index++)
		h ^= hash(value_at(this, index));

	return h;
}

// Method | Public | Len -> len
uint64 Array_len (var this_ptr)
{
	THIS(Array);
	return this->len;
}

// Method | Public | Resize -> resize
void Array_resize (var this_ptr, uint64 new_len)
{
	THIS(Array);
	uint64 factor = new_len * ALLOC_FACTOR;

	if (new_len > this->slots)
	{
		this->slots = factor;
		this->array = realloc(this->array, this->slots * this->slot_size);
		for (uint64 index = this->len; index < this->slots; index++)
			memory_init(this, index);
	}

	if (this->slots > factor)
	{
		for (uint64 index = new_len; index < this->slots; index++)
			destroy(value_at(this, index));
		this->slots = new_len;
		this->array = realloc(this->array, this->slots * this->slot_size);
	}

	this->len = new_len;
}

// Method | Public | Iter -> init
var Array_init (var this_ptr)
{
	THIS(Array);
	return (this->len > 0) ? value_at(this, 0) : ITER_STOP;
}

// Method | Public | Iter -> next
var Array_next (var this_ptr, var current)
{
	THIS(Array);

	if (current >= value_at(this, this->len - 1))
		return ITER_STOP;

	return memory_offset(current, this->slot_size);
}

// Method | Public | Iter -> iter_type
Type Array_iter_type (var this_ptr)
{
	THIS(Array);
	return this->type;
}

// Method | Public | Cast -> to_string
String Array_to_string (var this)
{
	String casted = new(String);
	var last = Array_last(this);
	concat(casted, S("|"));

	foreach (var, value in this)
	{
		with (String, string as str(value))
			concat(casted, string);

		if (value isnt last)
			concat(casted, S(", "));
	}

	concat(casted, S("|"));
	return casted;
}

// Method | Public | Concat -> concat
void Array_concat (var this_ptr, var other)
{
	THIS(Array);
	Type type = value_type(other);
	type = (type is VAR) ? TYPE(Ptr) : type;

	if (type is this->type)
	{
		foreach (var, value in other)
			Array_insert(this, value);
	}
}

// Method | Public | Sublist -> sublist
var Array_sublist (var this_ptr, var key_a, var key_b)
{
	THIS(Array);
	uint64 index_a = c_int(key_a);
	uint64 index_b = c_int(key_b);

	if (index_a <= index_b and index_b <= this->len)
	{
		Array sub_list = new(Array, this->type);
		Array_resize(sub_list, index_b - index_a);
		uint64 bytes = sub_list->len * sub_list->slot_size;
		memcpy(memory_at(sub_list, 0), memory_at(this, index_a), bytes);
		return sub_list;
	}

	return NULL;
}

// Method | Public | Access -> first
var Array_first (var this_ptr)
{
	THIS(Array);
	return (this->len > 0) ? value_at(this, 0) : NULL;
}

// Method | Public | Access -> last
var Array_last (var this_ptr)
{
	THIS(Array);
	return (this->len > 0) ? value_at(this, this->len - 1) : NULL;
}

// Method | Public | Access -> get
var Array_get (var this_ptr, var key)
{
	THIS(Array);
	uint64 index = c_int(key);
	return (index < this->len) ? value_at(this, index) : NULL;
}

// Method | Public | Access -> set
void Array_set (var this_ptr, var key, var value)
{
	THIS(Array);
	uint64 index = c_int(key);
	if (index < this->len)
		assign(value_at(this, index), value);
}

// Method | Public | Content -> contains
bool Array_contains (var this_ptr, var value)
{
	THIS(Array);

	for (uint64 index = 0; index < this->len; index++)
	{
		if (eq(value_at(this, index), value))
			return true;
	}

	return false;
}

// Method | Public | Content -> index_of
var Array_index_of (var this_ptr, var value)
{
	THIS(Array);

	for (uint64 index = 0; index < this->len; index++)
	{
		if (eq(value_at(this, index), value))
			return new(Int, I(index));
	}

	return NULL;
}

// Method | Public | Content -> key_type
Type Array_key_type (var this)
{
	UNUSED(this);
	return TYPE(Int);
}

// Method | Public | Content -> value_type
Type Array_value_type (var this_ptr)
{
	THIS(Array);
	return this->type;
}

// Method | Public | Insert -> insert
void Array_insert (var this_ptr, var value)
{
	THIS(Array);
	Array_resize(this, this->len + 1);
	assign(value_at(this, this->len - 1), value);
}

// Method | Public | Insert -> erase
void Array_erase (var this_ptr, var value)
{
	THIS(Array);

	for (uint64 index = 0; index < this->len; index++)
	{
		if (eq(value_at(this, index), value))
		{
			Array_erase_at(this, I(index));
			return;
		}
	}
}

// Method | Public | Insert -> insert_at
void Array_insert_at (var this_ptr, var key, var value)
{
	THIS(Array);
	uint64 index = c_int(key);

	if (index <= this->len)
	{
		Array_resize(this, this->len + 1);
		uint64 size = (this->len - index - 1) * this->slot_size;
		memmove(memory_at(this, index + 1), memory_at(this, index), size);
		Array_set(this, key, value);
	}
}

// Method | Public | Insert -> erase_at
void Array_erase_at (var this_ptr, var key)
{
	THIS(Array);
	uint64 index = c_int(key);

	if (index < this->len)
	{
		destroy(value_at(this, index));
		uint64 size = (this->len - index - 1) * this->slot_size;
		memmove(memory_at(this, index), memory_at(this, index + 1), size);
		Array_resize(this, this->len - 1);
	}
}