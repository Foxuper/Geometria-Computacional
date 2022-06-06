// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Add
void concat (var this, var other)
{
	INSTANCE(Concat);

	#if TYPE_CHECK
		if (type_of(this) isnt type_of(other))
		{
			String this_name = S(type_name(this));
			String other_name = S(type_name(other));
			throw(TYPE_EXCEPTION, "Cannot concatenate '#v' to '#v'", other_name, this_name);
		}
	#endif

	if (IMPLEMENTS(Concat, concat))
	{
		INS(Concat)->concat(this, other);
		return;
	}

	REPORT_METHOD(Concat, concat);
}

// Interface | Public | C_Types
Type TYPE(Concat) = new_type(Concat);