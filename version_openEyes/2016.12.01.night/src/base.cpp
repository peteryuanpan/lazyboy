#include "base.h"

void copy_array(int *array, const int *source, const int len_source)
{
	for (int i = 0; i < len_source; i ++)
	{
		array[i] = source[i];
	}
}