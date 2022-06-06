#ifndef PICO_HLCR_INT_H // Header | Int | START
#define PICO_HLCR_INT_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | C_Types
int64  Int_c_int   (var this_ptr);
double Int_c_float (var this_ptr);

// Methods | Public | Compare
int64 Int_cmp (var this_ptr, var other_ptr);

// Methods | Public | Hash
uint64 Int_hash (var this_ptr);

// Methods | Public | Cast
var    Int_cast_to   (var this_ptr, Type type);
String Int_to_string (var this);

// Methods | Public | IO
int64 Int_show_to   (var this_ptr, var other, uint64 index);
int64 Int_look_from (var this_ptr, var other, uint64 index);

// Type | Public | Int
Type TYPE(Int) = new_type
(
	Int,
	implement(C_Types, Int_c_int, Int_c_float, NULL),
	implement(Compare, Int_cmp),
	implement(Hash, Int_hash),
	implement(Cast, Int_cast_to, Int_to_string),
	implement(IO, Int_show_to, Int_look_from)
);

#endif // Header | PICO_HLCR_INT_H | END