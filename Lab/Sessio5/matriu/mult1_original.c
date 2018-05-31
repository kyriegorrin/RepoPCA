#include <stdio.h>
#include <stdlib.h>

#define N 512

int n;

int C[N][N];
int A[N][N];
int B[N][N];


void mult1(int C[][N], int B[][N], int A[][N], int n)
{

  int i,j,k;
  int sum;

  for ( i=0 ; i < n; i++ )
  {
    for ( j=0 ; j < n; j++ )
    {
      for ( k=0 ; k < n ; k++ )
         C[i][j]+= A[i][k]*B[k][j];
    }
  }
}

void init(int A[][N], int n)
{

  int i,j;

  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      A[i][j] = i+j;

}

int main(int argc, char *argv[])
{


  if (argc > 1) n = atoi(argv[1]); else n = N;

  init( A, n);
  init( B, n);
  init( C, n);
  

  mult1(C,B,A,n); 

  write(1, C, sizeof(C));
  return 0;
}
