#ifndef PICO_HLCR_MAP_H // Header | Map | START
#define PICO_HLCR_MAP_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Map_build_with (var this_ptr, Tuple args);

// Methods | Public | Len
uint64 Map_len (var this_ptr);

// Methods | Public | Call
var Map_call_with (var this_ptr, var variable);

// Methods | Public | Iter
var Map_init (var this_ptr);
var Map_next (var this_ptr, var current);

// Methods | Public | Cast
String Map_to_string (var this_ptr);

// Class | Public | Map
Type TYPE(Map) = new_type
(
	Map,
	implement(New, Map_build_with, NULL),
	implement(Len, Map_len),
	implement(Call, Map_call_with),
	implement(Iter, NULL, Map_init, Map_next, NULL),
	implement(Cast, NULL, Map_to_string)
);

#endif // Header | PICO_HLCR_MAP_H | END