// Include | Header
#include <pico_hlcr/Class/List.h>

// Structure | Private | Node Header
typedef struct Node_Header
{
	var *next;
	var *prev;
}
*Node_Header;

// Functions | Private | List
static var  node_new    (Type type);
static void node_delete (var node);
static var  node_header (var node);
static var  node_next   (var node);
static var  node_prev   (var node);
static void node_link   (var node, var next, var prev);
static var  list_at     (List this, uint64 position);
static void list_link   (List this, var node, var next, var prev);
static void list_unlink (List this, var node);

// Function | Private | New Node
static var node_new (Type type)
{
	uint64 node_size = sizeof(struct Node_Header) + HEADER_SIZE + size(type);
	Node_Header header = calloc(1, node_size);

	header->next = NULL;
	header->prev = NULL;

	var memory = memory_offset(header, sizeof(struct Node_Header));
	var node = header_init(memory, type, ALLOC_DATA);
	return build(node);
}

// Function | Private | Delete Node
static void node_delete (var node)
{
	destroy(node);
	free(node_header(node));
}

// Function | Private | Get Node Header
static var node_header (var node)
{
	Header header = memory_offset(node, -HEADER_SIZE);
	return memory_offset(header, -sizeof(struct Node_Header));
}

// Function | Private | Get Next Node
static var node_next (var node)
{
	Node_Header header = node_header(node);
	return header->next;
}

// Function | Private | Get Previous Node
static var node_prev (var node)
{
	Node_Header header = node_header(node);
	return header->prev;
}

// Function | Private | Link Node
static void node_link (var node, var next, var prev)
{
	Node_Header header = node_header(node);
	header->next = next;
	header->prev = prev;
}

// Function | Private | Get Node at Position
static var list_at (List this, uint64 position)
{
	var current = this->head;

	for (uint64 index = 0; index < position; index++)
		current = node_next(current);

	return current;
}

// Function | Private | Link List
static void list_link (List this, var node, var next, var prev)
{
	node_link(node, next, prev);

	if (next)
		node_link(next, node_next(next), node);
	else
		this->tail = node;

	if (prev)
		node_link(prev, node, node_prev(prev));
	else
		this->head = node;
}

// Function | Private | Unlink List
static void list_unlink (List this, var node)
{
	var next = node_next(node);
	var prev = node_prev(node);

	if (this->head is this->tail)
	{
		this->head = NULL;
		this->tail = NULL;
	}
	else if (this->head is node)
	{
		this->head = next;
		node_link(next, node_next(next), NULL);
	}
	else if (this->tail is node)
	{
		this->tail = prev;
		node_link(prev, NULL, node_prev(prev));
	}
	else
	{
		node_link(prev, next, node_prev(prev));
		node_link(next, node_next(next), prev);
	}
}

// Method | Public | New -> build_with
void List_build_with (var this_ptr, Tuple args)
{
	THIS(List);
	this->head = NULL;
	this->tail = NULL;
	this->len = 0;

	foreach (var, value in args)
		List_insert(this, value);
}

// Method | Public | New -> destroy
void List_destroy (var this_ptr)
{
	THIS(List);
	var current = this->head;

	while (current)
	{
		var next = node_next(current);
		node_delete(current);
		current = next;
	}

	this->head = NULL;
	this->tail = NULL;
	this->len = 0;
}

// Method | Public | Assign -> assign
void List_assign (var this, var other)
{
	List_destroy(this);

	foreach (var, value in other)
		List_insert(this, value);
}

// Method | Public | Compare -> cmp
int64 List_cmp (var this, var other)
{
	var a = List_init(this);
	var b = init(other);

	while (true)
	{
		if (a is ITER_STOP and b is ITER_STOP) return 0;
		if (a is ITER_STOP) return -1;
		if (b is ITER_STOP) return 1;

		int64 difference = cmp(a, b);

		if (difference is 0)
		{
			a = List_next(this, a);
			b = next(other, b);
		}
		else
			return difference;
	}
}

// Method | Public | Hash -> hash
uint64 List_hash (var this_ptr)
{
	THIS(List);
	uint64 h = 0;
	var value = this->head;

	while (value)
	{
		h ^= hash(value);
		value = node_next(value);
	}

	return h;
}

// Method | Public | Len -> len
uint64 List_len (var this_ptr)
{
	THIS(List);
	return this->len;
}

// Method | Public | Resize -> resize
void List_resize (var this_ptr, uint64 new_len)
{
	THIS(List);
	uint64 length = this->len;

	for (uint64 index = length; index < new_len; index++)
		List_insert(this, NONE);

	for (uint64 index = new_len; index < length; index++)
		List_erase_at(this, I(this->len - 1));
}

// Method | Public | Iter -> init
var List_init (var this_ptr)
{
	THIS(List);
	return this->head ? this->head : (var) ITER_STOP;
}

// Method | Public | Iter -> next
var List_next (var this, var current)
{
	UNUSED(this);
	var next = node_next(current);
	return next ? next : ITER_STOP;
}

// Method | Public | Cast -> to_string
String List_to_string (var this)
{
	String casted = new(String);
	var last = List_last(this);
	concat(casted, S("["));

	foreach (var, value in this)
	{
		with (String, string as str(value))
			concat(casted, string);

		if (value isnt last)
			concat(casted, S(", "));
	}

	concat(casted, S("]"));
	return casted;
}

// Method | Public | Concat -> concat
void List_concat (var this, var other)
{
	foreach (var, value in other)
		List_insert(this, value);
}

// Method | Public | Sublist -> sublist
var List_sublist (var this_ptr, var key_a, var key_b)
{
	THIS(Array);
	uint64 index_a = c_int(key_a);
	uint64 index_b = c_int(key_b);

	if (index_a <= index_b and index_b <= this->len)
	{
		List sub_list = new(List);

		foreach (Int, index in range(key_a, key_b))
		{
			var value = List_get(this, index);
			List_insert(sub_list, value);
		}

		return sub_list;
	}

	return NULL;
}

// Method | Public | Access -> first
var List_first (var this_ptr)
{
	THIS(List);
	return this->head;
}

// Method | Public | Access -> last
var List_last (var this_ptr)
{
	THIS(List);
	return this->tail;
}

// Method | Public | Access -> get
var List_get (var this_ptr, var key)
{
	THIS(List);
	uint64 index = c_int(key);
	return (index < this->len) ? list_at(this, index) : NULL;
}

// Method | Public | Access -> set
void List_set (var this_ptr, var key, var value)
{
	THIS(List);
	uint64 index = c_int(key);

	if (index >= this->len)
		return;

	Type type = type_of(value);
	var current = list_at(this, index);

	if (type isnt type_of(current))
	{
		var node = node_new(type);
		list_link(this, node, node_next(current), node_prev(current));
		node_delete(current);
		current = node;
	}

	assign(current, value);
}

// Method | Public | Content -> contains
bool List_contains (var this_ptr, var value)
{
	THIS(List);
	var current = this->head;

	while (current)
	{
		if (eq(current, value))
			return true;

		current = node_next(current);
	}

	return false;
}

// Method | Public | Content -> index_of
var List_index_of (var this_ptr, var value)
{
	THIS(List);
	var current = this->head;

	for (uint64 index = 0; index < this->len; index++)
	{
		if (eq(current, value))
			return new(Int, I(index));

		current = node_next(current);
	}

	return NULL;
}

// Method | Public | Content -> key_type
Type List_key_type (var this)
{
	UNUSED(this);
	return TYPE(Int);
}

// Method | Public | Insert -> insert
void List_insert (var this_ptr, var value)
{
	THIS(List);
	List_insert_at(this, I(this->len), value);
}

// Method | Public | Insert -> erase
void List_erase (var this_ptr, var value)
{
	THIS(List);
	var current = this->head;

	while (current)
	{
		if (eq(current, value))
		{
			list_unlink(this, current);
			node_delete(current);
			this->len--;
			return;
		}

		current = node_next(current);
	}
}

// Method | Public | Insert -> insert_at
void List_insert_at (var this_ptr, var key, var value)
{
	THIS(List);
	uint64 index = c_int(key);
	value = value ? value : NONE;

	if (index <= this->len)
	{
		var node = node_new(type_of(value));
		assign(node, value);
		this->len++;

		if (index > 0)
		{
			var prev = list_at(this, index - 1);
			list_link(this, node, node_next(prev), prev);
		}
		else
			list_link(this, node, this->head, NULL);
	}
}

// Method | Public | Insert -> erase_at
void List_erase_at (var this_ptr, var key)
{
	THIS(List);
	uint64 index = c_int(key);

	if (index < this->len)
	{
		var current = list_at(this, index);
		list_unlink(this, current);
		node_delete(current);
		this->len--;
	}
}