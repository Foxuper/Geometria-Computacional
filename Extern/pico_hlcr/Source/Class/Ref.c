// Include | Header
#include <pico_hlcr/Class/Ref.h>

// Method | Public | New -> destroy
void Ref_destroy (var this_ptr)
{
	THIS(Ref);
	delete(this->reference);
}

// Method | Public | Assign -> assign
void Ref_assign (var this_ptr, var other)
{
	THIS(Ref);
	this->reference = deref(other);
}

// Method | Public | Assign -> copy
var Ref_copy (var this_ptr)
{
	THIS(Ref);
	Ref ref_copy = alloc(TYPE(Ref));
	ref_copy->reference = copy(this->reference);
	return ref_copy;
}

// Method | Public | Compare -> cmp
int64 Ref_cmp (var this_ptr, var other)
{
	THIS(Ref);
	return cmp(this->reference, deref(other));
}

// Method | Public | Hash -> hash
uint64 Ref_hash (var this_ptr)
{
	THIS(Ref);
	return hash(this->reference);
}

// Method | Public | Context -> enter
void Ref_enter (var this_ptr)
{
	THIS(Ref);
	enter(this->reference);
}

// Method | Public | Context -> leave
void Ref_leave (var this_ptr)
{
	THIS(Ref);
	leave(this->reference);
	this->reference = NULL;
}

// Method | Public | Cast -> cast_to
var Ref_cast_to (var this_ptr, Type type)
{
	THIS(Ref);

	if (type is TYPE(Ptr))
	{
		Ptr casted = new(Ptr);
		casted->pointer = this->reference;
		return casted;
	}

	return NULL;
}

// Method | Public | Cast -> to_string
String Ref_to_string (var this_ptr)
{
	THIS(Ref);
	String casted = new(String);
	print_to(casted, 0, "ref(#v)", this->reference);
	return casted;
}

// Method | Public | Content -> contains
bool Ref_contains (var this_ptr, var value)
{
	THIS(Ref);

	if (this->reference is value)
		return true;

	return contains(this->reference, value);
}

// Method | Public | Pointer -> ref
void Ref_ref (var this_ptr, var value)
{
	THIS(Ref);
	this->reference = value;
}

// Method | Public | Pointer -> deref
var Ref_deref (var this_ptr)
{
	THIS(Ref);
	return this->reference;
}