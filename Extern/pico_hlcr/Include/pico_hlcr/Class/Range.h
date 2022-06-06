#ifndef PICO_HLCR_RANGE_H // Header | Range | START
#define PICO_HLCR_RANGE_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Range_build_with (var this_ptr, Tuple args);
void Range_destroy   (var this_ptr);

// Methods | Public | Compare
int64 Range_cmp (var this, var other);

// Methods | Public | Hash
uint64 Range_hash (var this);

// Methods | Public | Len
uint64 Range_len (var this_ptr);

// Methods | Public | Iter
var  Range_init      (var this_ptr);
var  Range_next      (var this_ptr, var current);
Type Range_iter_type (var this);

// Methods | Public | Cast
String Range_to_string (var this_ptr);

// Methods | Public | Access
var Range_first (var this_ptr);
var Range_last  (var this_ptr);
var Range_get   (var this_ptr, var key);

// Methods | Public | Content
bool Range_contains   (var this_ptr, var value);
var  Range_index_of   (var this_ptr, var value);
Type Range_key_type   (var this);
Type Range_value_type (var this);

// Class | Public | Range
Type TYPE(Range) = new_type
(
	Range,
	implement(New, Range_build_with, Range_destroy),
	implement(Compare, Range_cmp),
	implement(Hash, Range_hash),
	implement(Len, Range_len),
	implement(Iter, NULL, Range_init, Range_next, Range_iter_type),
	implement(Cast, NULL, Range_to_string),
	implement(Access, Range_first, Range_last, Range_get, NULL),
	implement(Content, Range_contains, Range_index_of, Range_key_type, Range_value_type)
);

#endif // Header | PICO_HLCR_RANGE_H | END