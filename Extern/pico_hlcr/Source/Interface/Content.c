// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Contains Value
bool contains (var this, var value)
{
	INSTANCE(Content);

	if (IMPLEMENTS(Content, contains))
		return INS(Content)->contains(this, value);

	REPORT_METHOD(Content, contains);
	return false;
}

// Method | Public | Index Of Value
var index_of (var this, var value)
{
	INSTANCE(Content);

	if (IMPLEMENTS(Content, index_of))
		return INS(Content)->index_of(this, value);

	REPORT_METHOD(Content, index_of);
	return NULL;
}

// Method | Public | Key Type
Type key_type (var this)
{
	INSTANCE(Content);

	if (IMPLEMENTS(Content, key_type))
		return INS(Content)->key_type(this);

	return VAR;
}

// Method | Public | Value Type
Type value_type (var this)
{
	INSTANCE(Content);

	if (IMPLEMENTS(Content, value_type))
		return INS(Content)->value_type(this);

	return VAR;
}

// Method | Public | Is Key Type
bool is_key_type (var this, var key)
{
	Type type = key_type(this);
	if (is_pointer(type)) return true;
	return (type and type_of(key) is type) ? true : false;
}

// Method | Public | Is Value Type
bool is_value_type (var this, var value)
{
	Type type = value_type(this);
	if (is_pointer(type)) return true;
	return (type and type_of(value) is type) ? true : false;
}

// Interface | Public | Content
Type TYPE(Content) = new_type(Content);