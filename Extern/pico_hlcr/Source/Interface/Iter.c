// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Get Iterable
var iter (var this)
{
	INSTANCE(Iter);

	if (IMPLEMENTS(Iter, iter))
		return INS(Iter)->iter(this);

	return this;
}

// Method | Public | Initialize Iterable
var init (var this)
{
	INSTANCE(Iter);

	if (IMPLEMENTS(Iter, init))
		return INS(Iter)->init(this);

	#if METHOD_CHECK
		if (this isnt NULL)
			REPORT_METHOD(Iter, init);
	#endif

	return ITER_STOP;
}

// Method | Public | Get Next Value
var next (var this, var current)
{
	INSTANCE(Iter);

	if (IMPLEMENTS(Iter, next))
		return INS(Iter)->next(this, current);

	#if METHOD_CHECK
		if (this isnt ITER_STOP)
			REPORT_METHOD(Iter, next);
	#endif

	return ITER_STOP;
}

// Method | Public | Iterable Value Type
Type iter_type (var this)
{
	INSTANCE(Iter);

	if (IMPLEMENTS(Iter, iter_type))
		return INS(Iter)->iter_type(this);

	return VAR;
}

// Interface | Public | Constants
Type _ = empty_type(_);
Type ITER_STOP = empty_type(ITER_STOP);

// Interface | Public | Iter
Type TYPE(Iter) = new_type(Iter);