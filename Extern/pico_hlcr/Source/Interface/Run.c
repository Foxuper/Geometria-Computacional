// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Start
void start (var this)
{
	INSTANCE(Run);

	if (IMPLEMENTS(Run, start))
	{
		INS(Run)->start(this);
		return;
	}

	REPORT_METHOD(Run, start);
}

// Method | Public | Stop
void stop (var this)
{
	INSTANCE(Run);

	if (IMPLEMENTS(Run, stop))
	{
		INS(Run)->stop(this);
		return;
	}

	REPORT_METHOD(Run, stop);
}

// Method | Public | Join
void join (var this)
{
	INSTANCE(Run);

	if (IMPLEMENTS(Run, join))
	{
		INS(Run)->join(this);
		return;
	}

	REPORT_METHOD(Run, join);
}

// Method | Public | Is Running
bool running (var this)
{
	INSTANCE(Run);

	if (IMPLEMENTS(Run, running))
		return INS(Run)->running(this);

	REPORT_METHOD(Run, running);
	return false;
}

// Interface | Public | Run
Type TYPE(Run) = new_type(Run);