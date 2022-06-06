#ifndef PICO_HLCR_PTR_H // Header | Ptr | START
#define PICO_HLCR_PTR_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | Assign
void Ptr_assign (var this_ptr, var other);

// Methods | Public | Compare
int64 Ptr_cmp (var this_ptr, var other);

// Methods | Public | Hash
uint64 Ptr_hash (var this_ptr);

// Method | Public | Cast
var    Ptr_cast_to   (var this_ptr, Type type);
String Ptr_to_string (var this_ptr);

// Methods | Public | Content
bool Ptr_contains (var this_ptr, var value);

// Methods | Public | Pointer
void Ptr_ref   (var this_ptr, var value);
var  Ptr_deref (var this_ptr);

// Class | Public | Ptr
Type TYPE(Ptr) = new_type
(
	Ptr,
	implement(Assign, Ptr_assign, NULL, NULL),
	implement(Compare, Ptr_cmp),
	implement(Hash, Ptr_hash),
	implement(Cast, Ptr_cast_to, Ptr_to_string),
	implement(Content, Ptr_contains, NULL, NULL, NULL),
	implement(Pointer, Ptr_ref, Ptr_deref)
);

#endif // Header | PICO_HLCR_PTR_H | END