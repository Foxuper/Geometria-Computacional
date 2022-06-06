// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Lock
void lock (var this)
{
	INSTANCE(Lock);

	if (IMPLEMENTS(Lock, lock))
	{
		INS(Lock)->lock(this);
		return;
	}

	REPORT_METHOD(Lock, lock);
}

// Method | Public | Unlock
void unlock (var this)
{
	INSTANCE(Lock);

	if (IMPLEMENTS(Lock, unlock))
	{
		INS(Lock)->unlock(this);
		return;
	}

	REPORT_METHOD(Lock, unlock);
}

// Method | Public | Try Lock
bool trylock (var this)
{
	INSTANCE(Lock);

	if (IMPLEMENTS(Lock, trylock))
		return INS(Lock)->trylock(this);

	REPORT_METHOD(Lock, trylock);
	return false;
}

// Interface | Public | Lock
Type TYPE(Lock) = new_type(Lock);