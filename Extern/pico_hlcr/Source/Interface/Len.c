// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Len
uint64 len (var this)
{
	if (this is NULL)
		return 0;

	INSTANCE(Len);

	if (IMPLEMENTS(Len, len))
		return INS(Len)->len(this);

	REPORT_METHOD(Len, len);
	return 0;
}

// Method | Public | Is Empty
bool empty (var this)
{
	return len(this) is 0;
}

// Interface | Public | Len
Type TYPE(Len) = new_type(Len);