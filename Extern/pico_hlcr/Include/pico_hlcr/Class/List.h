#ifndef PICO_HLCR_LIST_H // Header | List | START
#define PICO_HLCR_LIST_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void List_build_with (var this_ptr, Tuple args);
void List_destroy    (var this_ptr);

// Methods | Public | Assign
void List_assign (var this, var other);

// Methods | Public | Compare
int64 List_cmp (var this, var other);

// Methods | Public | Hash
uint64 List_hash (var this_ptr);

// Methods | Public | Len
uint64 List_len (var this_ptr);

// Methods | Public | Resize
void List_resize (var this_ptr, uint64 new_len);

// Methods | Public | Iter
var List_init (var this_ptr);
var List_next (var this, var current);

// Methods | Public | Cast
String List_to_string (var this);

// Methods | Public | Concat
void List_concat (var this, var other);

// Methods | Public | Sublist
var List_sublist (var this_ptr, var key_a, var key_b);

// Methods | Public | Access
var  List_first (var this_ptr);
var  List_last  (var this_ptr);
var  List_get   (var this_ptr, var key);
void List_set   (var this_ptr, var key, var value);

// Methods | Public | Content
bool List_contains (var this_ptr, var value);
var  List_index_of (var this_ptr, var value);
Type List_key_type (var this);

// Methods | Public | Insert
void List_insert    (var this_ptr, var value);
void List_erase     (var this_ptr, var value);
void List_insert_at (var this_ptr, var key, var value);
void List_erase_at  (var this_ptr, var key);

// Class | Public | Constants
Type NONE = empty_type(NONE);

// Class | Public | List
Type TYPE(List) = new_type
(
	List,
	implement(New, List_build_with, List_destroy),
	implement(Assign, List_assign, NULL, NULL),
	implement(Compare, List_cmp),
	implement(Hash, List_hash),
	implement(Len, List_len),
	implement(Resize, List_resize),
	implement(Iter, NULL, List_init, List_next, NULL),
	implement(Cast, NULL, List_to_string),
	implement(Concat, List_concat),
	implement(Sublist, List_sublist),
	implement(Access, List_first, List_last, List_get, List_set),
	implement(Content, List_contains, List_index_of, List_key_type, NULL),
	implement(Insert, List_insert, List_erase, List_insert_at, List_erase_at)
);

#endif // Header | PICO_HLCR_LIST_H | END