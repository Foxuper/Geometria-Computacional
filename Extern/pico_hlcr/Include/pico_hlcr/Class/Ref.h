#ifndef PICO_HLCR_REF_H // Header | Ref | START
#define PICO_HLCR_REF_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Ref_destroy (var this_ptr);

// Methods | Public | Assign
void Ref_assign (var this_ptr, var other);
var  Ref_copy   (var this_ptr);

// Methods | Public | Compare
int64 Ref_cmp (var this_ptr, var other);

// Methods | Public | Hash
uint64 Ref_hash (var this_ptr);

// Methods | Public | Context
void Ref_enter (var this_ptr);
void Ref_leave (var this_ptr);

// Methods | Public | Cast
var    Ref_cast_to   (var this_ptr, Type type);
String Ref_to_string (var this_ptr);

// Methods | Public | Content
bool Ref_contains (var this_ptr, var value);

// Methods | Public | Pointer
void Ref_ref   (var this_ptr, var reference);
var  Ref_deref (var this_ptr);

// Class | Public | Ref
Type TYPE(Ref) = new_type
(
	Ref,
	implement(New, NULL, Ref_destroy),
	implement(Assign, Ref_assign, NULL, Ref_copy),
	implement(Compare, Ref_cmp),
	implement(Hash, Ref_hash),
	implement(Context, Ref_enter, Ref_leave),
	implement(Cast, Ref_cast_to, Ref_to_string),
	implement(Content, Ref_contains, NULL, NULL, NULL),
	implement(Pointer, Ref_ref, Ref_deref)
);

#endif // Header | PICO_HLCR_REF_H | END