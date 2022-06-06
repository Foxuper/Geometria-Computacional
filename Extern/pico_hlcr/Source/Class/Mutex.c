// Include | Header
#include <pico_hlcr/Class/Mutex.h>

// Method | Public | New -> build_with
void Mutex_build_with (var this_ptr, Tuple args)
{
	THIS(Mutex);
	UNUSED(args);
	pthread_mutex_init(&this->mutex, NULL);
}

// Method | Public | New -> destroy
void Mutex_destroy (var this_ptr)
{
	THIS(Mutex);
	pthread_mutex_destroy(&this->mutex);
}

// Method | Public | Lock -> lock
void Mutex_lock (var this_ptr)
{
	THIS(Mutex);
	pthread_mutex_lock(&this->mutex);
}

// Method | Public | Lock -> unlock
void Mutex_unlock (var this_ptr)
{
	THIS(Mutex);
	pthread_mutex_unlock(&this->mutex);
}

// Method | Public | Lock -> trylock
bool Mutex_trylock (var this_ptr)
{
	THIS(Mutex);
	int error = pthread_mutex_trylock(&this->mutex);
	return error ? false : true;
}