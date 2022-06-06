// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Get Size
uint64 size (var this)
{
	INSTANCE(Size);

	if (IMPLEMENTS(Size, size))
		return INS(Size)->size();
	
	GET_TYPE(this);
	return type ? type->size : 0;
}

// Interface | Public | Size
Type TYPE(Size) = new_type(Size);