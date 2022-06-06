// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Get C Integer
int64 c_int (var this)
{
	INSTANCE(C_Types);

	if (IMPLEMENTS(C_Types, c_int))
		return INS(C_Types)->c_int(this);

	REPORT_METHOD(C_Types, c_int);
	return 0;
}

// Method | Public | Get C Float
double c_float (var this)
{
	INSTANCE(C_Types);

	if (IMPLEMENTS(C_Types, c_float))
		return INS(C_Types)->c_float(this);

	REPORT_METHOD(C_Types, c_float);
	return 0;
}

// Method | Public | Get C String
char *c_string (var this)
{
	INSTANCE(C_Types);

	if (IMPLEMENTS(C_Types, c_string))
		return INS(C_Types)->c_string(this);

	REPORT_METHOD(C_Types, c_string);
	return NULL;
}

// Interface | Public | C_Types
Type TYPE(C_Types) = new_type(C_Types);