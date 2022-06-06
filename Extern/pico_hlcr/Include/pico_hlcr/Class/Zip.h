#ifndef PICO_HLCR_ZIP_H // Header | Zip | START
#define PICO_HLCR_ZIP_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Zip_build_with (var this_ptr, Tuple args);
void Zip_destroy    (var this_ptr);

// Methods | Public | Len
uint64 Zip_len (var this_ptr);

// Methods | Public | Iter
var Zip_init (var this_ptr);
var Zip_next (var this_ptr, var current);

// Methods | Public | Content
bool Zip_contains   (var this, var value);
var  Zip_index_of   (var this_ptr, var value);
Type Zip_key_type   (var this);
Type Zip_value_type (var this);

// Methods | Public | Access
var  Zip_first (var this);
var  Zip_last  (var this);
var  Zip_get   (var this_ptr, var key);

// Methods | Public | Cast
String Zip_to_string (var this_ptr);

// Class | Public | Zip
Type TYPE(Zip) = new_type
(
	Zip,
	implement(New, Zip_build_with, Zip_destroy),
	implement(Len, Zip_len),
	implement(Iter, NULL, Zip_init, Zip_next, NULL),
	implement(Access, Zip_first, Zip_last, Zip_get, NULL),
	implement(Content, Zip_contains, Zip_index_of, Zip_key_type, Zip_value_type),
	implement(Cast, NULL, Zip_to_string)
);

#endif // Header | PICO_HLCR_ZIP_H | END