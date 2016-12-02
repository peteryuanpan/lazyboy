#ifndef HASH_H
#define HASH_H
#include <cstring>

class Hash
{
public:
	Hash();
	long long get_hash(const char *s);
private:
	int seed[2];
	int mod[2];
	int single_hash(const char *s, const int seed, const int mod);
};

#endif