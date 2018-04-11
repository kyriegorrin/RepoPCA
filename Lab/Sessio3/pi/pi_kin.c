#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

int N, N4;
char a[25480], b[25480], c[25480];
char string[100];

char DIV5x[256][5];
char DIV25x[256][25];
char DIV239x[256][239];

unsigned DIV5r[256][5];
unsigned DIV25r[256][25];
unsigned DIV239r[256][239];




//n = 5,25,239
/*
void DIVIDE( char *x, int n )                           
{                                                
    int j, k;
    unsigned q, r, u;
    long v;

    r = 0;                                       
    for( k = 0; k <= N4; k++ )                  
    {                                            
        u = r * 10 + x[k];                       
        q = u / n;                               
        r = u - q * n;                           
        x[k] = q;                                
    }                                           
}*/




void DIVIDE5( char *x)                           
{                                                
    int k;
    unsigned r, r_new;

    r = 0;                                       
    for( k = 0; k <= N4; k++ )                  
    {                                                                      
        r_new = DIV5r[x[k]][r];                           
        x[k] = DIV5x[x[k]][r];
	r = r_new;
    }                                           
}



void DIVIDE25( char *x )                           
{                                                
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



void DIVIDE239( char *x )                           
{                                                
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









void LONGDIV( char *x, int n )                          
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

void MULTIPLY( char *x, int n )                        
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

void SET( char *x, int n )                              
{                                                
    memset( x, 0, N4 + 1 );                      
    x[0] = n;                                    
}


void SUBTRACT( char *x, char *y, char *z )                      
{                                                
    int j, k;
    unsigned q, r, u;
    long v;
    for( k = N4; k >= 0; k-- )                   
    {                                            
        if( (x[k] = y[k] - z[k]) < 0 )           
        {                                        
            x[k] += 10;                          
            z[k-1]++;                            
        }                                        
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

    calculate();

    epilog();

    return 0;
}

void calculate( void )
{
    int j;

    N4 = N + 4;
    
    int k,kk;
    unsigned q, r, u;
    
    for (k = 0; k < 256; k++){
      for (kk = 0; kk < 5; kk++){
	u = kk * 10 + k;                       
        q = u / 5;                               
        DIV5r[k][kk] = u - q * 5;                           
        DIV5x[k][kk] = q;  
      }
    }
    
    for (k = 0; k < 256; k++){
      for (kk = 0; kk < 25; kk++){
	u = kk * 10 + k;                       
        q = u / 25;                               
        DIV25r[k][kk] = u - q * 25;                           
        DIV25x[k][kk] = q;  
      }
    }   
    
    for (k = 0; k < 256; k++){
      for (kk = 0; kk < 239; kk++){
	u = kk * 10 + k;                       
        q = u / 239;                               
        DIV239r[k][kk] = u - q * 239;                           
        DIV239x[k][kk] = q;  
      }
    }
    

    SET( a, 0 );
    SET( b, 0 );

    for( j = 2 * N4 + 1; j >= 3; j -= 2 )
    {
        SET( c, 1 );
        LONGDIV( c, j );

        SUBTRACT( a, c, a );
        DIVIDE25( a );

        SUBTRACT( b, c, b );
        DIVIDE239( b );
        DIVIDE239( b );

        progress();
    }

    SET( c, 1 );

    SUBTRACT( a, c, a );
    DIVIDE5( a );

    SUBTRACT( b, c, b );
    DIVIDE239( b );

    MULTIPLY( a, 4 );
    SUBTRACT( a, a, b );
    MULTIPLY( a, 4 );

    progress();
}

/*

 N = 10000                      
 A = 0                          
 B = 0                          
 J = 2 * (N + 4) + 1            
 FOR J = J TO 3 STEP -2         
     A = (1 / J - A) / 5 ^ 2    
     B = (1 / J - B) / 239 ^ 2  
 NEXT J                         
 A = (1 - A) / 5                
 B = (1 - B) / 239              
 PI = (A * 4 - B) * 4           

*/

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

