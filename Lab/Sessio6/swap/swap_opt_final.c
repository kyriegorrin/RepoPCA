#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <arm_neon.h>
 
#define TAM_BUFFER 128
#define IT_BUFFER 64

void panic(char *miss)
{
        if (errno != 0) perror(miss);
        else fprintf(stderr, "%s\n", miss);
 
        exit(1);
}
 
int main(int argc, char *argv[])
{
    char c[2048] __attribute__ ((__aligned__(16)));
	char aux;
    int n1, n2;
	int i;
	uint8x16_t vectorA, vectorB;	
 
    while (((n1=read(0, &c, TAM_BUFFER)) > 0))
    {
		for(i = 0; i < n1-15; i += 16){
			vectorA = vld1q_u8((uint8_t *) &c[i]);
			vectorB = vrev16q_u8(vectorA);
			vst1q_u8((uint8_t *) &c[i], vectorB);
		}
		
		//Epileg
		for(; i < n1-1; i += 2){
			aux = c[i];
			c[i] = c[i+1];
			c[i+1] = aux;
		}

		if (write(1, &c, n1) < 0) panic("write1");
    }
 
 
    return 0;
}
