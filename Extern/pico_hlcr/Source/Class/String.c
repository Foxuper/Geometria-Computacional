// Include | Header
#include <pico_hlcr/Class/String.h>

// Core Includes
#include <inttypes.h>

// Method | Public | New -> build_with
void String_build_with (var this_ptr, Tuple args)
{
	THIS(String);
	this->string = malloc(1);
	this->string[0] = CHR_NULL;

	if (not empty(args))
		String_assign(this, first(args));
}

// Method | Public | New -> destroy
void String_destroy (var this_ptr)
{
	THIS(String);
	free(this->string);
}

// Method | Public | Assign -> assign
void String_assign (var this_ptr, var other)
{
	THIS(String);
	char *string = c_string(other);
	String_resize(this, len(other));
	strcpy(this->string, string ? string : "");
}

// Method | Public | C_Types -> c_string
char *String_c_string (var this_ptr)
{
	THIS(String);
	return this->string;
}

// Method | Public | Compare -> cmp
int64 String_cmp (var this_ptr, var other)
{
	THIS(String);
	if (type_of(other) isnt TYPE(String))
		return (intptr) this_ptr - (intptr) other;
	return strcmp(this->string, c_string(other));
}

// Method | Public | Hash -> hash
uint64 String_hash (var this_ptr)
{
	THIS(String);
	return hash_data(this->string, strlen(this->string));
}

// Method | Public | Len -> len
uint64 String_len (var this_ptr)
{
	THIS(String);
	return this->string ? strlen(this->string) : 0;
}

// Method | Public | Resize -> resize
void String_resize (var this_ptr, uint64 new_len)
{
	THIS(String);
	uint64 length = this->string ? strlen(this->string) : 0;

	if (new_len > length)
	{
		this->string = realloc(this->string, new_len + 1);
		memset(this->string + length, CHR_SPACE, new_len - length);
		this->string[new_len] = CHR_NULL;
	}

	if (new_len <= length)
	{
		this->string = realloc(this->string, new_len + 1);
		this->string[new_len] = CHR_NULL;
	}
}

// Method | Public | Cast -> cast_to
var String_cast_to (var this_ptr, Type type)
{
	THIS(String);

	if (type is TYPE(Int))
	{
		Int casted = new(Int);
		if (sscanf(this->string, "%" PRId64, &casted->value))
			return casted;
		delete(casted);
	}

	if (type is TYPE(Float))
	{
		Float casted = new(Float);
		if (sscanf(this->string, "%lg", &casted->value))
			return casted;
		delete(casted);
	}

	return NULL;
}

// Method | Public | Concat -> concat
void String_concat (var this_ptr, var other)
{
	THIS(String);
	uint64 length = strlen(this->string);
	String_resize(this, len(other) + length);
	strcpy(this->string + length, c_string(other));
}

// Method | Public | Sublist -> sublist
var String_sublist (var this_ptr, var key_a, var key_b)
{
	THIS(String);
	uint64 index_a = c_int(key_a);
	uint64 index_b = c_int(key_b);
	uint64 length = strlen(this->string);

	if (index_a <= index_b and index_b <= length)
	{
		uint64 current = 0;
		String sub_list = new(String);
		String_resize(sub_list, index_b - index_a);

		for (uint64 index = index_a; index < index_b; index++)
			sub_list->string[current++] = this->string[index];

		return sub_list;
	}

	return NULL;
}

// Method | Public | Content -> contains
bool String_contains (var this_ptr, var value)
{
	THIS(String);
	char *string = c_string(value);
	uint64 value_len = strlen(string);
	uint64 length = strlen(this->string);

	if (not string or value_len > length)
		return false;

	for (uint64 index = 0; index < length; index++)
	{
		if (memcmp(this->string + index, string, value_len) is 0)
			return true;
	}

	return false;
}

// Method | Public | Content -> index_of
var String_index_of (var this_ptr, var value)
{
	THIS(String);
	char *string = c_string(value);
	uint64 value_len = strlen(string);
	uint64 length = strlen(this->string);

	for (uint64 index = 0; index < length; index++)
	{
		if (memcmp(this->string + index, string, value_len) is 0)
			return new(Int, I(index));
	}

	return NULL;
}

// Method | Public | Content -> key_type
Type String_key_type (var this)
{
	UNUSED(this);
	return TYPE(Int);
}

// Method | Public | Content -> value_type
Type String_value_type (var this)
{
	UNUSED(this);
	return TYPE(String);
}

// Method | Public | Insert -> insert_at
void String_insert (var this, var value)
{
	String_concat(this, value);
}

// Method | Public | Insert -> erase
void String_erase (var this_ptr, var value)
{
	THIS(String);

	with (Int, key as String_index_of(this, value))
	{
		if (not key) return;
		uint64 index = c_int(key);
		uint64 length = strlen(this->string);
		uint64 value_len = String_len(value);
		uint64 bytes = length - (value_len + index);
		memmove(this->string + index, this->string + index + value_len, bytes);
		String_resize(this, length - value_len);
	}
}

// Method | Public | Insert -> insert_at
void String_insert_at (var this_ptr, var key, var value)
{
	THIS(String);
	uint64 index = c_int(key);
	uint64 length = strlen(this->string);

	if (index <= length)
	{
		String remainder = String_sublist(this, I(index), I(length));
		String_resize(this, index);
		String_concat(this, value);
		String_concat(this, remainder);
		delete(remainder);
	}
}

// Method | Public | Insert -> erase_at
void String_erase_at (var this_ptr, var key)
{
	THIS(String);
	uint64 index = c_int(key);
	uint64 length = strlen(this->string);

	if (index < length)
	{
		String remainder = String_sublist(this, I(index + 1), I(length));
		String_resize(this, index);
		String_concat(this, remainder);
		delete(remainder);
	}
}

// Method | Public | IO -> show_to
int64 String_show_to (var this_ptr, var other, uint64 index)
{
	THIS(String);
	return format_to(other, index, this->string);
}

// Method | Public | IO -> look_from
int64 String_look_from (var this_ptr, var other, uint64 index)
{
	THIS(String);
	int64 scanned = 0;
	String_resize(this, len(other) + index);
	int64 error = format_from(other, index, "%s%n", this->string, &scanned);
	String_resize(this, scanned);
	return error < 0 ? error : scanned;
}

// Method | Public | Format -> format_to
int64 String_format_to (var this_ptr, uint64 index, char *format, va_list args)
{
	THIS(String);
	uint64 length = this->string ? strlen(this->string) : 0;
	uint64 new_len = vsnprintf(NULL, 0, format, args) + index;
	String_resize(this, (new_len > length) ? new_len : length);
	return vsprintf(this->string + index, format, args);
}

// Method | Public | Format -> format_from
int64 String_format_from (var this_ptr, uint64 index, char *format, va_list args)
{
	THIS(String);
	return vsscanf(this->string + index, format, args);
}