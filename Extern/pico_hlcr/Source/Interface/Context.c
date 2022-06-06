// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Enter
var enter (var this)
{
	INSTANCE(Context);

	if (IMPLEMENTS(Context, enter))
		INS(Context)->enter(this);

	return this;
}

// Method | Public | Exit
var leave (var this)
{
	INSTANCE(Context);

	if (IMPLEMENTS(Context, leave))
		INS(Context)->leave(this);

	delete(this);
	return WITH_EXIT;
}

// Interface | Public | Constants
Type WITH_EXIT = empty_type(WITH_EXIT);

// Interface | Public | Context
Type TYPE(Context) = new_type(Context);