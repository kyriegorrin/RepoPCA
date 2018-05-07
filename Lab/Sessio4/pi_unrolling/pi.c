#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

//Definició de macros trambòliques de unrolling
#define UNROLL_SUB(_k)				\
 	if( (x[_k] = y[_k] - z[_k]) < 0 ){ \
    	x[_k] += 10;                 \
        z[_k-1]++;}                  \
			
#define UNROLL_DIV25(_k)		\
        r_new = DIV25r[x[_k]][r];       \
        x[_k] = DIV25x[x[_k]][r];	\
	r = r_new;

#define UNROLL_DIV239(_k)		\
        r_new = DIV239r[x[_k]][r];      \
        x[_k] = DIV239x[x[_k]][r];	\
	r = r_new;

#define UNROLL_LONGDIV_IF(_k)		\
            u = r * 10 + x[_k];		\
            q = u / n;                  \
            r = u - q * n;              \
            x[_k] = q;                    
        
int N, N4;
signed char a[25480], b[25480], c[25480];
signed char string[100];

char DIV25x[10][25];
char DIV239x[10][239];

unsigned DIV25r[10][25];
unsigned DIV239r[10][239];

int unrollFactor = 12;

void memo(){

    int i, j;
    unsigned q, r, u;
    
    for (i = 0; i < 10; i++){
      for (j = 0; j < 25; j++){
	u = j * 10 + i;                       
        q = u / 25;                               
        DIV25r[i][j] = u - q * 25;                           
        DIV25x[i][j] = q;  
      }
    }   
    
    for (i = 0; i < 10; i++){
      for (j = 0; j < 239; j++){
	u = j * 10 + i;                       
        q = u / 239;                               
        DIV239r[i][j] = u - q * 239;                           
        DIV239x[i][j] = q;  
      }
    }
}

void DIVIDE25( signed char *x){
    int k;
    unsigned r, r_new;

    r = 0;                                       
    for( k = 0; k <= N4; k++ )                  
    {                                                                      
        r_new = DIV25r[x[k]][r];
        x[k] = DIV25x[x[k]][r];
	r = r_new;
    }                          
}

void DIVIDE239( signed char *x){
    int k;
    unsigned r, r_new;

    r = 0;                                       
    for( k = 0; k <= N4; k++ )                  
    {                                                                      
        r_new = DIV239r[x[k]][r];
        x[k] = DIV239x[x[k]][r];
	r = r_new;
    }                          
}

void DIVIDE( signed char *x, int n )                           
{                                                
    int j, k;
    unsigned q, r, u;
    long v;

    r = 0;                                       
    for(k = 0; k <= N4; k++)                  
    {                                            
        u = r * 10 + x[k]; 
        q = u / n;                               
        r = u - q * n;                           
        x[k] = q;                                
    }                                           
}

void LONGDIV( signed char *x, int n )                          
{                                                
    int j, k;
    unsigned q, r, u;
    long v;

    if( n < 6553 )                               
    {                                            
        r = 0;                                   
        for( k = 0; k <= N4; k++ )               
        {                                        
            u = r * 10 + x[k];		
            q = u / n;                  
            r = u - q * n;              
            x[k] = q;                    
        }
    }                                            
    else                                         
    {                                            
        r = 0;                                   
        for( k = 0; k <= N4; k++ )              
        {                                       
            if( r < 6553 )                      
            {                                   
                u = r * 10 + x[k];              
                q = u / n;                      
                r = u - q * n;                  
            }                                   
            else                                
            {                                   
                v = (long) r * 10 + x[k];       
                q = v / n;                      
                r = v - q * n;                  
            }                                   
            x[k] = q;                           
        }                                       
    }                                           
}

void MULTIPLY( signed char *x, int n )                        
{                                            
    int j, k;
    unsigned q, r, u;
    long v;
    r = 0;                                   
    for( k = N4; k >= 0; k-- )               
    {                                        
        q = n * x[k] + r;                    
        r = q / 10;                          
        x[k] = q - r * 10;                   
    }                                        
}

void SET( signed char *x, int n )                              
{                                                
    memset( x, 0, N4 + 1 );                      
    x[0] = n;                                    
}


void SUBTRACT( signed char *x, signed char *y, signed char *z )                      
{                                                
    int j, k;
    unsigned q, r, u;
    long v;
    for( k = N4; k >= unrollFactor; k-= unrollFactor )                   
    {                                            
		UNROLL_SUB(k);
		UNROLL_SUB(k-1);
		UNROLL_SUB(k-2);
		UNROLL_SUB(k-3);
		UNROLL_SUB(k-4);
		UNROLL_SUB(k-5);
		UNROLL_SUB(k-6);
		UNROLL_SUB(k-7);
		UNROLL_SUB(k-8);
		UNROLL_SUB(k-9);
		UNROLL_SUB(k-10);
		UNROLL_SUB(k-11);
    }

	k = N4%unrollFactor;
	for(; k >= 0; --k){
		UNROLL_SUB(k);
	}                                             
}


void calculate( void );
void progress( void );
void epilog( void );


int main( int argc, char *argv[] )
{
    N = 10000;

    if( argc > 1 )
        N = atoi(argv[1]);

    setbuf(stdout, NULL);

	//Insertem operacions de memorització
	memo();

    calculate();

    epilog();

    return 0;
}

void calculate( void )
{
    int j;

    N4 = N + 4;

    SET( a, 0 );
    SET( b, 0 );

    for( j = 2 * N4 + 1; j >= 3; j -= 2 )
    {
        SET( c, 1 );
        LONGDIV( c, j );

        SUBTRACT( a, c, a );
        DIVIDE25(a);

        SUBTRACT( b, c, b );
        DIVIDE239(b);
        DIVIDE239(b);

        progress();
    }

    SET( c, 1 );

    SUBTRACT( a, c, a );
    DIVIDE( a, 5 );

    SUBTRACT( b, c, b );
    DIVIDE239(b);

    MULTIPLY( a, 4 );
    SUBTRACT( a, a, b );
    MULTIPLY( a, 4 );

    progress();
}

void progress( void )
{
    printf(".");
}

void epilog( void )
{
    int j;

    {
        fprintf( stdout, " \n3.");
        for( j = 1; j <= N; j++ )
        {
            fprintf( stdout, "%d", a[j]);
            if( j % 5  == 0 )
                if( j % 50 == 0 )
                    if( j % 250  == 0 )
                        fprintf( stdout, "    <%d>\n\n   ", j );
                    else
                        fprintf( stdout, "\n   " );
                else
                    fprintf( stdout, " " );
        }
    }
}

