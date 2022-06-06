// Include | Header
#include <pico_hlcr/Class/Tuple.h>

// Method | Public | New -> build_with
void Tuple_build_with (var this_ptr, Tuple args)
{
	THIS(Tuple);
	this->array = NULL;
	this->len = 0;
	this->index = 0;
	Tuple_assign(this, args);
}

// Method | Public | New -> destroy
void Tuple_destroy (var this_ptr)
{
	THIS(Tuple);
	free(this->array);
}

// Method | Public | Assign -> assign
void Tuple_assign (var this_ptr, var other)
{
	THIS(Tuple);
	uint64 index = 0;
	uint64 length = len(other);
	Tuple_resize(this, length);

	foreach (var, value in other)
		this->array[index++] = value;
}

// Method | Public | Compare -> cmp
int64 Tuple_cmp (var this, var other)
{
	var a = Tuple_init(this);
	var b = init(other);

	while (true)
	{
		if (a is ITER_STOP and b is ITER_STOP) return 0;
		if (a is ITER_STOP) return -1;
		if (b is ITER_STOP) return 1;

		int64 difference = cmp(a, b);

		if (difference is 0)
		{
			a = Tuple_next(this, a);
			b = next(other, b);
		}
		else
			return difference;
	}
}

// Method | Public | Hash -> hash
uint64 Tuple_hash (var this_ptr)
{
	THIS(Tuple);
	uint64 h = 0;

	for (uint64 index = 0; index < this->len; index++)
		h ^= hash(this->array[index]);

	return h;
}

// Method | Public | Len -> len
uint64 Tuple_len (var this_ptr)
{
	THIS(Tuple);
	return this->len;
}

// Methods | Public | Resize -> resize
void Tuple_resize (var this_ptr, uint64 new_len)
{
	THIS(Tuple);

	if (this->len != new_len)
	{
		this->array = realloc(this->array, sizeof(var) * new_len);
		this->len = new_len;
	}
}

// Method | Public | Iter -> init
var Tuple_init (var this_ptr)
{
	THIS(Tuple);

	if (this->len > 0)
	{
		this->index = 1;
		return this->array[0];
	}

	return ITER_STOP;
}

// Method | Public | Iter -> next
var Tuple_next (var this_ptr, var current)
{
	THIS(Tuple);
	UNUSED(current);
	return (this->index < this->len) ? this->array[this->index++] : ITER_STOP;
}

// Method | Public | Cast -> to_string
String Tuple_to_string (var this)
{
	String casted = new(String);
	uint64 length = len(this);
	uint64 index = 1;

	concat(casted, S("("));

	foreach (var, value in this)
	{
		with (String, string as str(value))
			concat(casted, string);

		if (index < length)
		{
			concat(casted, S(", "));
			index++;
		}
	}

	concat(casted, S(")"));
	return casted;
}

// Method | Public | Concat -> concat
void Tuple_concat (var this_ptr, var other)
{
	THIS(Tuple);
	uint64 index = this->len;
	uint64 length = len(other);
	Tuple_resize(this, this->len + length);

	foreach (var, value in other)
		this->array[index++] = value;
}

// Method | Public | Sublist -> sublist
var Tuple_sublist (var this_ptr, var key_a, var key_b)
{
	THIS(Tuple);
	uint64 index_a = c_int(key_a);
	uint64 index_b = c_int(key_b);

	if (index_a <= index_b and index_b <= this->len)
	{
		Tuple sub_list = new(Tuple);
		Tuple_resize(sub_list, index_b - index_a);
		uint64 bytes = sub_list->len * sizeof(var);
		memcpy(sub_list->array, &this->array[index_a], bytes);
		return sub_list;
	}

	return NULL;
}

// Method | Public | Access -> first
var Tuple_first (var this_ptr)
{
	THIS(Tuple);
	return (this->len > 0) ? this->array[0] : NULL;
}

// Method | Public | Access -> last
var Tuple_last (var this_ptr)
{
	THIS(Tuple);
	return (this->len > 0) ? this->array[this->len - 1] : NULL;
}

// Method | Public | Access -> get
var Tuple_get (var this_ptr, var key)
{
	THIS(Tuple);
	uint64 index = c_int(key);

	if (index < this->len)
		return this->array[index];

	return NULL;
}

// Method | Public | Access -> set
void Tuple_set (var this_ptr, var key, var value)
{
	THIS(Tuple);
	uint64 index = c_int(key);

	if (index < this->len)
		this->array[index] = value;
}

// Method | Public | Content -> contains
bool Tuple_contains (var this_ptr, var value)
{
	THIS(Tuple);

	for (uint64 index = 0; index < this->len; index++)
	{
		if (eq(this->array[index], value))
			return true;
	}

	return false;
}

// Method | Public | Content -> index_of
var Tuple_index_of (var this_ptr, var value)
{
	THIS(Tuple);

	for (uint64 index = 0; index < this->len; index++)
	{
		if (eq(this->array[index], value))
			return new(Int, I(index));
	}

	return NULL;
}

// Method | Public | Content -> key_type
Type Tuple_key_type (var this)
{
	UNUSED(this);
	return TYPE(Int);
}

// Method | Public | Insert -> insert
void Tuple_insert (var this_ptr, var value)
{
	THIS(Tuple);
	Tuple_resize(this, this->len + 1);
	this->array[this->len - 1] = value;
}

// Method | Public | Insert -> erase
void Tuple_erase (var this_ptr, var value)
{
	THIS(Tuple);

	for (uint64 index = 0; index < this->len; index++)
	{
		if (eq(this->array[index], value))
			Tuple_erase_at(this, I(index));
	}
}

// Method | Public | Insert -> insert_at
void Tuple_insert_at (var this_ptr, var key, var value)
{
	THIS(Tuple);
	uint64 index = c_int(key);

	if (index <= this->len)
	{
		Tuple_resize(this, this->len + 1);
		uint64 bytes = (this->len - index - 1) * sizeof(var);
		memmove(&this->array[index + 1], &this->array[index], bytes);
		Tuple_set(this, key, value);
	}
}

// Method | Public | Insert -> erase_at
void Tuple_erase_at (var this_ptr, var key)
{
	THIS(Tuple);
	uint64 index = c_int(key);

	if (index < this->len)
	{
		uint64 bytes = (this->len - index - 1) * sizeof(var);
		memmove(&this->array[index], &this->array[index + 1], bytes);
		Tuple_resize(this, this->len - 1);
	}
}