// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Insert Value
void insert (var this, var value)
{
	#if TYPE_CHECK
		if (not is_value_type(this, value))
		{
			String value_name = S(type_name(value));
			String expected_type = S(type_name(value_type(this)));
			throw(TYPE_EXCEPTION, "Value '#v' isnt of type '#v'", value_name, expected_type);
		}
	#endif

	if (not alloc_at_heap(this))
	{
		String this_name = S(type_name(this));
		throw(RESOURCE_EXCEPTION, "Attempt to insert into non heap variable '#v'", this_name);
	}

	INSTANCE(Insert);

	if (IMPLEMENTS(Insert, insert))
	{
		INS(Insert)->insert(this, value);
		return;
	}

	REPORT_METHOD(Insert, insert);
}

// Method | Public | Erase Value
void erase (var this, var value)
{
	#if TYPE_CHECK
		if (not is_value_type(this, value))
		{
			String value_name = S(type_name(value));
			String expected_type = S(type_name(value_type(this)));
			throw(TYPE_EXCEPTION, "Value '#v' isnt of type '#v'", value_name, expected_type);
		}
	#endif

	if (not alloc_at_heap(this))
	{
		String this_name = S(type_name(this));
		throw(RESOURCE_EXCEPTION, "Attempt to erase from non heap variable '#v'", this_name);
	}

	INSTANCE(Insert);

	if (IMPLEMENTS(Insert, erase))
	{
		INS(Insert)->erase(this, value);
		return;
	}

	REPORT_METHOD(Insert, erase);
}

// Method | Public | Insert Value At Key Location
void insert_at (var this, var key, var value)
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

	if (not alloc_at_heap(this))
	{
		String this_name = S(type_name(this));
		throw(RESOURCE_EXCEPTION, "Attempt to insert into non heap variable '#v'", this_name);
	}

	INSTANCE(Insert);

	if (IMPLEMENTS(Insert, insert_at))
	{
		INS(Insert)->insert_at(this, key, value);
		return;
	}

	REPORT_METHOD(Insert, insert_at);
}

// Method | Public | Erase Value At Key Location
void erase_at (var this, var key)
{
	#if TYPE_CHECK
		if (not is_key_type(this, key))
		{
			String key_name = S(type_name(key));
			String expected_type = S(type_name(key_type(this)));
			throw(TYPE_EXCEPTION, "Key '#v' isnt of type '#v'", key_name, expected_type);
		}
	#endif

	if (not alloc_at_heap(this))
	{
		String this_name = S(type_name(this));
		throw(RESOURCE_EXCEPTION, "Attempt to erase from non heap variable'#v'", this_name);
	}

	INSTANCE(Insert);

	if (IMPLEMENTS(Insert, erase_at))
	{
		INS(Insert)->erase_at(this, key);
		return;
	}

	REPORT_METHOD(Insert, erase_at);
}

// Interface | Public | Insert
Type TYPE(Insert) = new_type(Insert);