// Include | Header
#include <pico_hlcr/Class/Int.h>

// Core Includes
#include <inttypes.h>

// Method | Public | C_Types -> c_int
int64 Int_c_int (var this_ptr)
{
	THIS(Int);
	return this->value;
}

// Method | Public | C_Types -> c_float
double Int_c_float (var this_ptr)
{
	THIS(Int);
	return this->value;
}

// Method | Public | Compare -> cmp 
int64 Int_cmp (var this_ptr, var other_ptr)
{
	THIS(Int);

	if (type_of(other_ptr) isnt TYPE(Int))
		return (intptr) this_ptr - (intptr) other_ptr;

	Int other = other_ptr;
	return this->value - other->value;
}

// Method | Public | Hash -> hash
uint64 Int_hash (var this_ptr)
{
	THIS(Int);
	return this->value;
}

// Method | Public | Cast -> cast_to
var Int_cast_to (var this_ptr, Type type)
{
	THIS(Int);

	if (type is TYPE(Float))
	{
		Float casted = new(Float);
		casted->value = this->value;
		return casted;
	}

	return NULL;
}

// Method | Public | Cast -> to_string
String Int_to_string (var this)
{
	String casted = new(String);
	Int_show_to(this, casted, 0);
	return casted;
}

// Method | Public | IO -> show_to
int64 Int_show_to (var this_ptr, var other, uint64 index)
{
	THIS(Int);
	return format_to(other, index, "%" PRId64, this->value);
}

// Method | Public | IO -> look_from
int64 Int_look_from (var this_ptr, var other, uint64 index)
{
	THIS(Int);
	int64 scanned = 0;
	int64 error = format_from(other, index, "%" PRId64 "%n", &this->value, &scanned);
	return error < 0 ? error : scanned;
}