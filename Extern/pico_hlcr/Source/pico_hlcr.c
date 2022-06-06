// Pico High Level C Runtime
// by. Foxuper

#include <pico_hlcr/pico_hlcr.h>

// Constant | Private | Null Type Name
static char *null_name = "NULL";

// Function | Private | Search Instance
static var search_instance (Type type, Type interface);

#if MAGIC_CHECK
	// Function | Private | Magic Check
	static bool magic_check (var this);
#endif

#if CACHE_SIZE isnt 0
	// Constant | Private | NULL_INSTANCE
	static Type NULL_INSTANCE = empty_type(NULL_INSTANCE);

	// Functions | Private | Cache
	static void cache_insert   (Type type, Type interface, var instance);
	static var  cache_instance (Type type, Type interface);
#endif

// Function | Public | Memory Offset
var memory_offset (var memory, int64 bytes)
{
	return memory ? (uint8 *) memory + bytes : NULL;
}

// Function | Public | Initialize Header
var header_init (var memory, Type type, AllocType alloc)
{
	Header header = memory;
	header->type = type;

	#if ALLOC_CHECK
		header->alloc = alloc;
	#else
		UNUSED(alloc);
	#endif

	#if MAGIC_CHECK
		header->magic = MAGIC_NUMBER;
	#endif

	return memory_offset(header, HEADER_SIZE);
}

// Function | Public | Type Of
Type type_of (var this)
{
	#if MAGIC_CHECK
		if (not magic_check(this))
			return NULL;
	#endif

	Header header = memory_offset(this, -HEADER_SIZE);
	return header ? (header->type ? header->type : TYPE) : NULL;
}

// Function | Public | Is Type
bool is_type (var this)
{
	Type type = type_of(this);
	return type is TYPE;
}

// Function | Public | Is Object
bool is_object (var this)
{
	Type type = type_of(this);
	return type isnt TYPE;
}

// Function | Public | Allocation Type
AllocType alloc_type (var this)
{
	#if ALLOC_CHECK
		Header header = memory_offset(this, -HEADER_SIZE);
		return header ? header->alloc : ALLOC_UNKNOWN;
	#else
		UNUSED(this);
		return ALLOC_UNKNOWN;
	#endif
}

// Function | Public | Allocated at Heap
bool alloc_at_heap (var this)
{
	AllocType alloc = alloc_type(this);
	return alloc is ALLOC_HEAP or alloc is ALLOC_UNKNOWN;
}

// Function | Public | Type Name
char *type_name (var this)
{
	GET_TYPE(this);
	return type ? type->name : null_name;
}

// Function | Public | Clear Cache
void clear_cache (var this)
{
	GET_TYPE(this);

	#if CACHE_SIZE isnt 0
		if (type)
			type->cache_size = 0;
	#else
		UNUSED(type);
	#endif
}

// Function | Public | Get Interface Instance
var instance (var this, Type interface)
{
	GET_TYPE(this);
	return type_instance(type, interface);
}

// Function | Public | Get Interface Instance from Type
var type_instance (Type type, Type interface)
{
	#if CACHE_SIZE isnt 0
		var instance = type ? cache_instance(type, interface) : NULL;

		if (instance is NULL_INSTANCE)
		{
			instance = search_instance(type, interface);
			cache_insert(type, interface, instance);
		}

		return instance;
	#else
		return search_instance(type, interface);
	#endif
}

// Function | Public | Has Method At
bool has_method_at (var this, Type interface, uint64 offset)
{
	var ins = instance(this, interface);
	var method = ins ? ((uint8 *) ins + offset) : NULL;
	return method;
}

// Constant | Public | TYPE
Type TYPE = empty_type(TYPE);

// Function | Private | Search Instance
static var search_instance (Type type, Type interface)
{
	struct Interface *type_interface = type ? type->interfaces : NULL;

	while (type_interface and type_interface->name)
	{
		if (strcmp(type_interface->name, interface->name) is 0)
			return type_interface->instance;

		type_interface++;
	}

	return NULL;
}

#if MAGIC_CHECK
	// Function | Private | Magic Number Check
	static bool magic_check (var this)
	{
		Header header = memory_offset(this, -HEADER_SIZE);

		#if MAGIC_THROW
			if (header and header->magic isnt MAGIC_NUMBER)
				return throw(INTERNAL_EXCEPTION, "Magic number mismatch");
		#else
			if (header and header->magic isnt MAGIC_NUMBER)
				return false;
		#endif

		return true;
	}
#endif

#if CACHE_SIZE isnt 0
	// Function | Private | Insert Interface instance into Cache
	static void cache_insert (Type type, Type interface, var instance)
	{
		if (type->cache_size < CACHE_SIZE)
			type->cache_size++;

		uint16 move_size = (type->cache_size - 1) * sizeof(struct CacheEntry);
		memmove(type->cache + 1, type->cache, move_size);

		type->cache[0].interface = interface;
		type->cache[0].instance = instance;
	}

	// Function | Private | Get Interface Instance from Cache
	static var cache_instance (Type type, Type interface)
	{
		for (uint64 index = 0; index < type->cache_size; index++)
		{
			if (type->cache[index].interface is interface)
				return type->cache[index].instance;
		}

		return NULL_INSTANCE;
	}
#endif