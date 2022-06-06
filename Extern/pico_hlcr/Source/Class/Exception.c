// Include | Header
#include <pico_hlcr/Class/Exception.h>

// Definition | Private | Exception Thread Key
#define EXCEPTION_KEY S("__EXCEPTION_KEY__")

// Variable | Private | Exception Handler Function
static void (*exception_function) (void) = NULL;

// Functions | Private | Exception
static jmp_buf  *get_environment  (Exception e);
static void      exception_error  (Exception e);
static void      exception_signal (int signal);

// Function | Private | Get Environment
static jmp_buf *get_environment (Exception e)
{
	return deref(last(e->buffer));
}

// Function | Private | Exception Error
static void exception_error (Exception e)
{
	print_to(v(File, stderr), 0, "\n");
	print_to(v(File, stderr), 0, "!!  \n");
	print_to(v(File, stderr), 0, "!!  Uncaught #v\n", S(type_name(e->exception)));
	print_to(v(File, stderr), 0, "!!  \n");
	print_to(v(File, stderr), 0, "!!      #v\n", e->message);
	print_to(v(File, stderr), 0, "!!  \n");
	if (exception_function) exception_function();
	exit(EXIT_FAILURE);
}

// Function | Private | Exception Signal
static void exception_signal (int signal)
{
	register_signals();

	switch (signal)
	{
		case SIGABRT: throw(PROGRAM_ABORTED_EXCEPTION, "Signal: Program Aborted"); break;
		case SIGFPE:  throw(ARITHMETIC_EXCEPTION, "Signal: Arithmetic Error"); break;
		case SIGILL:  throw(ILLEGAL_INSTRUCTION_EXCEPTION, "Signal: Illegal Instruction"); break;
		case SIGINT:  throw(PROGRAM_INTERRUPTION_EXCEPTION, "Signal: Program Interruption"); break;
		case SIGSEGV: throw(SEGMENTATION_FAULT_EXCEPTION, "Signal: Segmentation Fault"); break;
		case SIGTERM: throw(PROGRAM_TERMINATION_EXCEPTION, "Signal: Program Termination"); break;
	}
}

// Function | Public | Report Method
void report_method (Type type, Type interface, var instance, char *method)
{
	String typ = S(type_name(type));
	String itf = S(type_name(interface));

	if (type is NULL)
		throw(METHOD_EXCEPTION, "Null pointer call on method '#v' (#v)", S(method), itf);

	if (instance)
		throw(METHOD_EXCEPTION, "'#v' doesn't implement method '#v' (#v)", typ, S(method), itf);
	else
		throw(METHOD_EXCEPTION, "'#v' doesn't implement interface '#v' (#v)", typ, itf, S(method));
}

// Function | Public | Throw Errno
var throw_errno (int error, Type type, char *function)
{
	char error_buffer[BUFFER_SIZE];
	strerror_s(error_buffer, BUFFER_SIZE, error);

	String name = S(type_name(type));
	String error_text = S(error_buffer);
	return throw(ERRNO_EXCEPTION, "#v: #v() = #v (#v)", name, S(function), I(error), error_text);
}

// Function | Public | Throw Exception
var throw_exception (var exception, char *format, Tuple args)
{
	Exception e = current(TYPE(Exception));

	if (e is NULL)
	{
		e = v(Exception, INTERNAL_EXCEPTION, S("throw: FATAL ERROR"), NULL, true);
		exception_error(e);
		return NULL;
	}

	e->thrown = true;
	e->exception = exception;
	print_to_with(e->message, 0, format, args);

	if (e->thrown or len(e->buffer) is 0)
	{
		exception_error(e);
		return NULL;
	}

	longjmp(*get_environment(e), 1);
	return NULL;
}

// Function | Public | Try Statement
bool try_statement (jmp_buf *env)
{
	Exception e = current(TYPE(Exception));

	if (env)
	{
		if (e->thrown)
			exception_error(e);
		insert(e->buffer, P(env));
	}
	else
		erase_at(e->buffer, I(len(e->buffer) - 1));

	return env;
}

// Function | Public | Catch With
Type catch_with (Tuple args)
{
	Exception e = current(TYPE(Exception));

	if (not e->thrown)
		return NULL;

	if (len(args) is 0)
	{
		e->thrown = false;
		return e->exception;
	}

	foreach (var, arg in args)
	{
		if (arg is e->exception)
		{
			e->thrown = false;
			return e->exception;
		}
	}

	if (not empty(e->buffer))
		longjmp(*get_environment(e), 1);

	return NULL;
}

// Function | Public | Finally Enter
bool finally_enter (void)
{
	Exception e = current(TYPE(Exception));
	if (e->thrown and exception_function)
	{
		exception_function();
		exception_function = NULL;
	}
	return true;
}

// Function | Public | Finally Leave
bool finally_leave (void)
{
	Exception e = current(TYPE(Exception));
	if (e->thrown)
		exception_error(e);
	return false;
}

// Function | Public | Register Exception Signals
void register_signals (void)
{
	signal(SIGABRT, exception_signal);
	signal(SIGFPE, exception_signal);
	signal(SIGILL, exception_signal);
	signal(SIGINT, exception_signal);
	signal(SIGSEGV, exception_signal);
	signal(SIGTERM, exception_signal);
}

// Function | Public | Get Exception Message
String exception_msg (void)
{
	Exception e = current(TYPE(Exception));
	return e->message;
}

// Function | Public | Register Function to Handle Exceptions
void exception_handler (void (*function) (void))
{
	exception_function = function;
}

// Method | Public | New -> build_with
void Exception_build_with (var this_ptr, Tuple args)
{
	THIS(Exception);
	UNUSED(args);

	if (len(args) isnt 0)
		throw(ARGUMENT_EXCEPTION, "Exception: Invalid argument count");

	this->exception = NULL;
	this->message = new(String);
	this->buffer = new(Array);
	this->thrown = false;

	insert_at(current(TYPE(Thread)), EXCEPTION_KEY, this);
}

// Method | Public | New -> destroy
void Exception_destroy (var this_ptr)
{
	THIS(Exception);
	delete(this->message);
	delete(this->buffer);
	erase_at(current(TYPE(Thread)), EXCEPTION_KEY);
}

// Method | Public | Current -> current
var Exception_current (void)
{
	Thread thread = current(TYPE(Thread));
	return deref(get(thread, EXCEPTION_KEY));
}

// Method | Public | Cast -> to_string
String Exception_to_string (var this_ptr)
{
	THIS(Exception);
	String casted = new(String);
	String exception = S(type_name(this->exception));

	if (this->thrown)
		print_to(casted, 0, "exception(#v) -> #v", exception, this->message);
	else
		print_to(casted, 0, "exception(waiting...)");

	return casted;
}