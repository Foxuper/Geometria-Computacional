// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Definition | Private | Self Reference Stack Key
#define SR_STACK_KEY S("__SR_STACK_KEY__")

// Method | Public | Current
var current (Type type)
{
	TYPE_INSTANCE(Current);

	if (IMPLEMENTS(Current, current))
		return INS(Current)->current();

	TYPE_REPORT_METHOD(Current, current);
	return NULL;
}

// Interface | Public | Current
Type TYPE(Current) = new_type(Current);