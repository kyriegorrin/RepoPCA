#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <papi.h>

#define NUM_COUNTERS 1

int N, N4;
signed char a[25480], b[25480], c[25480];

void DIVIDE( signed char *x, int n )                           
{                                                
    int j, k;
    unsigned q, r, u;
    long v;

    r = 0;                                       
    for( k = 0; k <= N4; k++ )                  
    {                                            
        u = r * 10 + x[k];                       
        q = u/n;                               
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
    for( k = N4; k >= 1; k-- )                   
    {                                            
        if( (x[k] = y[k] - z[k]) < 0 )           
        {                                        
            x[k] += 10;                          
            z[k-1]++;                            
        }                                        
    }                                            
    if( (x[k] = y[k] - z[k]) < 0 )           
    {                                        
        x[k] += 10;                          
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
    struct tms start, end;
	
	//OLD GARBAGE
    //if (times(&start) == (clock_t)-1) exit(0);
	
	//NEW PAPASITO DELUXE TIMING EDITION SETUP
	long long counters[NUM_COUNTERS];
	int PAPI_events[] = {PAPI_TOT_CYC}; 
	
	PAPI_library_init(PAPI_VER_CURRENT);
	int papi_counters = PAPI_num_counters();
	
	//Start counting type of events indicated by PAPI events
	int status = PAPI_start_counters(PAPI_events, NUM_COUNTERS);
	if(status!=PAPI_OK) exit(0);

	//CODE START

    N4 = N + 4;

    SET( a, 0 );
    SET( b, 0 );

    for( j = 2 * N4 + 1; j >= 3; j -= 2 )
    {
        SET( c, 1 );
        LONGDIV( c, j );

        SUBTRACT( a, c, a );
        DIVIDE( a, 25 );

        SUBTRACT( b, c, b );
        DIVIDE( b, 239 );
        DIVIDE( b, 239 );

        progress();
    }

    SET( c, 1 );

    SUBTRACT( a, c, a );
    DIVIDE( a, 5 );

    SUBTRACT( b, c, b );
    DIVIDE( b, 239 );

    MULTIPLY( a, 4 );
    SUBTRACT( a, a, b );
    MULTIPLY( a, 4 );

    progress();

	//CODE END
	status = PAPI_read_counters(counters, NUM_COUNTERS);
	if(status!=PAPI_OK) exit(0);
	printf("%lld cycles, %lld temps memes", counters[0], counters[0]/666);
	
	/*OLD JUNK*/
    //if (times(&end) == (clock_t)-1) exit(0);
    //fprintf(stderr, "\n Timing amb crida times: user %f segons, system: %f segons\n", (float)(end.tms_utime-start.tms_utime)/sysconf(_SC_CLK_TCK), (float)(end.tms_stime-start.tms_stime)/sysconf(_SC_CLK_TCK));
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

