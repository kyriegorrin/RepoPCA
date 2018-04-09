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

	//Getting the new constant value to add to d
	
	
	srand(0);
	for (i=0, it1=0; i<n; i++)
	{
		r = rand();
		for (j=0, d=0, it1=0; j<PUNTS; j++, it1+=2)
		{
			x = r*cos(d); y = r*sin(d);
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
