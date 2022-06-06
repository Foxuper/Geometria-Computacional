// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Definitions | Private | Special Characters
#define CHR_SPECIAL '#'
#define CHR_VARIABLE 'v'

// Definition | Private | Variable Print Format
#define VAR_FORMAT "#v"

// Method | Public | Show
int64 show (var this)
{
	return show_to(this, v(File, stdout), 0);
}

// Method | Public | Look
int64 look (var this)
{
	return look_from(this, v(File, stdin), 0);
}

// Method | Public | Show To
int64 show_to (var this, var other, uint64 index)
{
	INSTANCE(IO);

	if (IMPLEMENTS(IO, show_to))
		return INS(IO)->show_to(this, other, index);

	int64 error = 0;

	with (String, string as str(this))
		error = print_to(other, index, VAR_FORMAT, string);

	return error;
}

// Method | Public | Look From
int64 look_from (var this, var other, uint64 index)
{
	INSTANCE(IO);

	if (IMPLEMENTS(IO, look_from))
		return INS(IO)->look_from(this, other, index);

	REPORT_METHOD(IO, look_from);
	return -1;
}

// Method | Public | Internal | Print With
int64 print_with (char *format, Tuple args)
{
	return print_to_with(v(File, stdout), 0, format, args);
}

// Method | Public | Internal | Scan With
int64 scan_with (char *format, Tuple args)
{
	return scan_from_with(v(File, stdin), 0, format, args);
}

// Method | Public | Internal | Println With
int64 println_with (char *format, Tuple args)
{
	int64 error = print_to_with(v(File, stdout), 0, format, args);
	return (error < 0) ? error : print_to_with(v(File, stdout), error, "\n", NULL);
}

// Method | Public | Internal | Scanln With
int64 scanln_with (char *format, Tuple args)
{
	int64 error = scan_from_with(v(File, stdin), 0, format, args);
	return (error < 0) ? error : scan_from_with(v(File, stdin), error, "\n", NULL);
}

// Method | Public | Internal | Print To With
int64 print_to_with (var this, uint64 index, char *format, Tuple args)
{
	String text, buffer = new(String, S(format));
	int64 error = 0, position = index;
	char *current = buffer->string;
	var value = init(args);
	bool update = true;

	while (true)
	{
		text = update ? S(current) : text;

		while ((*current isnt CHR_SPECIAL) and (*current isnt CHR_NULL))
			current++;

		if (*current is CHR_SPECIAL)
		{
			if (*(current + 1) is CHR_VARIABLE)
			{
				update = true;
				*current = CHR_NULL;
				error = show_to(text, this, position);

				if (error < 0)
				{
					delete(buffer);
					return error;
				}

				if (value is ITER_STOP)
					return (int64) throw(ARGUMENT_EXCEPTION, "IO: Not enough arguments to print");

				position += error;
				error = show_to(value, this, position);

				if (error < 0)
				{
					delete(buffer);
					return error;
				}

				value = next(args, value);
				position += error;
				current++;
			}
			else if (*(current + 1) is CHR_SPECIAL)
			{
				update = true;
				*(current + 1) = CHR_NULL;
				error = show_to(text, this, position);

				if (error < 0)
				{
					delete(buffer);
					return error;
				}

				position += error;
				current++;
			}
			else
				update = false;

			current++;
		}
		else
		{
			error = show_to(text, this, position);

			if (error < 0)
			{
				delete(buffer);
				return error;
			}

			position += error;
			break;
		}
	}

	if (value isnt ITER_STOP)
		return (int64) throw(ARGUMENT_EXCEPTION, "IO: More arguments to print than expected");

	delete(buffer);
	return position - index;
}

// Method | Public | Internal | Scan From With
int64 scan_from_with (var this, uint64 index, char *format, Tuple args)
{
	String text, buffer = new(String, S(format));
	int64 error = 0, position = index;
	char *current = buffer->string;
	var value = init(args);
	bool update = true;

	while (true)
	{
		text = update ? S(current) : text;

		while ((*current isnt CHR_SPECIAL) and (*current isnt CHR_NULL))
			current++;

		if (*current is CHR_SPECIAL)
		{
			if (*(current + 1) is CHR_VARIABLE)
			{
				update = true;
				*current = CHR_NULL;

				with (String, text_copy as copy(text))
				{
					int64 scanned = 0;
					concat(text_copy, S("%n"));
					error = format_from(this, position, c_string(text_copy), &scanned);
					position += scanned;
				}

				if (error < 0)
				{
					delete(buffer);
					return error;
				}

				if (value is ITER_STOP)
					return (int64) throw(ARGUMENT_EXCEPTION, "IO: Not enough arguments to scan");

				error = look_from(value, this, position);

				if (error < 0)
				{
					delete(buffer);
					return error;
				}

				value = next(args, value);
				position += error;
				current++;
			}
			else if (*(current + 1) is CHR_SPECIAL)
			{
				update = true;
				*(current + 1) = CHR_NULL;

				with (String, text_copy as copy(text))
				{
					int64 scanned = 0;
					concat(text_copy, S("%n"));
					error = format_from(this, position, c_string(text_copy), &scanned);
					position += scanned;
				}

				if (error < 0)
				{
					delete(buffer);
					return error;
				}

				position += error;
				current++;
			}
			else
				update = false;

			current++;
		}
		else
		{
			with (String, text_copy as copy(text))
			{
				int64 scanned = 0;
				concat(text_copy, S("%n"));
				error = format_from(this, position, c_string(text_copy), &scanned);
				position += scanned;
			}

			if (error < 0)
			{
				delete(buffer);
				return error;
			}

			break;
		}
	}

	if (value isnt ITER_STOP)
		return (int64) throw(ARGUMENT_EXCEPTION, "IO: More arguments to scan than expected");

	delete(buffer);
	return position - index;
}

// Class | Public | IO
Type TYPE(IO) = new_type(IO);