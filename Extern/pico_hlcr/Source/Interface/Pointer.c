// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Reference
void ref (var this, var value)
{
	INSTANCE(Pointer);

	if (IMPLEMENTS(Pointer, ref))
	{
		INS(Pointer)->ref(this, value);
		return;
	}

	REPORT_METHOD(Pointer, ref);
}

// Method | Public | Dereference
var deref (var this)
{
	INSTANCE(Pointer);

	if (IMPLEMENTS(Pointer, deref))
		return INS(Pointer)->deref(this);

	return this;
}

// Function | Public | Is Pointer
bool is_pointer (var this)
{
	GET_TYPE(this);
	return (type is VAR) or (type is TYPE(Ptr)) or (type is TYPE(Ref));
}

// Interface | Public | Constants
Type VAR = empty_type(VAR);

// Interface | Public | Pointer
Type TYPE(Pointer) = new_type(Pointer);