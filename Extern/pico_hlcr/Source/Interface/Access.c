// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Get First Value
var first (var this)
{
	INSTANCE(Access);

	if (IMPLEMENTS(Access, first))
		return INS(Access)->first(this);

	REPORT_METHOD(Access, first);
	return NULL;
}

// Method | Public | Get Last Value
var last (var this)
{
	INSTANCE(Access);

	if (IMPLEMENTS(Access, last))
		return INS(Access)->last(this);

	REPORT_METHOD(Access, last);
	return NULL;
}

// Method | Public | Get Value At Key Location
var get (var this, var key)
{
	#if TYPE_CHECK
		if (not is_key_type(this, key))
		{
			String key_name = S(type_name(key));
			String expected_type = S(type_name(key_type(this)));
			return throw(TYPE_EXCEPTION, "Key '#v' isnt of type '#v'", key_name, expected_type);
		}
	#endif

	INSTANCE(Access);

	if (IMPLEMENTS(Access, get))
		return INS(Access)->get(this, key);

	REPORT_METHOD(Access, get);
	return NULL;
}

// Method | Public | Set Value At Key Location
void set (var this, var key, var value)
{
	#if TYPE_CHECK
		if (not is_key_type(this, key))
		{
			String key_name = S(type_name(key));
			String expected_type = S(type_name(key_type(this)));
			throw(TYPE_EXCEPTION, "Key '#v' isnt of type '#v'", key_name, expected_type);
		}

		if (not is_value_type(this, value))
		{
			String value_name = S(type_name(value));
			String expected_type = S(type_name(value_type(this)));
			throw(TYPE_EXCEPTION, "Value '#v' isnt of type '#v'", value_name, expected_type);
		}
	#endif

	INSTANCE(Access);

	if (IMPLEMENTS(Access, set))
	{
		INS(Access)->set(this, key, value);
		return;
	}

	REPORT_METHOD(Access, set);
}

// Interface | Public | Access
Type TYPE(Access) = new_type(Access);