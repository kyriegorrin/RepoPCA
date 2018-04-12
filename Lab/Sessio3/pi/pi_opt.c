#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

int N, N4;
signed char a[25480], b[25480], c[25480];
signed char string[100];

//FIRST VERSION DEPRECATED
//unsigned r10[239]; //Aquest nomes guarda r*10
//unsigned q25[250];
//unsigned q239[2400];
//unsigned r25[10];
//unsigned r239[11];

char DIV25x[10][25];
char DIV239x[10][239];

unsigned DIV25r[10][25];
unsigned DIV239r[10][239];

void memo(){
		/* FIRST VERSION DEPRECATED
	int i;
	for(i = 0; i < 239; ++i){
		r10[i] = i*10;
	}
		
	for(i = 0; i < 250; ++i){
		q25[i] = i/25;
	}
	
	for(i = 0; i < 2400; ++i){
		q239[i] = i/239;
	}

	for(i = 0; i < 10; ++i){
		r25[i] = i*25;
	}

	for(i = 0; i < 11; ++i){
		r239[i] = i*239;
	}*/

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
/* DEPRECATED
    int j, k;
    unsigned q, r, u;
    long v;

    r = 0;
    for(k = 0; k <= N4; k++)                  
    {                                            
        u = r10[r] + x[k]; 
        x[k] = q25[u];                               
        r = u - r25[x[k]];                           
    }                                           
*/
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
/* DEPRECATED
    int j, k;
    unsigned q, r, u;
    long v;

    r = 0;                                       
    for(k = 0; k <= N4; k++)                  
    {                                            
        u = r10[r] + x[k]; 
        x[k] = q239[u];                               
        r = u - r239[x[k]];                           
    }                                           
*/
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

