// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Functions | Private | Assign
static void memswap (var a, var b, uint64 size);

// Method | Public | Assign Value
var assign (var this, var other)
{
	if (other is NULL)
		return this;

	INSTANCE(Assign);

	if (IMPLEMENTS(Assign, assign))
	{
		INS(Assign)->assign(this, other);
		return this;
	}

	if (type_of(this) is type_of(other))
		return memcpy(this, other, size(this));

	#if METHOD_CHECK
		String this_name = S(type_name(this));
		String other_name = S(type_name(other));
		throw(METHOD_EXCEPTION, "Couln't assign type '#v' from type '#v'", this_name, other_name);
	#endif

	return NULL;
}

// Method | Public | Swap Values
void swap (var this, var other)
{
	INSTANCE(Assign);

	if (IMPLEMENTS(Assign, swap))
	{
		INS(Assign)->swap(this, other);
		return;
	}

	if (type_of(this) is type_of(other))
		memswap(this, other, size(this));

	#if METHOD_CHECK
		String this_name = S(type_name(this));
		String other_name = S(type_name(this));
		throw(METHOD_EXCEPTION, "Couln't swap type '#v' with type '#v'", this_name, other_name);
	#endif
}

// Method | Public | Get Copy
var copy (var this)
{
	INSTANCE(Assign);

	if (IMPLEMENTS(Assign, copy))
		return INS(Assign)->copy(this);

	var memory = alloc(type_of(this));
	return assign(build(memory), this);
}

// Function | Private | Swap Memory
static void memswap (var a, var b, uint64 size)
{
	uint8 aux[size];
	memcpy(aux, a, size);
	memcpy(a, b, size);
	memcpy(b, aux, size);
}

// Interface | Public | Assign
Type TYPE(Assign) = new_type(Assign);