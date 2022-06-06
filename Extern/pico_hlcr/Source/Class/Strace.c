// Include | Header
#include <pico_hlcr/pico_hlcr/pico_hlcr.h>

// Definition | Private | Strace Thread Key
#define STRACE_KEY S("__STRACE_KEY__")

static bool strace_running = false;

void enable_strace (void)
{
	strace_running = true;
}

// Function | Public | Strace Enter
void strace_enter (var object, char *method)
{
	Thread thread = current(TYPE(Thread));
	if (thread and not thread->strace_running) return;

	Strace strace = deref(get(thread, STRACE_KEY));
	resize(strace->stack, len(strace->stack) + 1);
	print_to(last(strace->stack), 0, "#v -> #v", S(type_name(object)), S(method));
}

// Function | Public | Strace Leave
void strace_leave (void)
{
	Strace strace = current(TYPE(Strace));
	if (strace is NULL) return;
	uint64 length = len(strace->stack);

	if (length is 0)
		throw(INTERNAL_EXCEPTION, "Corrupted stack trace");

	resize(strace->stack, length - 1);
}

// Function | Public | Strace Print
void strace_print (void)
{
	Strace strace = current(TYPE(Strace));
	if (strace is NULL) return;
	uint64 index = 0;

	print_to(v(File, stderr), 0, "!!  \n");
	print_to(v(File, stderr), 0, "!!  Stack Trace:\n");
	print_to(v(File, stderr), 0, "!!  \n");

	if (len(strace->stack) is 0)
		print_to(v(File, stderr), 0, "!!    EMPTY STACK\n");

	foreach (line in reverse(strace->stack))
		print_to(v(File, stderr), 0, "!!    [#v]: #v\n", I(index++), line);

	print_to(v(File, stderr), 0, "!!  \n");
}

// Method | Private | New -> build_with
static void Strace_build_with (var this_ptr, Tuple args)
{
	THIS(Strace);
	UNUSED(args);
	this->stack = new(Array, TYPE(String));
	insert_at(current(TYPE(Thread)), S(STRACE_KEY), this);
}

// Method | Private | New -> build_with
static void Strace_destroy (var this_ptr)
{
	THIS(Strace);
	delete(this->stack);
	erase_at(current(TYPE(Thread)), S(STRACE_KEY));
}

// Method | Private | Current -> current
static var Strace_current (void)
{
	Thread thread = current(TYPE(Thread));
	
	var current = deref(get(thread, STRACE_KEY));
	return current;
}

Type TYPE(Strace) = new_type
(
	Strace,
	implement(New, Strace_build_with, Strace_destroy),
	implement(Current, Strace_current)
);