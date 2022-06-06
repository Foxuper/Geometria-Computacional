// Include | Header
#include <pico_hlcr/Class/Function.h>

// Method | Public | Call -> call_with
var Function_call_with (var this_ptr, var variable)
{
	THIS(Function);
	return this->function ? this->function(variable) : NULL;
}

// Method | Public | Cast -> to_string
String Function_to_string (var this_ptr)
{
	THIS(Function);
	String casted = new(String);

	with (String, location as new(String))
	{
		format_to(location, 0, "0x%X", this->function);
		print_to(casted, 0, "function(#v)", location);
	}
	
	return casted;
}