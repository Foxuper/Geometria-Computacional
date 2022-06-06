// Include | Header
#include <pico_hlcr/Class/Filter.h>

// Method | Public | New -> build_with
void Filter_build_with (var this_ptr, Tuple args)
{
	THIS(Filter);
	this->iterable = get(args, I(0));
	this->function = get(args, I(1));
}

// Method | Public | Call -> call_with
var Filter_call_with (var this_ptr, var variable)
{
	THIS(Filter);
	UNUSED(variable);
	Tuple filtered = new(Tuple);

	foreach (var, value in this->iterable)
	{
		if (not call_with(this->function, value))
			insert(filtered, copy(value));
	}

	foreach (var, value in filtered)
	{
		erase(this->iterable, value);
		delete(value);
	}

	delete(filtered);
	return this->iterable;
}

// Method | Public | Iter -> init
var Filter_init (var this_ptr)
{
	THIS(Filter);
	var value = init(this->iterable);
	
	while (value isnt ITER_STOP)
	{
		if (call_with(this->function, value))
			return value;
		value = next(this->iterable, value);
	}

	return ITER_STOP;
}

// Method | Public | Iter -> next
var Filter_next (var this_ptr, var current)
{
	THIS(Filter);
	var value = next(this->iterable, current);

	while (value isnt ITER_STOP)
	{
		if (call_with(this->function, value))
			return value;
		value = next(this->iterable, value);
	}

	return ITER_STOP;
}

// Method | Public | Iter -> iter_type
Type Filter_iter_type (var this_ptr)
{
	THIS(Filter);
	return iter_type(this->iterable);
}

// Method | Public | Cast -> to_string
String Filter_to_string (var this_ptr)
{
	THIS(Filter);
	String casted = new(String);
	var function = this->function;
	Type iter_type = type_of(this->iterable);
	print_to(casted, 0, "filter(#v by #v)", S(iter_type->name), function);
	return casted;
}

// Method | Public | Content -> contains
bool Filter_contains (var this_ptr, var value)
{
	THIS(Filter);
	return call_with(this->function, value) and contains(this->iterable, value);
}

// Method | Public | Content -> index_of
var Filter_index_of (var this_ptr, var value)
{
	THIS(Filter);
	return call_with(this->function, value) ? index_of(this->iterable, value) : NULL;
}