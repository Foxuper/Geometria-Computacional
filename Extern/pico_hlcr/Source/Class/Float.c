// Include | Header
#include <pico_hlcr/Class/Float.h>

// Method | Public | C_Types -> c_int
int64 Float_c_int (var this_ptr)
{
	THIS(Float);
	return this->value;
}

// Method | Public | C_Types -> c_float
double Float_c_float (var this_ptr)
{
	THIS(Float);
	return this->value;
}

// Method | Public | Compare -> cmp 
int64 Float_cmp (var this_ptr, var other_ptr)
{
	THIS(Float);

	if (type_of(other_ptr) isnt TYPE(Float))
		return (intptr) this_ptr - (intptr) other_ptr;

	Float other = other_ptr;
	double difference = this->value - other->value;

	if (difference > 0)
		return 1;

	if (difference < 0)
		return -1;

	return 0;
}

// Method | Public | Hash -> hash
uint64 Float_hash (var this_ptr)
{
	THIS(Float);
	return *((uint64 *) &this->value);
}

// Method | Public | Cast -> cast_to
var Float_cast_to (var this_ptr, Type type)
{
	THIS(Float);

	if (type is TYPE(Int))
	{
		Int casted = new(Int);
		casted->value = this->value;
		return casted;
	}

	return NULL;
}

// Method | Public | Cast -> to_string
String Float_to_string (var this)
{
	String casted = new(String);
	Float_show_to(this, casted, 0);
	return casted;
}

// Method | Public | IO -> show_to
int64 Float_show_to (var this_ptr, var other, uint64 index)
{
	THIS(Float);
	return format_to(other, index, "%g", this->value);
}

// Method | Public | IO -> look_from
int64 Float_look_from (var this_ptr, var other, uint64 index)
{
	THIS(Float);
	int64 scanned = 0;
	int64 error = format_from(other, index, "%lf%n", &this->value, &scanned);
	return error < 0 ? error : scanned;
}