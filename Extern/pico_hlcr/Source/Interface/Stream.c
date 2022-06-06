// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Method | Public | Stream Open
var s_open (var this, char *name, char *mode)
{
	INSTANCE(Stream);

	if (IMPLEMENTS(Stream, open))
	{
		INS(Stream)->open(this, name, mode);
		return this;
	}
	
	REPORT_METHOD(Stream, open);
	return this;
}

// Method | Public | Stream Close
void s_close (var this)
{
	INSTANCE(Stream);

	if (IMPLEMENTS(Stream, close))
	{
		INS(Stream)->close(this);
		return;
	}

	REPORT_METHOD(Stream, close);
}

// Method | Public | Stream Read
uint64 s_read (var this, void *buffer, uint64 size)
{
	INSTANCE(Stream);

	if (IMPLEMENTS(Stream, read))
		return INS(Stream)->read(this, buffer, size);

	REPORT_METHOD(Stream, read);
	return 0;
}

// Method | Public | Stream Write
uint64 s_write (var this, void *buffer, uint64 size)
{
	INSTANCE(Stream);

	if (IMPLEMENTS(Stream, write))
		return INS(Stream)->write(this, buffer, size);

	REPORT_METHOD(Stream, write);
	return 0;
}

// Method | Public | Stream Seek
int64 s_seek (var this, uint64 index, int origin)
{
	INSTANCE(Stream);

	if (IMPLEMENTS(Stream, seek))
		return INS(Stream)->seek(this, index, origin);

	REPORT_METHOD(Stream, seek);
	return -1;
}

// Method | Public | Stream Tell
uint64 s_tell (var this)
{
	INSTANCE(Stream);

	if (IMPLEMENTS(Stream, tell))
		return INS(Stream)->tell(this);

	REPORT_METHOD(Stream, tell);
	return 0;
}

// Method | Public | Stream Flush
int64 s_flush (var this)
{
	INSTANCE(Stream);

	if (IMPLEMENTS(Stream, flush))
		return INS(Stream)->flush(this);

	REPORT_METHOD(Stream, flush);
	return -1;
}

// Method | Public | Stream EOF
bool s_eof (var this)
{
	INSTANCE(Stream);

	if (IMPLEMENTS(Stream, eof))
		return INS(Stream)->eof(this);

	REPORT_METHOD(Stream, eof);
	return true;
}

// Interface | Public | Size
Type TYPE(Stream) = new_type(Stream);