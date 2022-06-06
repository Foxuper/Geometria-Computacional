#ifndef PICO_HLCR_FILTER_H // Header | Filter | START
#define PICO_HLCR_FILTER_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Filter_build_with (var this_ptr, Tuple args);

// Methods | Public | Call
var Filter_call_with (var this_ptr, var variable);

// Methods | Public | Iter
var  Filter_init      (var this_ptr);
var  Filter_next      (var this_ptr, var current);
Type Filter_iter_type (var this_ptr);

// Methods | Public | Cast
String Filter_to_string (var this_ptr);

// Methods | Public | Content
bool Filter_contains (var this_ptr, var value);
var  Filter_index_of (var this_ptr, var value);

// Class | Public | Filter
Type TYPE(Filter) = new_type
(
	Filter,
	implement(New, Filter_build_with, NULL),
	implement(Call, Filter_call_with),
	implement(Iter, NULL, Filter_init, Filter_next, Filter_iter_type),
	implement(Cast, NULL, Filter_to_string),
	implement(Content, Filter_contains, Filter_index_of, NULL, NULL)
);

#endif // Header | PICO_HLCR_FILTER_H | END