// Include | Header
#include <pico_hlcr/Class/Slice.h>

// Method | Public | New -> build_with
void Slice_build_with (var this_ptr, Tuple args)
{
	THIS(Slice);
	this->iterable = first(args);
	with (Tuple, limits as sublist(args, I(1), I(4)))
		this->range = new(Range, limits);
}

// Method | Public | New -> destroy
void Slice_destroy (var this_ptr)
{
	THIS(Slice);
	delete(this->range);
}

// Method | Public | Compare -> cmp
int64 Slice_cmp (var this, var other)
{
	var a = Slice_init(this);
	var b = init(other);

	while (true)
	{
		if (a is ITER_STOP and b is ITER_STOP) return 0;
		if (a is ITER_STOP) return -1;
		if (b is ITER_STOP) return 1;

		int64 difference = cmp(a, b);

		if (difference is 0)
		{
			a = Slice_next(this, a);
			b = next(other, b);
		}
		else
			return difference;
	}
}

// Method | Public | Hash -> hash
uint64 Slice_hash (var this)
{
	uint64 h = 0;

	foreach (var, value in this)
		h ^= hash(value);

	return h;
}

// Method | Public | Len -> len
uint64 Slice_len (var this_ptr)
{
	THIS(Slice);
	return len(this->range);
}

// Method | Public | Iter -> init
var Slice_init (var this_ptr)
{
	THIS(Slice);
	var key = init(this->range);
	return (key is ITER_STOP) ? ITER_STOP : get(this->iterable, key);
}

// Method | Public | Iter -> next
var Slice_next (var this_ptr, var current)
{
	THIS(Slice);
	UNUSED(current);
	Range range = this->range;
	var key = next(range, range->value);
	return (key is ITER_STOP) ? ITER_STOP : get(this->iterable, key);
}

// Method | Public | Iter -> iter_type
Type Slice_iter_type (var this_ptr)
{
	THIS(Slice);
	return iter_type(this->iterable);
}

// Method | Public | Cast -> to_string
String Slice_to_string (var this)
{
	String casted = new(String);
	var last = Slice_last(this);
	concat(casted, S("/"));

	foreach (var, value in this)
	{
		with (String, string as str(value))
			concat(casted, string);

		if (value isnt last)
			concat(casted, S(", "));
	}
	
	concat(casted, S("/"));
	return casted;
}

// Method | Public | Access -> first
var Slice_first (var this_ptr)
{
	THIS(Slice);
	var key = first(this->range);
	return get(this->iterable, key);
}

// Method | Public | Access -> last
var Slice_last (var this_ptr)
{
	THIS(Slice);
	var key = last(this->range);
	return get(this->iterable, key);
}

// Method | Public | Access -> get
var Slice_get (var this_ptr, var key)
{
	THIS(Slice);
	key = get(this->range, key);
	return get(this->iterable, key);
}

// Method | Public | Access -> set
void Slice_set (var this_ptr, var key, var value)
{
	THIS(Slice);
	key = get(this->range, key);
	set(this->iterable, key, value);
}

// Method | Public | Content -> contains
bool Slice_contains (var this, var value)
{
	foreach (var, element in this)
	{
		if (eq(element, value))
			return true;
	}

	return false;
}

// Method | Public | Content -> index_of
var Slice_index_of (var this, var value)
{
	uint64 index = 0;
	
	foreach (var, element in this)
	{
		if (eq(element, value))
			return new(Int, I(index));
		index++;
	}

	return NULL;
}

// Method | Public | Content -> key_type
Type Slice_key_type (var this)
{
	UNUSED(this);
	return TYPE(Int);
}

// Method | Public | Content -> value_type
Type Slice_value_type (var this_ptr)
{
	THIS(Slice);
	return value_type(this->iterable);
}