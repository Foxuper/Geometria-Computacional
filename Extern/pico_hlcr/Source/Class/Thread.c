// Include | Header
#include <pico_hlcr/Class/Thread.h>

// Variables | Private | Main Thread And Exception
static Thread main_thread = NULL;
static Exception main_exception = NULL;

// Variables | Private | Thread Local Storage Key
static pthread_key_t tls_key;
static bool tls_key_created = false;

// Functions | Private | Thread
static void create_tls_key   (void);
static void delete_tls_key   (void);
static void check_tls_key    (void);
static void delete_main      (void);
static var  function_wrapper (var this_ptr);

// Function | Private | Create TLS Key
static void create_tls_key (void)
{
	int error = pthread_key_create(&tls_key, NULL);

	if (error is EAGAIN)
		throw(RESOURCE_EXCEPTION, "Thread: Couldn't create another TLS key");
	if (error is ENOMEM)
		throw(OUT_OF_MEMORY_EXCEPTION, "Thread: Insufficient memory to create TLS key");
	if (error)
		throw_errno(error, TYPE(Thread), "pthread_key_create");
}

// Function | Private | Delete TLS Key
static void delete_tls_key (void)
{
	int error = pthread_key_delete(tls_key);

	if (error)
		throw_errno(error, TYPE(Thread), "pthread_key_create");
}

// Function | Private | Check TLS Key
static void check_tls_key (void)
{
	if (not tls_key_created)
	{
		create_tls_key();
		int error = atexit(delete_tls_key);

		if (error)
			throw_errno(error, TYPE(Thread), "atexit");

		tls_key_created = true;
	}
}

// Function | Private | Delete Main Thread
static void delete_main (void)
{
	delete(main_exception);
	delete(main_thread);
}

// Function | Private | Function Wrapper
static var function_wrapper (var this_ptr)
{
	THIS(Thread);

	this->is_running = true;
	int error = pthread_setspecific(tls_key, this);

	if (error is ENOMEM)
		return throw(OUT_OF_MEMORY_EXCEPTION, "Thread: Insufficient memory to set TLS key");
	if (error)
		return throw_errno(error, TYPE(Thread), "pthread_setspecific");

	var result = call_with(this->function, this->arguments);
	delete(this->arguments);
	this->arguments = NULL;

	return result;
}

// Method | Public | New -> build_with
void Thread_build_with (var this_ptr, Tuple args)
{
	THIS(Thread);

	this->function = first(args);
	this->arguments = NULL;
	this->storage = new(Table, TYPE(String), TYPE(Ptr));
	this->is_running = false;
}

// Method | Public | New -> build_with
void Thread_destroy (var this_ptr)
{
	THIS(Thread);
	delete(this->arguments);
	delete(this->storage);
}

// Method | Public | Call -> call_with
var Thread_call_with (var this_ptr, var variable)
{
	THIS(Thread);

	check_tls_key();
	this->arguments = copy(variable);
	int error = pthread_create(&this->thread, NULL, function_wrapper, this);

	if (error is EAGAIN)
		return throw(RESOURCE_EXCEPTION, "Thread: Couldn't create another thread");
	if (error is EPERM)
		return throw(RESOURCE_EXCEPTION, "Thread: Insufficient privileges");
	if (error)
		return throw_errno(error, TYPE(Thread), "pthread_create");

	return this;
}

// Method | Public | Current -> current
var Thread_current (void)
{
	check_tls_key();
	Thread this = pthread_getspecific(tls_key);

	if (this is NULL)
	{
		if (main_thread is NULL)
		{
			main_thread = new(Thread);
			main_thread->is_running = true;
			main_thread->thread = pthread_self();

			main_exception = new(Exception);
			int error = atexit(delete_main);

			if (error)
				return throw_errno(error, TYPE(Thread), "atexit");
		}

		this = main_thread;
		int error = pthread_setspecific(tls_key, this);

		if (error is ENOMEM)
			return throw(OUT_OF_MEMORY_EXCEPTION, "Thread: Insufficient memory to set TLS key");
		if (error)
			return throw_errno(error, TYPE(Thread), "pthread_setspecific");
	}

	return this;
}

// Method | Public | Run -> start
void Thread_start (var this)
{
	Thread_call_with(this, NULL);
}

// Method | Public | Run -> stop
void Thread_stop (var this_ptr)
{
	THIS(Thread);
	int error = pthread_kill(this->thread, SIGINT);

	if (error is EINVAL)
		throw(ARGUMENT_EXCEPTION, "Thread: Invalid arguments to stop thread");
	if (error)
		throw_errno(error, TYPE(Thread), "pthread_kill");

	this->is_running = false;
}

// Method | Public | Run -> join
void Thread_join (var this_ptr)
{
	THIS(Thread);
	int error = pthread_join(this->thread, NULL);

	if (error is EDEADLK)
		throw(RESOURCE_EXCEPTION, "Thread: Deadlock detected in joining thread");
	if (error)
		throw_errno(error, TYPE(Thread), "pthread_join");

	this->is_running = false;
}

// Method | Public | Run -> running
bool Thread_running (var this_ptr)
{
	THIS(Thread);
	return this->is_running;
}

// Method | Public | Access -> get
var Thread_get (var this_ptr, var key)
{
	THIS(Thread);
	return get(this->storage, key);
}

// Method | Public | Access -> set
void Thread_set (var this_ptr, var key, var value)
{
	THIS(Thread);
	set(this->storage, key, value);
}

// Method | Public | Content -> key_type
Type Thread_key_type (var this_ptr)
{
	THIS(Thread);
	return key_type(this->storage);
}

// Method | Public | Content -> value_type
Type Thread_value_type (var this_ptr)
{
	THIS(Thread);
	return value_type(this->storage);
}

// Method | Public | Insert -> insert_at
void Thread_insert_at (var this_ptr, var key, var value)
{
	THIS(Thread);
	insert_at(this->storage, key, value);
}

// Method | Public | Insert -> erase_at
void Thread_erase_at (var this_ptr, var key)
{
	THIS(Thread);
	erase_at(this->storage, key);
}