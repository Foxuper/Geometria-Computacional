#ifndef PICO_HLCR_FLOAT_H // Header | Float | START
#define PICO_HLCR_FLOAT_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | C_Types
int64  Float_c_int   (var this_ptr);
double Float_c_float (var this_ptr);

// Methods | Public | Compare
int64 Float_cmp (var this_ptr, var other_ptr);

// Methods | Public | Hash
uint64 Float_hash (var this_ptr);

// Methods | Public | Cast
var    Float_cast_to   (var this_ptr, Type type);
String Float_to_string (var this);

// Methods | Public | IO
int64 Float_show_to   (var this_ptr, var other, uint64 index);
int64 Float_look_from (var this_ptr, var other, uint64 index);

// Type | Public | Float
Type TYPE(Float) = new_type
(
	Float,
	implement(C_Types, Float_c_int, Float_c_float, NULL),
	implement(Compare, Float_cmp),
	implement(Hash, Float_hash),
	implement(Cast, Float_cast_to, Float_to_string),
	implement(IO, Float_show_to, Float_look_from)
);

#endif // Header | PICO_HLCR_FLOAT_H | END