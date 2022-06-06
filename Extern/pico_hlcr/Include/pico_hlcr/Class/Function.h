#ifndef PICO_HLCR_FUNCTION_H // Header | Function | START
#define PICO_HLCR_FUNCTION_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | Call
var Function_call_with (var this_ptr, var variable);

// Methods | Public | Cast
String Function_to_string (var this_ptr);

// Class | Public | Function
Type TYPE(Function) = new_type
(
	Function,
	implement(Call, Function_call_with),
	implement(Cast, NULL, Function_to_string)
);

#endif // Header | PICO_HLCR_FUNCTION_H | END