// Include | Header
#include <pico_hlcr/Class/Zip.h>

// Method | Public | New -> build_with
void Zip_build_with (var this_ptr, Tuple args)
{
	THIS(Zip);
	this->iterables = new(Tuple);
	this->values = new(Tuple);
	assign(this->iterables, args);
	resize(this->values, len(args));
}

// Method | Public | New -> destroy
void Zip_destroy (var this_ptr)
{
	THIS(Zip);
	delete(this->iterables);
	delete(this->values);
}

// Method | Public | Len -> len
uint64 Zip_len (var this_ptr)
{
	THIS(Zip);
	var iterable = init(this->iterables);
	uint64 length = len(iterable);

	foreach (Int, index in range(I(1), I(len(this->iterables))))
	{
		iterable = next(this->iterables, iterable);
		if (iterable is ITER_STOP) break;
		uint64 current_len = len(iterable);
		length = (current_len < length) ? current_len : length;
	}

	return length;
}

// Method | Public | Iter -> init
var Zip_init (var this_ptr)
{
	THIS(Zip);
	var length = I(len(this->iterables));

	foreach (Int, index in range(length))
	{
		var iterable = get(this->iterables, index);
		var value = init(iterable);

		if (value is ITER_STOP)
			return ITER_STOP;

		set(this->values, index, value);
	}

	return this->values;
}

// Method | Public | Iter -> next
var Zip_next (var this_ptr, var current)
{
	THIS(Zip);
	var length = I(len(this->iterables));

	foreach (Int, index in range(length))
	{
		var iterable = get(this->iterables, index);
		var value = next(iterable, get(current, index));

		if (value is ITER_STOP)
			return ITER_STOP;

		set(this->values, index, value);
	}

	return this->values;
}

// Method | Public | Access -> first
var Zip_first (var this)
{
	return Zip_get(this, I(0));
}

// Method | Public | Access -> last
var Zip_last (var this)
{
	uint64 length = Zip_len(this);
	return Zip_get(this, I(length - 1));
}

// Method | Public | Access -> get
var Zip_get (var this_ptr, var key)
{
	THIS(Zip);

	if ((uint64) c_int(key) >= Zip_len(this))
		return NULL;

	foreach (Int, index in range(I(len(this->iterables))))
	{
		var iterable = get(this->iterables, index);
		var value = get(iterable, key);
		set(this->values, index, value);
	}

	return this->values;
}

// Method | Public | Content -> contains
bool Zip_contains (var this, var value)
{
	foreach (var, item in this)
	{
		if (eq(item, value))
			return true;
	}

	return false;
}

// Method | Public | Content -> index_of
var Zip_index_of (var this_ptr, var value)
{
	THIS(Zip);
	uint64 index = 0;

	foreach (var, item in this)
	{
		if (eq(item, value))
			return new(Int, I(index));
		index++;
	}

	return NULL;
}

// Method | Public | Content -> key_type
Type Zip_key_type (var this)
{
	UNUSED(this);
	return TYPE(Int);
}

// Method | Public | Content -> value_type
Type Zip_value_type (var this)
{
	UNUSED(this);
	return TYPE(Tuple);
}

// Method | Public | Cast -> to_string
String Zip_to_string (var this_ptr)
{
	THIS(Zip);
	String casted = new(String);
	var last_iter = last(this->iterables);
	print_to(casted, 0, "zip(");

	foreach (var, iterable in this->iterables)
	{
		concat(casted, S(type_name(iterable)));

		if (iterable isnt last_iter)
			concat(casted, S(", "));
	}

	concat(casted, S(")"));
	return casted;
}