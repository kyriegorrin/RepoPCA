#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define N 1000
#define PUNTS 1000
#define TAMANY_BUFF PUNTS*2*sizeof(double)

int main(int argc, char *argv[])
{
	unsigned int i, r, j, n, it1;
	double d, x, y;
	size_t kk;

	if (argc == 1) n = N; else n = atoi(argv[1]);
	
	//Creating a buffer of the appropiate size
	double *buffer = (double *) malloc(TAMANY_BUFF);
	double *memoMerdes = (double *) malloc(TAMANY_BUFF); 

	srand(0);
	
	//Initial (D) 1000 iterations of the inner loop, for memoization purposes
	r = rand();
	for (j=0, d=0, it1=0; j<PUNTS; j++, it1+=2)
	{
		memoMerdes[it1] = cos(d);
		memoMerdes[it1+1] = sin(d);
		x = r*memoMerdes[it1]; 
		y = r*memoMerdes[it1+1];
		//kk=write(1, &x, sizeof(x));
		//kk=write(1, &y, sizeof(y));
		buffer[it1] = x;
		buffer[it1+1] = y;
		d += 2*M_PI/PUNTS;
	}
	write(1, buffer, TAMANY_BUFF); 

	//Rest of the iterations
	for (i=1, it1=0; i<n; i++)
	{
		r = rand();
		for (j=0, d=0, it1=0; j<PUNTS; j++, it1+=2)
		{
			x = r*memoMerdes[it1]; 
			y = r*memoMerdes[it1+1];
			//kk=write(1, &x, sizeof(x));
			//kk=write(1, &y, sizeof(y));
			buffer[it1] = x;
			buffer[it1+1] = y;
			d += 2*M_PI/PUNTS;
		}
		write(1, buffer, TAMANY_BUFF); 
	}
	return 0;
}
