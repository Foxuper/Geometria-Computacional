#ifndef PICO_HLCR_EXCEPTION_H // Header | Exception | START
#define PICO_HLCR_EXCEPTION_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void Exception_build_with (var this_ptr, Tuple args);
void Exception_destroy    (var this_ptr);

// Methods | Public | Current
var Exception_current (void);

// Methods | Public | Cast
String Exception_to_string (var this_ptr);

// Class | Public | Constants
Type EXCEPTION = empty_type(EXCEPTION);
Type ERRNO_EXCEPTION = empty_type(ERRNO_EXCEPTION);
Type INTERNAL_EXCEPTION = empty_type(INTERNAL_EXCEPTION);
Type IO_EXCEPTION = empty_type(IO_EXCEPTION);
Type TYPE_EXCEPTION = empty_type(TYPE_EXCEPTION);
Type KEY_EXCEPTION = empty_type(KEY_EXCEPTION);
Type VALUE_EXCEPTION = empty_type(VALUE_EXCEPTION);
Type METHOD_EXCEPTION = empty_type(METHOD_EXCEPTION);
Type RESOURCE_EXCEPTION = empty_type(RESOURCE_EXCEPTION);
Type ARGUMENT_EXCEPTION = empty_type(ARGUMENT_EXCEPTION);
Type OUT_OF_MEMORY_EXCEPTION = empty_type(OUT_OF_MEMORY_EXCEPTION);
Type INDEX_OUT_OF_BOUNDS_EXCEPTION = empty_type(INDEX_OUT_OF_BOUNDS_EXCEPTION);

// Class | Public | Errno Constants
Type PROGRAM_ABORTED_EXCEPTION = empty_type(PROGRAM_ABORTED_EXCEPTION);
Type ARITHMETIC_EXCEPTION = empty_type(ARITHMETIC_EXCEPTION);
Type ILLEGAL_INSTRUCTION_EXCEPTION = empty_type(ILLEGAL_INSTRUCTION_EXCEPTION);
Type PROGRAM_INTERRUPTION_EXCEPTION = empty_type(PROGRAM_INTERRUPTION_EXCEPTION);
Type SEGMENTATION_FAULT_EXCEPTION = empty_type(SEGMENTATION_FAULT_EXCEPTION);
Type PROGRAM_TERMINATION_EXCEPTION = empty_type(PROGRAM_TERMINATION_EXCEPTION);

// Class | Public | Exception
Type TYPE(Exception) = new_type
(
	Exception,
	implement(New, Exception_build_with, Exception_destroy),
	implement(Current, Exception_current),
	implement(Cast, NULL, Exception_to_string)
);

#endif // Header | PICO_HLCR_EXCEPTION_H | END