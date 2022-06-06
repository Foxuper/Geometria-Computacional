#ifndef PICO_HLCR_SLICE_H // Header | Slice | START
#define PICO_HLCR_SLICE_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Slice_build_with (var this_ptr, Tuple args);
void Slice_destroy    (var this_ptr);

// Methods | Public | Compare
int64 Slice_cmp (var this, var other);

// Methods | Public | Hash
uint64 Slice_hash (var this);

// Methods | Public | Len
uint64 Slice_len (var this_ptr);

// Methods | Public | Iter
var  Slice_init      (var this_ptr);
var  Slice_next      (var this_ptr, var current);
Type Slice_iter_type (var this_ptr);

// Methods | Public | Cast
String Slice_to_string (var this);

// Methods | Public | Access
var  Slice_first (var this_ptr);
var  Slice_last  (var this_ptr);
var  Slice_get   (var this_ptr, var key);
void Slice_set   (var this_ptr, var key, var value);

// Methods | Public | Content
bool Slice_contains   (var this, var value);
var  Slice_index_of   (var this, var value);
Type Slice_key_type   (var this);
Type Slice_value_type (var this_ptr);

// Class | Public | Slice
Type TYPE(Slice) = new_type
(
	Slice,
	implement(New, Slice_build_with, Slice_destroy),
	implement(Compare, Slice_cmp),
	implement(Hash, Slice_hash),
	implement(Len, Slice_len),
	implement(Iter, NULL, Slice_init, Slice_next, Slice_iter_type),
	implement(Cast, NULL, Slice_to_string),
	implement(Access, Slice_first, Slice_last, Slice_get, Slice_set),
	implement(Content, Slice_contains, Slice_index_of, Slice_key_type, Slice_value_type)
);

#endif // Header | PICO_HLCR_SLICE_H | END