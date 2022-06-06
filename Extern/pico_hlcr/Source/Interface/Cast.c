// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Internal | Cast To Type
var cast_to (var this, Type type)
{
	if (type_of(this) is type)
		return copy(this);

	if (type is TYPE(String))
		return to_string(this);

	INSTANCE(Cast);

	if (IMPLEMENTS(Cast, cast_to))
		return INS(Cast)->cast_to(this, type);

	REPORT_METHOD(Cast, cast_to);
	return NULL;
}

// Method | Public | Internal | Cast to String
String to_string (var this)
{
	if (type_of(this) is TYPE(String))
		return copy(this);

	INSTANCE(Cast);

	if (IMPLEMENTS(Cast, to_string))
		return INS(Cast)->to_string(this);

	var string = new(String);
	String name = S(type_name(this));

	if (is_object(this))
	{
		with (String, location as new(String))
		{
			format_to(location, 0, "0x%X", this);
			print_to(string, 0, "<'#v' at #v>", name, location);
		}
	}
	else
		print_to(string, 0, "<Type '#v'>", name);

	return string;
}

// Interface | Public | Cast
Type TYPE(Cast) = new_type(Cast);