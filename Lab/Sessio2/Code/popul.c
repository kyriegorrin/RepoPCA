#include <stdio.h>
#include <stdlib.h>

#define N 10000000

int main(int argc, char *argv[])
{
	unsigned int k, k_saved, i, n;
	unsigned char pop;

	if (argc == 1) n = N; else n = atoi(argv[1]);

	for (i=0; i<n; i++)
	{
		k = k_saved = rand();

		pop = 0;
		while (k > 0)
		{
			if (k % 2) pop++;
			k = k >> 1;
		}

		fwrite(&k_saved, sizeof(k_saved), 1, stdout);
		fwrite(&pop, sizeof(pop), 1, stdout);
	}
        fflush(stdout);

	return 0;
}	
