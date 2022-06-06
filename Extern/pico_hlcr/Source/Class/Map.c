// Include | Header
#include <pico_hlcr/Class/Map.h>

// Method | Public | New -> build_with
void Map_build_with (var this_ptr, Tuple args)
{
	THIS(Map);
	this->function = get(args, I(0));
	this->iterable = get(args, I(1));
}

// Method | Public | Len -> len
uint64 Map_len (var this_ptr)
{
	THIS(Map);
	return len(this->iterable);
}

// Method | Public | Call -> call_with
var Map_call_with (var this_ptr, var variable)
{
	THIS(Map);
	UNUSED(variable);
	foreach (var, value in this);
	return this->iterable;
}

// Method | Public | Iter -> init
var Map_init (var this_ptr)
{
	THIS(Map);
	this->current = init(this->iterable);

	if (this->current isnt ITER_STOP)
		return call_with(this->function, this->current);

	return ITER_STOP;
}

// Method | Public | Iter -> next
var Map_next (var this_ptr, var current)
{
	THIS(Map);
	this->current = next(this->iterable, current);

	if (this->current isnt ITER_STOP)
		return call_with(this->function, this->current);

	return ITER_STOP;
}

// Method | Public | Cast -> to_string
String Map_to_string (var this_ptr)
{
	THIS(Map);
	String casted = new(String);
	var function = this->function;
	Type iter_type = type_of(this->iterable);
	print_to(casted, 0, "map(#v to #v)", function, S(iter_type->name));
	return casted;
}