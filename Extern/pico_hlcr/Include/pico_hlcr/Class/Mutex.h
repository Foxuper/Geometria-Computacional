#ifndef PICO_HLCR_MUTEX_H // Header | Mutex | START
#define PICO_HLCR_MUTEX_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Mutex_build_with (var this_ptr, Tuple args);
void Mutex_destroy    (var this_ptr);

// Methods | Public | Lock
void Mutex_lock    (var this_ptr);
void Mutex_unlock  (var this_ptr);
bool Mutex_trylock (var this_ptr);

// Class | Public | Mutex
Type TYPE(Mutex) = new_type
(
	Mutex,
	implement(New, Mutex_build_with, Mutex_destroy),
	implement(Lock, Mutex_lock, Mutex_unlock, Mutex_trylock)
);

#endif // Header | PICO_HLCR_MUTEX_H | END