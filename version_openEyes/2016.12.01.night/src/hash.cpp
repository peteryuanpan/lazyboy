#include "hash.h"

Hash::Hash()
{
	seed[0] = 233;
	seed[1] = 127;
	mod[0] = 1e9 + 7;
	mod[1] = 1e9 + 9;
}

long long Hash::get_hash(const char *s)
{
	int val_1 = single_hash(s, seed[0], mod[0]);
	int val_2 = single_hash(s, seed[1], mod[1]);
	return 1LL * val_1 * mod[0] + val_2;
}

int Hash::single_hash(const char *s, const int seed, const int mod)
{
	int len = (int) strlen(s);
	long long hash_val = 0;
	for (int i = 0; i < len; i ++)
	{
		hash_val = hash_val * seed + (int) s[i];
		if (hash_val >= mod) hash_val %= mod;
	}
	return (int) hash_val;
}