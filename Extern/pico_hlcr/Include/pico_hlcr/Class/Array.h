#ifndef PICO_HLCR_ARRAY_H // Header | Array | START
#define PICO_HLCR_ARRAY_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Array_build_with (var this_ptr, Tuple args);
void Array_destroy    (var this_ptr);

// Methods | Public | Assign
void Array_assign (var this_ptr, var other);

// Methods | Public | Compare
int64 Array_cmp (var this, var other);

// Methods | Public | Hash
uint64 Array_hash (var this_ptr);

// Methods | Public | Len
uint64 Array_len (var this_ptr);

// Methods | Public | Resize
void Array_resize (var this_ptr, uint64 new_len);

// Methods | Public | Iter
var  Array_init      (var this_ptr);
var  Array_next      (var this_ptr, var current);
Type Array_iter_type (var this_ptr);

// Methods | Public | Cast
String Array_to_string (var this);

// Methods | Public | Concat
void Array_concat (var this_ptr, var other);

// Methods | Public | Sublist
var Array_sublist (var this_ptr, var key_a, var key_b);

// Methods | Public | Access
var  Array_first (var this_ptr);
var  Array_last  (var this_ptr);
var  Array_get   (var this_ptr, var key);
void Array_set   (var this_ptr, var key, var value);

// Methods | Public | Content
bool Array_contains   (var this_ptr, var value);
var  Array_index_of   (var this_ptr, var value);
Type Array_key_type   (var this);
Type Array_value_type (var this_ptr);

// Methods | Public | Insert
void Array_insert    (var this_ptr, var value);
void Array_erase     (var this_ptr, var value);
void Array_insert_at (var this_ptr, var key, var value);
void Array_erase_at  (var this_ptr, var key);

// Class | Public | Array
Type TYPE(Array) = new_type
(
	Array,
	implement(New, Array_build_with, Array_destroy),
	implement(Assign, Array_assign, NULL, NULL),
	implement(Compare, Array_cmp),
	implement(Hash, Array_hash),
	implement(Len, Array_len),
	implement(Resize, Array_resize),
	implement(Iter, NULL, Array_init, Array_next, Array_iter_type),
	implement(Cast, NULL, Array_to_string),
	implement(Concat, Array_concat),
	implement(Sublist, Array_sublist),
	implement(Access, Array_first, Array_last, Array_get, Array_set),
	implement(Content, Array_contains, Array_index_of, Array_key_type, Array_value_type),
	implement(Insert, Array_insert, Array_erase, Array_insert_at, Array_erase_at)
);

#endif // Header | PICO_HLCR_ARRAY_H | END