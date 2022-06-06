// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Internal | Call With
var call_with (var this, var variable)
{
	INSTANCE(Call);

	if (IMPLEMENTS(Call, call_with))
		return INS(Call)->call_with(this, variable);

	REPORT_METHOD(Call, call_with);
	return NULL;
}

// Interface | Public | Call
Type TYPE(Call) = new_type(Call);