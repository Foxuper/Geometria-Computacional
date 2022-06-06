#ifndef PICO_HLCR_TUPLE_H // Header | Tuple | START
#define PICO_HLCR_TUPLE_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Tuple_build_with (var this_ptr, Tuple args);
void Tuple_destroy    (var this_ptr);

// Methods | Public | Assign
void Tuple_assign (var this_ptr, var other);

// Methods | Public | Compare
int64 Tuple_cmp (var this, var other);

// Methods | Public | Hash
uint64 Tuple_hash (var this_ptr);

// Methods | Public | Len
uint64 Tuple_len (var this_ptr);

// Methods | Public | Resize
void Tuple_resize (var this_ptr, uint64 new_len);

// Methods | Public | Iter
var Tuple_init (var this_ptr);
var Tuple_next (var this_ptr, var current);

// Methods | Public | Cast
String Tuple_to_string (var this);

// Methods | Public | Concat
void Tuple_concat (var this_ptr, var other);

// Methods | Public | Sublist
var Tuple_sublist (var this_ptr, var key_a, var key_b);

// Methods | Public | Access
var  Tuple_first (var this_ptr);
var  Tuple_last  (var this_ptr);
var  Tuple_get   (var this_ptr, var key);
void Tuple_set   (var this_ptr, var key, var value);

// Methods | Public | Content
bool Tuple_contains (var this_ptr, var value);
var  Tuple_index_of (var this_ptr, var value);
Type Tuple_key_type (var this);

// Methods | Public | Insert
void Tuple_insert    (var this_ptr, var value);
void Tuple_erase     (var this_ptr, var value);
void Tuple_insert_at (var this_ptr, var key, var value);
void Tuple_erase_at  (var this_ptr, var key);

// Class | Public | Tuple
Type TYPE(Tuple) = new_type
(
	Tuple,
	implement(New, Tuple_build_with, Tuple_destroy),
	implement(Assign, Tuple_assign, NULL, NULL),
	implement(Compare, Tuple_cmp),
	implement(Hash, Tuple_hash),
	implement(Len, Tuple_len),
	implement(Resize, Tuple_resize),
	implement(Iter, NULL, Tuple_init, Tuple_next, NULL),
	implement(Cast, NULL, Tuple_to_string),
	implement(Concat, Tuple_concat),
	implement(Sublist, Tuple_sublist),
	implement(Access, Tuple_first, Tuple_last, Tuple_get, Tuple_set),
	implement(Content, Tuple_contains, Tuple_index_of, Tuple_key_type, NULL),
	implement(Insert, Tuple_insert, Tuple_erase, Tuple_insert_at, Tuple_erase_at)
);

#endif // Header | PICO_HLCR_TUPLE_H | END