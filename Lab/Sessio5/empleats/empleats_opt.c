#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOM 40
#define MAX_DIAS_MES 31 
#define MAX_CATEGORIA 40 
#define MAX_ADDRESS 100


#define EXTES


typedef struct 
{
  long long int NID;              /* Número d'identificació: clau d'ordenació */
  char Nom[MAX_NOM];  
  char Cognoms[MAX_NOM];  
  int  horesMes[MAX_DIAS_MES]; 
  char Categoria[MAX_CATEGORIA];
  unsigned int ptrClauForaneaDepartament;
  unsigned int ptrClauCategoria;
#ifdef EXTES     
  char Address[MAX_ADDRESS];
  char Nom_parella[MAX_NOM];
  char Cognoms_parella[MAX_NOM];
#endif
} Templeat;

//Nova estructura
typedef struct{
	long long NID;
	Templeat * empleat;
} Refempleat;

int compare(const void *p1, const void *p2)
{
    if (((Refempleat*) p1)->NID > ((Refempleat*) p2)->NID) return(1);
    else if (((Refempleat*) p1)->NID < ((Refempleat*) p2)->NID) return(-1);
    else return(0);

}

int main(int argc, char *argv[])
{
	Templeat *empleats;
	Refempleat *refempleats;	

	unsigned int N = 1000000, i;

	if (argc > 1) N = atoi(argv[1]);


	empleats = (Templeat *) malloc(N*sizeof(Templeat));
	//Conva estructura de dades
	refempleats = (Refempleat *) malloc(N*sizeof(Refempleat));
	
	if (empleats == NULL) { fprintf(stderr, "Out of memory\n"); exit(0); }
	memset(empleats, 0, N*sizeof(Templeat));

	/* Random generation */
	for (i=0; i<N; i++) {
		empleats[i].NID = rand();
		empleats[i].Nom[0] = 'A' + ((i>>3) + (i<<2)) & 0x15;
		empleats[i].Cognoms[0] = 'a' + ((i>>2) + (i<<3)) & 0x15;
		empleats[i].horesMes[i % MAX_DIAS_MES] = i;
		empleats[i].Categoria[i % MAX_CATEGORIA] = (i << 2) * i;
		empleats[i].ptrClauForaneaDepartament = i<<10 + i<<14 + i>>3;
		empleats[i].ptrClauCategoria = i<<24 + i<< 18 + i<<10 + i>>2;
#ifdef EXTES     
      	empleats[i].Address[0] = 'A' + ((i>>4) + (i<<2)) & 0x15;
       	empleats[i].Nom_parella[0] = 'a' + ((i>>5) + (i<<2)) & 0x15;
  		empleats[i].Cognoms_parella[0] = 'A' + ((i>>5) + (i<<3)) & 0x15;
#endif

		refempleats[i].NID = empleats[i].NID;
		refempleats[i].empleat = &empleats[i];
	}

	qsort(refempleats, N, sizeof(Refempleat), compare);
	for (i=0; i<N; i++){
	        write(1, refempleats[i].empleat,sizeof(Templeat));
	}

	return 0;
}
