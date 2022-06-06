// Pico High Level C Runtime
// by. Foxuper

#ifndef PICO_HLCR_H
#define PICO_HLCR_H

// Headers | Core
#include <pico_types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <setjmp.h>

// Definitions | Core Settings
#define ALLOC_CHECK true
#define MAGIC_CHECK true
#define MAGIC_THROW true
#define CACHE_SIZE  16

// Definitions | Settings
#define TYPE_CHECK   true
#define METHOD_CHECK true
#define HASH_SEED    0
#define BUFFER_SIZE  256
#define LOAD_FACTOR  0.75
#define ALLOC_FACTOR 1.5

// Definition | Magic Number
#define MAGIC_NUMBER 0x123456789ABCDEF0ull

// Definitions | Syntax Replacers
#define is   ==
#define isnt !=
#define not  !
#define and  &&
#define or   ||

// Definitions | Alloc Automatic Settings
#if not ALLOC_CHECK
	#define HEADER_ALLOC_INIT
#else
	#define HEADER_ALLOC_INIT , .alloc = ALLOC_STATIC
#endif

// Definitions | Magic Number Automatic Settings
#if not MAGIC_CHECK
	#define HEADER_MAGIC_INIT
#else
	#define HEADER_MAGIC_INIT , .magic = MAGIC_NUMBER
#endif

// Definition | Cache Automatic Setting
#if CACHE_SIZE is 0
	#define TYPE_CACHE
#else
	#define TYPE_CACHE , .cache_size = 0
#endif

// Definitions | Common Syntax Abstractions
#define TYPE_INSTANCE(interface) interface INS(interface) = type_instance(type, TYPE(interface))
#define INSTANCE(interface) interface INS(interface) = instance(this, TYPE(interface))
#define GET_TYPE(this) Type type = type_of(this); type = (type is TYPE) ? this : type
#define IMPLEMENTS(interface, method) INS(interface) and INS(interface)->method
#define THIS(type) type this = this_ptr
#define THISP(type) THIS(PTR(type))
#define UNUSED(var) (void) var

// Definitions | Preprocessor Varargs Macros
#define VARGS_LEN(...) sizeof((var[]) {NULL, ##__VA_ARGS__}) / sizeof(var) - 1
#define VARGS_CONCAT(IGNORED, ...) __VA_ARGS__

// Definitions | Variable Wrappers
#define STOUN(var) StructOrUnion_ ## var
#define TEMP(var)  Temporal_ ## var
#define INS(var)   Instance_ ## var
#define TYPE(var)  Type_ ## var
#define PTR(var)   Ptr_ ## var

// Definitions | Header Offset and Size
#define HEADER_OFFSET sizeof(struct Creator) - sizeof(struct Type)
#define HEADER_SIZE   sizeof(struct Header)

// Definitions | Type Creation Macros
#define TYPE_SIZE(type)      sizeof(STOUN(type))
#define TYPE_HEADER          {.type = NULL HEADER_ALLOC_INIT HEADER_MAGIC_INIT}
#define TYPE_INTERFACES(...) (struct Interface []) {VARGS_CONCAT(_, ## __VA_ARGS__, {NULL, NULL})}
#define TYPE_CREATOR(...)    (Type) ((uint8 *) &((struct Creator) {__VA_ARGS__}) + HEADER_OFFSET)

// Definition | Stack Memory Allocation
#define STACK_MEMORY(type) ((var) &((char[TYPE_SIZE(type) + HEADER_SIZE]) {0}))

// Definitions | Structure / Union Type Definition
#define TYPEDEF_STRUCT(name)     typedef struct name *name; typedef struct name STOUN(name)
#define TYPEDEF_UNION(name)      typedef union name *name; typedef union name STOUN(name)
#define TYPEDEF_STRUCT_PTR(name) typedef struct name *PTR(name); typedef struct name STOUN(name)
#define TYPEDEF_UNION_PTR(name)  typedef union name *PTR(name); typedef union name STOUN(name)

// Definitions | Type Declarations
#define declare_empty(type)      Type type
#define declare_only(type)       Type TYPE(type)
#define declare_type(type)       declare_only(type); TYPEDEF_STRUCT(type)
#define declare_type_u(type)     declare_only(type); TYPEDEF_UNION(type)
#define declare_existent(type)   declare_only(type); TYPEDEF_STRUCT_PTR(type)
#define declare_existent_u(type) declare_only(type); TYPEDEF_UNION_PTR(type)

// Definitions | Type Structure / Union Creation
#define type_struct(name)      TYPEDEF_STRUCT(name); struct name
#define type_union(name)       TYPEDEF_UNION(name); union name
#define full_type_struct(name) declare_only(name); type_struct(name)
#define full_type_union(name)  declare_only(name); type_union(name)

// Definitions | Pretty Syntax for Class and Interface
#define new_class(name)     full_type_struct(name)
#define new_class_u(name)   full_type_union(name)
#define new_interface(name) full_type_struct(name)

// Definitions | Has Method
#define has_method(this, interface, method) has_method_at(HAS_ARGS(this, interface, method))
#define HAS_ARGS(this, interface, method) this, TYPE(interface), offsetof(struct interface, method)

// Definition | Empty Type Creation
#define empty_type(type_name) TYPE_CREATOR \
( \
	.header = \
		TYPE_HEADER, \
	.type = \
	{ \
		.name = #type_name, \
		.size = 0, \
		.interfaces = NULL \
		TYPE_CACHE \
	} \
)

// Definition | Type Creation
#define new_type(type_name, ...) TYPE_CREATOR \
( \
	.header = \
		TYPE_HEADER, \
	.type = \
	{ \
		.name = #type_name, \
		.size = TYPE_SIZE(type_name), \
		.interfaces = TYPE_INTERFACES(__VA_ARGS__) \
		TYPE_CACHE \
	} \
)

// Definitions | Stack Variable
#define v(type, ...) memcpy \
( \
	header_init(STACK_MEMORY(type), TYPE(type), ALLOC_STACK), \
	&((STOUN(type)) {__VA_ARGS__}), \
	TYPE_SIZE(type) \
)

// Definition | Interface Implementation
#define implement(interface, ...) \
{ \
	.name = #interface, \
	.instance = &((struct interface) {__VA_ARGS__}) \
}

// Data Type | Allocation Type
typedef enum AllocType
{
	ALLOC_UNKNOWN,
	ALLOC_STATIC,
	ALLOC_STACK,
	ALLOC_HEAP,
	ALLOC_DATA
}
AllocType;

// Data Type | Variable
typedef void *var;

// Data Type | Variable Type
typedef struct Type
{
	char *name;
	uint64 size;
	struct Interface *interfaces;

	#if CACHE_SIZE isnt 0
		uint8 cache_size;

		struct CacheEntry
		{
			struct Type *interface;
			var instance;
		}
		cache[CACHE_SIZE];
	#endif
}
*Type;

// Data Type | Variable Header
typedef struct Header
{
	Type type;

	#if ALLOC_CHECK
		AllocType alloc;
	#endif

	#if MAGIC_CHECK
		uint64 magic;
	#endif
}
*Header;

// Structure | Interface Instance
struct Interface
{
	char *name;
	var instance;
};

// Structure | Type Creator
struct Creator
{
	struct Header header;
	struct Type type;
};

// Constant | TYPE
extern declare_empty(TYPE);

// Functions | Primitive Functions
var memory_offset (var memory, int64 bytes);
var header_init   (var memory, Type type, AllocType alloc);

// Functions | Core Functions
Type      type_of       (var this);
bool      is_type       (var this);
bool      is_object     (var this);
AllocType alloc_type    (var this);
bool      alloc_at_heap (var this);
char     *type_name     (var this);
void      clear_cache   (var this);
var       instance      (var this, Type interface);
var       type_instance (Type type, Type interface);
bool      has_method_at (var this, Type interface, uint64 offset);

// ----------------------- Core | End -----------------------

// Definitions | Method Automatic Settings
#if not METHOD_CHECK
	#define REPORT_METHOD(name, method)
	#define TYPE_REPORT_METHOD(name, method)
#else
	#define REPORT_METHOD(name, method) report_method(this, TYPE(name), INS(name), #method)
	#define TYPE_REPORT_METHOD(name, method) report_method(type, TYPE(name), INS(name), #method)
#endif

// Definitions | Characters
#define CHR_NULL  '\0'
#define CHR_SPACE ' '

// Definitions | Syntax Replacers
#define in , // Foreach
#define as , // With, Catch
#define to , // Cast, Map
#define by , // Filter

// Definition | Class Macro -> Tuple
#define tuple(...) v(Tuple, (var[]) {NULL, ##__VA_ARGS__} + 1, VARGS_LEN(__VA_ARGS__), 0)

// Definition | Class Macro -> Range
#define range(...) range_init(v(Range, I(0), 0, 0, 0), tuple(__VA_ARGS__))

// Definition | Class Macro -> Filter
#define filter(...) FILTER_AUX(__VA_ARGS__)
#define FILTER_AUX(iterable, function) v(Filter, iter(iterable), function)

// Definition | Class Macro -> Map
#define map(...) MAP_AUX(__VA_ARGS__)
#define MAP_AUX(function, iterable) v(Map, function, iter(iterable), NULL)

// Definition | Class Macro -> Zip
#define zip(...) v(Zip, tuple(__VA_ARGS__), tuple(__VA_ARGS__))

// Definition | Class Macro -> Enumerate
#define enumerate(iterable) zip(range(I(len(iterable))), iterable)

// Definition | Class Macro -> Slice
#define slice(iterable, ...) v(Slice, iterable, range(__VA_ARGS__))

// Definition | Class Macro -> Reverse
#define reverse(iterable) slice(iterable, I(len(iterable) - 1), I(-1), I(-1))

// Definition | Class Macro -> Throw
#define throw(exception, format, ...) throw_exception(exception, format, tuple(__VA_ARGS__))

// Definition | Class Macro -> Try
#define try for \
( \
	jmp_buf TEMP(env), *TEMP(env_ptr) = &TEMP(env); \
	try_statement(TEMP(env_ptr)); \
	TEMP(env_ptr) = NULL \
) \
if (setjmp(TEMP(env)) is 0)

// Definition | Class Macro -> Catch
#define catch(...) CATCH_AUX(__VA_ARGS__)
#define CATCH_AUX(e, ...) for (Type e = catch_with(tuple(__VA_ARGS__)); e; e = NULL)

// Definition | Class Macro -> Finally
#define finally for (bool TEMP(fin) = finally_enter(); TEMP(fin); TEMP(fin) = finally_leave())

// Definitions | Primitive Types
#define P(value)  v(Ptr, value)
#define R(value)  v(Ref, value)
#define I(value)  v(Int, value)
#define F(value)  v(Float, value)
#define S(value)  v(String, value)
#define FX(value) v(Function, value)

// Class | Tuple
extern new_class(Tuple)
{
	var *array;
	uint64 len;
	uint64 index;
};

// Class | Ptr
extern new_class(Ptr)
{
	var pointer;
};

// Class | Ref
extern new_class(Ref)
{
	var reference;
};

// Class | Int
extern new_class(Int)
{
	int64 value;
};

// Class | Float
extern new_class(Float)
{
	double value;
};

// Class | String
extern new_class(String)
{
	char *string;
};

// Class | Function
extern new_class(Function)
{
	var (*function) (var variable);
};

// Class | Array
extern new_class(Array)
{
	var *array;
	Type type;
	uint64 len;
	uint64 slots;
	uint64 slot_size;
};

// Class | List
extern new_class(List)
{
	var head;
	var tail;
	uint64 len;
};

// Constants | Declaration -> List
extern declare_empty(NONE);

// Class | Table
extern new_class(Table)
{
	var table;
	Type key_type;
	Type value_type;
	uint64 key_size;
	uint64 value_size;
	uint64 len;
	uint64 slots;
	uint64 slot_size;
	double load_factor;
};

// Class | Range
extern new_class(Range)
{
	Int value;
	int64 start;
	int64 stop;
	int64 step;
};

// Class | Filter
extern new_class(Filter)
{
	var iterable;
	Function function;
};

// Class | Map
extern new_class(Map)
{
	Function function;
	var iterable;
	var current;
};

// Class | Zip
extern new_class(Zip)
{
	Tuple iterables;
	Tuple values;
};

// Class | Slice
extern new_class(Slice)
{
	var iterable;
	Range range;
};

// Class | File
extern new_class(File)
{
	FILE* file;
};

// Class | Thread
extern new_class(Thread)
{
	Function function;
	Tuple arguments;
	Table storage;
	pthread_t thread;
	bool is_running;
};

// Class | Mutex
extern new_class(Mutex)
{
	pthread_mutex_t mutex;
};

// Class | Exception
extern new_class(Exception)
{
	Type exception;
	String message;
	Array buffer;
	bool thrown;
};

// Function | Internal | Range
Range range_init (Range range, Tuple args);

// Functions | Exception
void   register_signals  (void);
String exception_msg     (void);
void   exception_handler (void (*function) (void));
var    throw_errno       (int error, Type type, char *function);

// Functions | Internal | Exception
void report_method   (Type type, Type interface, var instance, char *method);
var  throw_exception (var exception, char *format, Tuple args);
bool try_statement   (jmp_buf *env);
Type catch_with      (Tuple args);
bool finally_enter   (void);
bool finally_leave   (void);

// Constants | Declaration | List
extern declare_empty(NONE);

// Constants | Declaration | Exception
extern declare_empty(EXCEPTION);
extern declare_empty(ERRNO_EXCEPTION);
extern declare_empty(INTERNAL_EXCEPTION);
extern declare_empty(IO_EXCEPTION);
extern declare_empty(TYPE_EXCEPTION);
extern declare_empty(KEY_EXCEPTION);
extern declare_empty(VALUE_EXCEPTION);
extern declare_empty(METHOD_EXCEPTION);
extern declare_empty(RESOURCE_EXCEPTION);
extern declare_empty(ARGUMENT_EXCEPTION);
extern declare_empty(OUT_OF_MEMORY_EXCEPTION);
extern declare_empty(INDEX_OUT_OF_BOUNDS_EXCEPTION);
extern declare_empty(PROGRAM_ABORTED_EXCEPTION);
extern declare_empty(ARITHMETIC_EXCEPTION);
extern declare_empty(ILLEGAL_INSTRUCTION_EXCEPTION);
extern declare_empty(PROGRAM_INTERRUPTION_EXCEPTION);
extern declare_empty(SEGMENTATION_FAULT_EXCEPTION);
extern declare_empty(PROGRAM_TERMINATION_EXCEPTION);

// _________________________________________________________

// Interface | Size
extern new_interface(Size)
{
	uint64 (*size) (void);
};

// Methods | Size
uint64 size (var this);

// _________________________________________________________

// Interface | Alloc
extern new_interface(Alloc)
{
	var  (*alloc)   (void);
	void (*dealloc) (var this);
};

// Methods | Alloc
var  alloc   (Type type);
void dealloc (var this);

// _________________________________________________________

// Interface | New
extern new_interface(New)
{
	void (*build_with) (var this, Tuple args);
	void (*destroy)    (var this);
};

// Methods | New
void delete  (var this);
var  destroy (var this);

// Methods | Internal | New
var new_with   (Type type, Tuple args);
var build_with (var this, Tuple args);

// Definition | New -> New
#define new(type, ...) new_with(TYPE(type), tuple(__VA_ARGS__))

// Definition | New -> Build
#define build(this, ...) build_with(this, tuple(__VA_ARGS__))

// _________________________________________________________

// Interface | Assign
extern new_interface(Assign)
{
	void (*assign) (var this, var other);
	void (*swap)   (var this, var other);
	var  (*copy)   (var this);
};

// Methods | Assign
var  assign (var this, var other);
void swap   (var this, var other);
var  copy   (var this);

// _________________________________________________________

// Interface | C_Types
extern new_interface(C_Types)
{
	int64  (*c_int)    (var this);
	double (*c_float)  (var this);
	char  *(*c_string) (var this);
};

// Methods | C_Types
int64  c_int    (var this);
double c_float  (var this);
char  *c_string (var this);

// _________________________________________________________

// Interface | Compare
extern new_interface(Compare)
{
	int64 (*cmp) (var this, var other);
};

// Methods | Compare
int64 cmp (var this, var other);
bool  eq  (var this, var other);
bool  ne  (var this, var other);
bool  gt  (var this, var other);
bool  lt  (var this, var other);
bool  ge  (var this, var other);
bool  le  (var this, var other);

// _________________________________________________________

// Interface | Hash
extern new_interface(Hash)
{
	uint64 (*hash) (var this);
};

// Method | Hash
uint64 hash      (var this);
uint64 hash_data (var data, uint64 size);

// _________________________________________________________

// Interface | Len
extern new_interface(Len)
{
	uint64 (*len) (var this);
};

// Methods | Len
uint64 len   (var this);
bool   empty (var this);

// _________________________________________________________

// Interface | Resize
extern new_interface(Resize)
{
	void (*resize) (var this, uint64 new_len);
};

// Method | Resize
void resize (var this, uint64 new_len);

// _________________________________________________________

// Interface | Call
extern new_interface(Call)
{
	var (*call_with) (var this, var variable);
};

// Method | Internal | Call
var call_with (var this, var variable);

// Definition | Call -> Call
#define call(function, ...) call_with(function, tuple(__VA_ARGS__))

// _________________________________________________________

// Interface | Current
extern new_interface(Current)
{
	var (*current) (void);
};

// Methods | Current
var current (Type type);

// _________________________________________________________

// Interface | Run
extern new_interface(Run)
{
	void (*start)   (var this);
	void (*stop)    (var this);
	void (*join)    (var this);
	bool (*running) (var this);
};

// Methods | Run
void start   (var this);
void stop    (var this);
void join    (var this);
bool running (var this);

// _________________________________________________________

// Interface | Lock
extern new_interface(Lock)
{
	void (*lock)    (var this);
	void (*unlock)  (var this);
	bool (*trylock) (var this);
};

// Methods | Lock
void lock    (var this);
void unlock  (var this);
bool trylock (var this);

// _________________________________________________________

// Interface | Context
extern new_interface(Context)
{
	void (*enter) (var this);
	void (*leave) (var this);
};

// Methods | Context
var enter (var this);
var leave (var this);

// Definitions | Context -> With
#define with(...) WITH_AUX(__VA_ARGS__)
#define WITH_AUX(type, variable, object) for \
( \
	type variable = enter(object); \
	(var) variable isnt WITH_EXIT; \
	variable = leave(variable) \
)

// Constant | Declaration | Context
extern declare_empty(WITH_EXIT);

// _________________________________________________________

// Interface | Iter
extern new_interface(Iter)
{
	var  (*iter)      (var this);
	var  (*init)      (var this);
	var  (*next)      (var this, var current);
	Type (*iter_type) (var this);
};

// Methods | Iter
var  iter      (var this);
var  init      (var this);
var  next      (var this, var current);
Type iter_type (var this);

// Definitions | Iter -> Foreach
#define foreach(...) FOREACH_AUX(__VA_ARGS__)
#define FOREACH_AUX(type, variable, iterable) for \
( \
	type TEMP(variable) = iter(iterable), \
	variable = init(TEMP(variable)); \
	(var) variable isnt ITER_STOP; \
	variable = next(TEMP(variable), variable) \
)

// Constants | Declaration | Iter
extern declare_empty(_);
extern declare_empty(ITER_STOP);

// _________________________________________________________

// Interface | Cast
extern new_interface(Cast)
{
	var    (*cast_to)   (var this, Type type);
	String (*to_string) (var this);
};

// Method | Internal | Cast
var    cast_to   (var this, Type type);
String to_string (var this);

// Definitions | Cast -> Cast
#define cast(...) CAST_AUX(__VA_ARGS__)
#define CAST_AUX(variable, type) cast_to(variable, TYPE(type))

// Definition | Cast -> Str
#define str(this) cast(this to String)

// _________________________________________________________

// Interface | Concat
extern new_interface(Concat)
{
	void (*concat) (var this, var other);
};

// Method | Concat
void concat (var this, var other);

// _________________________________________________________

// Interface | Sublist
extern new_interface(Sublist)
{
	var (*sublist) (var this, var key_a, var key_b);
};

// Method | Sublist
var sublist (var this, var key_a, var key_b);

// _________________________________________________________

// Interface | Access
extern new_interface(Access)
{
	var  (*first) (var this);
	var  (*last)  (var this);
	var  (*get)   (var this, var key);
	void (*set)   (var this, var key, var value);
};

// Methods | Access
var  first (var this);
var  last  (var this);
var  get   (var this, var key);
void set   (var this, var key, var value);

// _________________________________________________________

// Interface | Content
extern new_interface(Content)
{
	bool (*contains)   (var this, var value);
	var  (*index_of)   (var this, var value);
	Type (*key_type)   (var this);
	Type (*value_type) (var this);
};

// Methods | Content
bool   contains      (var this, var value);
var    index_of      (var this, var value);
Type   key_type      (var this);
Type   value_type    (var this);
bool   is_key_type   (var this, var key);
bool   is_value_type (var this, var value);

// _________________________________________________________

//Interface | Insert
extern new_interface(Insert)
{
	void (*insert)    (var this, var value);
	void (*erase)     (var this, var value);
	void (*insert_at) (var this, var key, var value);
	void (*erase_at)  (var this, var key);
};

// Methods | Insert
void insert    (var this, var value);
void erase     (var this, var value);
void insert_at (var this, var key, var value);
void erase_at  (var this, var key);

// _________________________________________________________

// Interface | Pointer
extern new_interface(Pointer)
{
	void (*ref)   (var this, var value);
	var  (*deref) (var this);
};

// Methods | Pointer
void ref        (var this, var value);
var  deref      (var this);
bool is_pointer (var this);

// Constant | Declaration | Pointer
extern declare_empty(VAR);

// _________________________________________________________

// Interface | IO
extern new_interface(IO)
{
	int64 (*show_to)   (var this, var other, uint64 index);
	int64 (*look_from) (var this, var other, uint64 index);
};

// Methods | IO
int64 show      (var this);
int64 look      (var this);
int64 show_to   (var this, var other, uint64 index);
int64 look_from (var this, var other, uint64 index);

// Methods | Internal | IO
int64 print_with     (char *format, Tuple args);
int64 scan_with      (char *format, Tuple args);
int64 println_with   (char *format, Tuple args);
int64 scanln_with    (char *format, Tuple args);
int64 print_to_with  (var this, uint64 index, char *format, Tuple args);
int64 scan_from_with (var this, uint64 index, char *format, Tuple args);

// Definitions | IO -> Print
#define print(format, ...) print_with(format, tuple(__VA_ARGS__))
#define println(format, ...) println_with(format, tuple(__VA_ARGS__))
#define print_to(this, index, format, ...) print_to_with(this, index, format, tuple(__VA_ARGS__))

// Definitions | IO -> Scan
#define scan(format, ...) scan_with(format, tuple(__VA_ARGS__))
#define scanln(format, ...) scanln_with(format, tuple(__VA_ARGS__))
#define scan_from(this, index, format, ...) scan_from_with(this, index, format, tuple(__VA_ARGS__))

// _________________________________________________________

// Interface | Format
extern new_interface(Format)
{
	int64 (*format_to)   (var this, uint64 index, char *format, va_list args);
	int64 (*format_from) (var this, uint64 index, char *format, va_list args);
};

// Methods | Format
int64 format_to   (var this, uint64 index, char *format, ...);
int64 format_from (var this, uint64 index, char *format, ...);

// Methods | Internal | Format
int64 format_to_va   (var this, uint64 index, char *format, va_list args);
int64 format_from_va (var this, uint64 index, char *format, va_list args);

// _________________________________________________________

// Interface | Stream
extern new_interface(Stream)
{
	void   (*open)  (var this, char *name, char *mode);
	void   (*close) (var this);
	uint64 (*read)  (var this, void *buffer, uint64 size);
	uint64 (*write) (var this, void *buffer, uint64 size);
	int64  (*seek)  (var this, uint64 index, int origin);
	uint64 (*tell)  (var this);
	int64  (*flush) (var this);
	bool   (*eof)   (var this);
};

// Methods | Stream
var    s_open  (var this, char *name, char *mode);
void   s_close (var this);
uint64 s_read  (var this, void *buffer, uint64 size);
uint64 s_write (var this, void *buffer, uint64 size);
int64  s_seek  (var this, uint64 index, int origin);
uint64 s_tell  (var this);
int64  s_flush (var this);
bool   s_eof   (var this);

#endif // PICO_HLCR_H