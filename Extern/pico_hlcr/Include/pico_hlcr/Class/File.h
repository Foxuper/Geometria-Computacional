#ifndef PICO_HLCR_FILE_H // Header | File | START
#define PICO_HLCR_FILE_H

// Include | Core
#include <pico_hlcr/pico_hlcr.h>

// Methods | Public | New
void File_build_with (var this_ptr, Tuple args);
void File_destroy    (var this_ptr);

// Methods | Public | Len
uint64 File_len (var this_ptr);

// Methods | Public | Context
void File_leave (var this_ptr);

// Methods | Public | Cast
String File_to_string (var this_ptr);

// Methods | Public | Format
int64 File_format_to   (var this_ptr, uint64 index, char *format, va_list args);
int64 File_format_from (var this_ptr, uint64 index, char *format, va_list args);

// Methods | Public | Stream
void    File_open  (var this_ptr, char *name, char *mode);
void    File_close (var this_ptr);
uint64  File_read  (var this_ptr, void *buffer, uint64 size);
uint64  File_write (var this_ptr, void *buffer, uint64 size);
int64   File_seek  (var this_ptr, uint64 index, int origin);
uint64  File_tell  (var this_ptr);
int64   File_flush (var this_ptr);
bool    File_eof   (var this_ptr);

// Type | Public | File
Type TYPE(File) = new_type
(
	File,
	implement(New, File_build_with, File_destroy),
	implement(Len, File_len),
	implement(Context, NULL, File_leave),
	implement(Cast, NULL, File_to_string),
	implement(Format, File_format_to, File_format_from),
	implement(Stream, File_open, File_close, File_read, File_write, File_seek, File_tell, File_flush, File_eof)
);

#endif // Header | PICO_HLCR_FILE_H | END