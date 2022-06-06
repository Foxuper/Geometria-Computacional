// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Delete
void delete (var this)
{
	if (not alloc_at_heap(this))
	{
		String this_name = S(type_name(this));
		throw(RESOURCE_EXCEPTION, "Attempt to delete non heap variable '#v'", this_name);
	}

	dealloc(destroy(this));
}

// Method | Public | Destroy
var destroy (var this)
{
	INSTANCE(New);

	if (IMPLEMENTS(New, destroy))
		INS(New)->destroy(this);

	return this;
}

// Method | Public | Internal | New With
var new_with (Type type, Tuple args)
{
	return build_with(alloc(type), args);
}

// Method | Public | Internal | Build With
var build_with (var this, Tuple args)
{
	INSTANCE(New);

	if (IMPLEMENTS(New, build_with))
	{
		INS(New)->build_with(this, args);
		return this;
	}

	return assign(this, first(args));
}

// Interface | Public | New
Type TYPE(New) = new_type(New);