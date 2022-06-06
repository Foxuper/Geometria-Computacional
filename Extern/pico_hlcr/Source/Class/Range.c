// Include | Header
#include <pico_hlcr/Class/Range.h>

// Function | Public | Internal | Range Initializer
Range range_init (Range this, Tuple args)
{
	var arg_a = get(args, I(0));
	var arg_b = get(args, I(1));
	var arg_c = get(args, I(2));

	this->start = 0;
	this->stop = 0;
	this->step = 1;

	switch (len(args))
	{
		case 1:
			this->stop = (arg_a is _) ? 0 : c_int(arg_a);
		break;
		case 2:
			this->start = (arg_a is _) ? 0 : c_int(arg_a);
			this->stop = (arg_b is _) ? 0 : c_int(arg_b);
		break;
		case 3:
			this->start = (arg_a is _) ? 0 : c_int(arg_a);
			this->stop = (arg_b is _) ? 0 : c_int(arg_b);
			this->step = (arg_c is _) ? 1 : c_int(arg_c);
		break;
	}

	if (this->start is this->stop)
		this->step = 0;

	if (this->stop < this->start and this->step > 0)
		this->step *= -1;

	if (this->stop > this->start and this->step < 0)
		this->step *= -1;

	return this;
}

// Method | Public | New -> build_with
void Range_build_with (var this_ptr, Tuple args)
{
	THIS(Range);
	this->value = new(Int);
	range_init(this, args);
}

// Method | Private | New -> destroy
void Range_destroy (var this_ptr)
{
	THIS(Range);
	delete(this->value);
}

// Method | Private | Compare -> cmp
int64 Range_cmp (var this, var other)
{
	var a = Range_init(this);
	var b = init(other);

	while (true)
	{
		if (a is ITER_STOP and b is ITER_STOP) return 0;
		if (a is ITER_STOP) return -1;
		if (b is ITER_STOP) return 1;

		int64 difference = cmp(a, b);

		if (difference is 0)
		{
			a = Range_next(this, a);
			b = next(other, b);
		}
		else
			return difference;
	}
}

// Method | Private | Hash -> hash
uint64 Range_hash (var this)
{
	uint64 h = 0;

	foreach (var, value in this)
		h ^= hash(value);

	return h;
}

// Method | Private | Len -> len
uint64 Range_len (var this_ptr)
{
	THIS(Range);

	if (this->step is 0)
		return 0;

	if (this->step > 0)
		return (this->stop - this->start - 1) / this->step + 1;

	if (this->step < 0)
		return (this->start - this->stop - 1) / -this->step + 1;

	return 0;
}

// Method | Private | Iter -> init
var Range_init (var this_ptr)
{
	THIS(Range);
	this->value->value = this->start;
	return (this->step isnt 0) ? this->value : (var) ITER_STOP;
}

// Method | Private | Iter -> next
var Range_next (var this_ptr, var current)
{
	THIS(Range);
	UNUSED(current);
	Int x = this->value;
	x->value += this->step;

	if (this->step > 0 and x->value >= this->stop)
		return ITER_STOP;

	if (this->step < 0 and x->value <= this->stop)
		return ITER_STOP;

	return (this->step isnt 0) ? this->value : (var) ITER_STOP;
}

// Method | Private | Iter -> iter_type
Type Range_iter_type (var this)
{
	UNUSED(this);
	return TYPE(Int);
}

// Method | Private | Cast -> to_string
String Range_to_string (var this_ptr)
{
	THIS(Range);
	String casted = new(String);
	var start = I(this->start);
	var stop = I(this->stop);
	var step = I(this->step);
	print_to(casted, 0, "range(#v, #v, #v)", start, stop, step);
	return casted;
}

// Method | Private | Access -> first
var Range_first (var this_ptr)
{
	THIS(Range);
	this->value->value = this->start;
	return (this->step isnt 0) ? this->value : NULL;
}

// Method | Private | Access -> last
var Range_last (var this_ptr)
{
	THIS(Range);
	Int x = this->value;
	x->value = this->stop - this->step;

	if (this->step > 0 and x->value >= this->start)
		return x;

	if (this->step < 0 and x->value <= this->start)
		return x;

	return NULL;
}

// Method | Private | Access -> get
var Range_get (var this_ptr, var key)
{
	THIS(Range);
	Int x = this->value;
	int64 index = c_int(key);

	if (index < (int64) Range_len(this))
	{
		if (this->step > 0 and (this->start + this->step * index) < this->stop)
		{
			x->value = this->start + this->step * index;
			return x;
		}

		if (this->step < 0 and (this->start + this->step * index) > this->stop)
		{
			x->value = this->start + this->step * index;
			return x;
		}
	}

	return NULL;
}

// Method | Private | Content -> contains
bool Range_contains (var this_ptr, var value)
{
	THIS(Range);
	int64 x = c_int(value);

	if (this->step is 0)
		return false;

	if (this->start <= x and x < this->stop)
		return (x - this->start) % this->step is 0;

	if (this->stop < x and x <= this->start)
		return (x - this->start) % this->step is 0;

	return false;
}

// Method | Private | Content -> index_of
var Range_index_of (var this_ptr, var value)
{
	THIS(Range);
	int64 x = c_int(value);

	if (Range_contains(this, value))
		return new(Int, I((x - this->start) / this->step));

	return NULL;
}

// Method | Private | Content -> key_type
Type Range_key_type (var this)
{
	UNUSED(this);
	return TYPE(Int);
}

// Method | Private | Content -> key_type
Type Range_value_type (var this)
{
	UNUSED(this);
	return TYPE(Int);
}