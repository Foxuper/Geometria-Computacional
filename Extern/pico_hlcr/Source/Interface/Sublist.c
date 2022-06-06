// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Get Sub List
var sublist (var this, var key_a, var key_b)
{
	#if TYPE_CHECK
		if (not is_key_type(this, key_a))
		{
			String key_name = S(type_name(key_a));
			String expected_type = S(type_name(key_type(this)));
			throw(KEY_EXCEPTION, "Key '#v' isn't of type '#v'", key_name, expected_type);
		}

		if (not is_key_type(this, key_b))
		{
			String key_name = S(type_name(key_b));
			String expected_type = S(type_name(key_type(this)));
			throw(KEY_EXCEPTION, "Key '#v' isn't of type '#v'", key_name, expected_type);
		}
	#endif

	INSTANCE(Sublist);

	if (IMPLEMENTS(Sublist, sublist))
		return INS(Sublist)->sublist(this, key_a, key_b);

	REPORT_METHOD(Sublist, sublist);
	return NULL;
}

// Interface | Public | Sublist
Type TYPE(Sublist) = new_type(Sublist);