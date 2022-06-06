// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Allocate
var alloc (Type type)
{
	if (not type)
		return NULL;

	TYPE_INSTANCE(Alloc);

	if (IMPLEMENTS(Alloc, alloc))
		return INS(Alloc)->alloc();

	var memory = calloc(1, HEADER_SIZE + size(type));
	return header_init(memory, type, ALLOC_HEAP);
}

// Method | Public | Deallocate
void dealloc (var this)
{
	if (not alloc_at_heap(this))
	{
		String this_name = S(type_name(this));
		throw(RESOURCE_EXCEPTION, "Attempt to deallocate non heap variable '#v'", this_name);
	}

	INSTANCE(Alloc);

	if (IMPLEMENTS(Alloc, dealloc))
		INS(Alloc)->dealloc(this);

	free(memory_offset(this, -HEADER_SIZE));
}

// Interface | Public | Alloc
Type TYPE(Alloc) = new_type(Alloc);