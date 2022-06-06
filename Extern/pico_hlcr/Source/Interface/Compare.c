// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Compare
int64 cmp (var this, var other)
{
	if (this is other)
		return 0;

	INSTANCE(Compare);

	if (IMPLEMENTS(Compare, cmp))
		return INS(Compare)->cmp(this, other);

	if (type_of(this) is type_of(other))
		return memcmp(this, other, size(this));

	return (uintptr) this - (uintptr) other;
}

// Methods | Public | Comparisons
bool eq (var this, var other) { return cmp(this, other) == 0; }
bool ne (var this, var other) { return cmp(this, other) != 0; }
bool gt (var this, var other) { return cmp(this, other) > 0; }
bool lt (var this, var other) { return cmp(this, other) < 0; }
bool ge (var this, var other) { return cmp(this, other) >= 0; }
bool le (var this, var other) { return cmp(this, other) <= 0; }

// Interface | Public | Compare
Type TYPE(Compare) = new_type(Compare);