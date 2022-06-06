// Include | Header
#include <pico_hlcr/Class/File.h>

// Core Includes
#include <unistd.h>

// Functions | Private | File
static bool is_stream (FILE *file);

// Function | Private | Is Stream
static bool is_stream (FILE *file)
{
	return isatty(fileno(file));
}

// Method | Public | New -> build_with
void File_build_with (var this_ptr, Tuple args)
{
	THIS(File);
	this->file = NULL;
	char *name = c_string(get(args, I(0)));
	char *mode = c_string(get(args, I(1)));

	if (name and mode)
		this->file = fopen(name, mode);
}

// Method | Public | New -> destroy
void File_destroy (var this_ptr)
{
	THIS(File);
	if (this->file)
		fclose(this->file);
}

// Method | Public | Len -> len
uint64 File_len (var this_ptr)
{
	THIS(File);
	uint64 cursor, length = BUFFER_SIZE;

	if (not is_stream(this->file))
	{
		cursor = ftello(this->file);
		fseeko(this->file, 0, SEEK_END);
		length = ftello(this->file);
		fseeko(this->file, cursor, SEEK_SET);
	}

	return length;
}

// Method | Public | Context -> leave
void File_leave (var this_ptr)
{
	THIS(File);
	fclose(this->file);
	this->file = NULL;
}

// Method | Public | Cast -> to_string
String File_to_string (var this_ptr)
{
	THIS(File);
	String casted = new(String);

	with (String, location as new(String))
	{
		format_to(location, 0, "0x%X", this->file);
		print_to(casted, 0, "file(#v)", location);
	}

	return casted;
}

// Method | Public | File -> format_to
int64 File_format_to (var this_ptr, uint64 index, char *format, va_list args)
{
	THIS(File);

	if (not is_stream(this->file))
		fseeko(this->file, index, SEEK_SET);

	return vfprintf(this->file, format, args);
}

// Method | Public | File -> format_from
int64 File_format_from (var this_ptr, uint64 index, char *format, va_list args)
{
	THIS(File);

	if (not is_stream(this->file))
		fseeko(this->file, index, SEEK_SET);

	return vfscanf(this->file, format, args);
}

// Methods | Private | Stream -> open
void File_open (var this_ptr, char *name, char *mode)
{
	THIS(File);
	this->file = fopen(name, mode);
}

// Methods | Private | Stream -> close
void File_close (var this_ptr)
{
	THIS(File);
	fclose(this->file);
}

// Methods | Private | Stream -> read
uint64 File_read (var this_ptr, void *buffer, uint64 size)
{
	THIS(File);
	return fread(buffer, 1, size, this->file);
}

// Methods | Private | Stream -> write
uint64 File_write (var this_ptr, void *buffer, uint64 size)
{
	THIS(File);
	return fwrite(buffer, 1, size, this->file);
}

// Methods | Private | Stream -> seek
int64 File_seek (var this_ptr, uint64 index, int origin)
{
	THIS(File);
	return fseeko(this->file, index, origin);
}

// Methods | Private | Stream -> tell
uint64 File_tell (var this_ptr)
{
	THIS(File);
	return ftello(this->file);
}

// Methods | Private | Stream -> flush
int64 File_flush (var this_ptr)
{
	THIS(File);
	return fflush(this->file);
}

// Methods | Private | Stream -> eof
bool File_eof (var this_ptr)
{
	THIS(File);
	return feof(this->file);
}