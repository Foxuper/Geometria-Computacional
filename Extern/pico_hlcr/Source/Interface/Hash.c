// Include | Header
#include <pico_hlcr/pico_hlcr.h>

// Private | Functions | Hash
static uint64 murmur_hash_64 (var data, uint64 size, uint64 seed);

// Method | Public | Hash
uint64 hash (var this)
{
	INSTANCE(Hash);

	if (IMPLEMENTS(Hash, hash))
		return INS(Hash)->hash(this);

	return hash_data(this, size(this));
}

// Method | Public | Hash Data
uint64 hash_data (var data, uint64 size)
{
	return murmur_hash_64(data, size, HASH_SEED);
}

// Private | Function | Murmur Hash
static uint64 murmur_hash_64 (var data, uint64 size, uint64 seed)
{
	const uint64 *end = (uint64 *) data + (size / 8);
	const uint64 m = 0xc6a4a7935bd1e995;
	const uint64 *data_a = data;
	const int r = 47;

	uint64 h = seed ^ (size * m);

	while (data_a != end)
	{
		uint64 k = *data_a++;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h ^= k;
		h *= m; 
	}

	const uint8 *data_b = (const uint8 *) data_a;

	switch (size & 7)
	{
		case 7: h ^= ((uint64) data_b[6]) << 48; goto case_a;
		case 6: case_a: h ^= ((uint64) data_b[5]) << 40; goto case_b;
		case 5: case_b: h ^= ((uint64) data_b[4]) << 32; goto case_c;
		case 4: case_c: h ^= ((uint64) data_b[3]) << 24; goto case_d;
		case 3: case_d: h ^= ((uint64) data_b[2]) << 16; goto case_e;
		case 2: case_e: h ^= ((uint64) data_b[1]) << 8; goto case_f;
		case 1: case_f: h ^= ((uint64) data_b[0]);
		h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}

// Interface | Public | Hash
Type TYPE(Hash) = new_type(Hash);