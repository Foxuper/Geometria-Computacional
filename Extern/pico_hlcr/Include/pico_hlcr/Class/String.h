#ifndef PICO_HLCR_STRING_H // Header | String | START
#define PICO_HLCR_STRING_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void String_build_with (var this_ptr, Tuple args);
void String_destroy    (var this_ptr);

// Methods | Public | Assign
void String_assign (var this_ptr, var other);

// Methods | Public | C_Types
char *String_c_string (var this_ptr);

// Methods | Public | Compare
int64 String_cmp (var this_ptr, var other);

// Methods | Public | Hash
uint64 String_hash (var this_ptr);

// Methods | Public | Len
uint64 String_len (var this_ptr);

// Methods | Public | Resize
void String_resize (var this_ptr, uint64 new_len);

// Methods | Public | Cast
var String_cast_to (var this_ptr, Type type);

// Methods | Public | Concat
void String_concat (var this_ptr, var other);

// Methods | Public | Sublist
var String_sublist (var this_ptr, var key_a, var key_b);

// Methods | Public | Content
bool String_contains   (var this_ptr, var value);
var  String_index_of   (var this_ptr, var value);
Type String_key_type   (var this);
Type String_value_type (var this);

// Methods | Public | Insert
void String_insert    (var this, var value);
void String_erase     (var this_ptr, var value);
void String_insert_at (var this_ptr, var key, var value);
void String_erase_at  (var this_ptr, var key);

// Methods | Public | IO
int64 String_show_to   (var this_ptr, var other, uint64 index);
int64 String_look_from (var this_ptr, var other, uint64 index);

// Methods | Public | Format
int64 String_format_to   (var this_ptr, uint64 index, char *format, va_list args);
int64 String_format_from (var this_ptr, uint64 index, char *format, va_list args);

// Class | Public | String
Type TYPE(String) = new_type
(
	String,
	implement(New, String_build_with, String_destroy),
	implement(Assign, String_assign, NULL, NULL),
	implement(C_Types, NULL, NULL, String_c_string),
	implement(Compare, String_cmp),
	implement(Hash, String_hash),
	implement(Len, String_len),
	implement(Resize, String_resize),
	implement(Cast, String_cast_to, NULL),
	implement(Concat, String_concat),
	implement(Sublist, String_sublist),
	implement(Content, String_contains, String_index_of, String_key_type, String_value_type),
	implement(Insert, String_insert, String_erase, String_insert_at, String_erase_at),
	implement(IO, String_show_to, String_look_from),
	implement(Format, String_format_to, String_format_from)
);

#endif // Header | PICO_HLCR_STRING_H | END