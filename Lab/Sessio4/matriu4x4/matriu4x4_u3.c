#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N_ITER 10000000

//Macro for inner loop unrolling
#define unrollK()\
 	C[i][j] = C[i][j] + A[i][k] * B[k][j];\
        C[i][j] = C[i][j] + A[i][k+1] * B[k+1][j];\
        C[i][j] = C[i][j] + A[i][k+2] * B[k+2][j];\
        C[i][j] = C[i][j] + A[i][k+3] * B[k+3][j];

#define unrollJ()\
	unrollK(); j++;\
	unrollK(); j++;\
	unrollK(); j++;\
	unrollK(); j=0;

#define unrollI()\
	unrollJ(); i++;\
	unrollJ(); i++;\
	unrollJ(); i++;\
	unrollJ(); i=0;

inline void multiplica(int A[4][4], int B[4][4], int C[4][4], unsigned int n_iter) __attribute__((always_inline));
inline void multiplica(int A[4][4], int B[4][4], int C[4][4], unsigned int n_iter) {
   int iter;
   int i,j,k;

   for (iter=0; iter<n_iter; iter++){
   	unrollI();
   }
}

void print_matriu(int C[][4])
{
   int i,j,k;

   for (i = 0; i < 4; i++) 
   {
     for (j = 0; j < 4; j++) 
     {
       printf("%d ", C[i][j]);
     }
     printf("\n");
   }
}

int main(int argc, char *argv[])
{
    int A[4][4], B[4][4], C[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    unsigned int n_iter=N_ITER, i,j;

    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
            A[i][j] = rand();
            B[i][j] = rand();
        }
    }

    if (argc > 1) {
	n_iter = atoi(argv[1]); 
    }

    multiplica(A, B, C, n_iter);

    print_matriu(C);
         
    return 0;
}
