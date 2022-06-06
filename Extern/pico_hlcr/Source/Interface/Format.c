// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Format To
int64 format_to (var this, uint64 index, char *format, ...)
{
	va_list args;
	va_start(args, format);

	int64 output = format_to_va(this, index, format, args);

	va_end(args);
	return output;
}

// Method | Public | Format From
int64 format_from (var this, uint64 index, char *format, ...)
{
	va_list args;
	va_start(args, format);

	int64 output = format_from_va(this, index, format, args);

	va_end(args);
	return output;
}

// Function | Public | Internal | Format To With Arguments
int64 format_to_va (var this, uint64 index, char *format, va_list args)
{
	INSTANCE(Format);

	if (IMPLEMENTS(Format, format_to))
		return INS(Format)->format_to(this, index, format, args);

	REPORT_METHOD(Format, format_to);
	return -1;
}

// Function | Public | Internal | Format From With Arguments
int64 format_from_va (var this, uint64 index, char *format, va_list args)
{
	INSTANCE(Format);

	if (IMPLEMENTS(Format, format_from))
		return INS(Format)->format_from(this, index, format, args);

	REPORT_METHOD(Format, format_from);
	return -1;
}

// Interface | Public | Format
Type TYPE(Format) = new_type(Format);