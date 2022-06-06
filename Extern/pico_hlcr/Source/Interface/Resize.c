// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Resize Object
void resize (var this, uint64 new_len)
{
	if (not alloc_at_heap(this))
	{
		String this_name = S(type_name(this));
		throw(RESOURCE_EXCEPTION, "Attempt to resize non heap variable '#v'", this_name);
	}

	INSTANCE(Resize);

	if (IMPLEMENTS(Resize, resize))
	{
		INS(Resize)->resize(this, new_len);
		return;
	}

	REPORT_METHOD(Resize, resize);
}

// Interface | Public | Resize
Type TYPE(Resize) = new_type(Resize);