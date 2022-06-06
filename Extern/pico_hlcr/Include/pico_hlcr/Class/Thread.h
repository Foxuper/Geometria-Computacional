#ifndef PICO_HLCR_THREAD_H // Header | Thread | START
#define PICO_HLCR_THREAD_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Thread_build_with (var this_ptr, Tuple args);
void Thread_destroy    (var this_ptr);

// Methods | Public | Call
var Thread_call_with (var this_ptr, var variable);

// Methods | Public | Current
var Thread_current (void);

// Methods | Public | Run
void Thread_start   (var this);
void Thread_stop    (var this_ptr);
void Thread_join    (var this_ptr);
bool Thread_running (var this_ptr);

// Methods | Public | Access
var  Thread_get (var this_ptr, var key);
void Thread_set (var this_ptr, var key, var value);

// Method | Public | Content
Type Thread_key_type   (var this_ptr);
Type Thread_value_type (var this_ptr);

// Methods | Public | Insert
void Thread_insert_at (var this_ptr, var key, var value);
void Thread_erase_at  (var this_ptr, var key);

// Class | Public | Thread
Type TYPE(Thread) = new_type
(
	Thread,
	implement(New, Thread_build_with, Thread_destroy),
	implement(Call, Thread_call_with),
	implement(Current, Thread_current),
	implement(Run, Thread_start, Thread_stop, Thread_join, Thread_running),
	implement(Access, NULL, NULL, Thread_get, Thread_set),
	implement(Content, NULL, NULL, Thread_key_type, Thread_value_type),
	implement(Insert, NULL, NULL, Thread_insert_at, Thread_erase_at)
);

#endif // Header | PICO_HLCR_THREAD_H | END