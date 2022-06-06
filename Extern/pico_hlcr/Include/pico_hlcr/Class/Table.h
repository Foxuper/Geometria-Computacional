#ifndef PICO_HLCR_TABLE_H // Header | Table | START
#define PICO_HLCR_TABLE_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Table_build_with (var this_ptr, Tuple args);
void Table_destroy    (var this_ptr);

// Methods | Public | Assign
void Table_assign (var this_ptr, var other);

// Methods | Public | Compare
int64 Table_cmp (var this, var other);

// Methods | Public | Hash
uint64 Table_hash (var this_ptr);

// Methods | Public | Len
uint64 Table_len (var this_ptr);

// Methods | Public | Iter
var  Table_init      (var this_ptr);
var  Table_next      (var this_ptr, var current);
Type Table_iter_type (var this_ptr);

// Methods | Public | Cast
String Table_to_string (var this);

// Methods | Public | Concat
void Table_concat (var this_ptr, var other);

// Methods | Public | Access
var  Table_first (var this_ptr);
var  Table_last  (var this_ptr);
var  Table_get   (var this, var key);
void Table_set   (var this, var key, var value);

// Methods | Public | Content
bool Table_contains   (var this_ptr, var key);
var  Table_index_of   (var this_ptr, var value);
Type Table_key_type   (var this_ptr);
Type Table_value_type (var this_ptr);

// Methods | Public | Insert -> insert_at
void Table_erase     (var this_ptr, var value);
void Table_insert_at (var this_ptr, var key, var value);
void Table_erase_at  (var this_ptr, var key);

// Class | Public | Table
Type TYPE(Table) = new_type
(
	Table,
	implement(New, Table_build_with, Table_destroy),
	implement(Assign, Table_assign, NULL, NULL),
	implement(Compare, Table_cmp),
	implement(Hash, Table_hash),
	implement(Len, Table_len),
	implement(Iter, NULL, Table_init, Table_next, Table_iter_type),
	implement(Cast, NULL, Table_to_string),
	implement(Concat, Table_concat),
	implement(Access, Table_first, Table_last, Table_get, Table_set),
	implement(Content, Table_contains, Table_index_of, Table_key_type, Table_value_type),
	implement(Insert, NULL, Table_erase, Table_insert_at, Table_erase_at)
);

#endif // Header | PICO_HLCR_TABLE_H | END