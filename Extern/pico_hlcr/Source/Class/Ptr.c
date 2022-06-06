// Include | Header
#include <pico_hlcr/Class/Ptr.h>

// Method | Public | Assign -> assign
void Ptr_assign (var this_ptr, var other)
{
	THIS(Ptr);
	this->pointer = deref(other);
}

// Method | Public | Compare -> cmp
int64 Ptr_cmp (var this_ptr, var other)
{
	THIS(Ptr);
	return cmp(this->pointer, deref(other));
}

// Method | Public | Hash -> hash
uint64 Ptr_hash (var this_ptr)
{
	THIS(Ptr);
	return hash(this->pointer);
}

// Method | Public | Cast -> cast_to
var Ptr_cast_to (var this_ptr, Type type)
{
	THIS(Ptr);

	if (type is TYPE(Ref))
	{
		Ref casted = new(Ref);
		casted->reference = this->pointer;
		return casted;
	}

	return NULL;
}

// Method | Public | Cast -> to_string
String Ptr_to_string (var this_ptr)
{
	THIS(Ptr);
	String casted = new(String);
	print_to(casted, 0, "ptr(#v)", this->pointer);
	return casted;
}

// Method | Public | Content -> contains
bool Ptr_contains (var this_ptr, var value)
{
	THIS(Ptr);

	if (this->pointer is value)
		return true;

	return contains(this->pointer, value);
}

// Method | Public | Pointer -> ref
void Ptr_ref (var this_ptr, var value)
{
	THIS(Ptr);
	this->pointer = value;
}

// Method | Public | Pointer -> deref
var Ptr_deref (var this_ptr)
{
	THIS(Ptr);
	return this->pointer;
}