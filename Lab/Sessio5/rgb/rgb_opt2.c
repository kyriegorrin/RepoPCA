#include <stdio.h>
#include <stdlib.h>


typedef struct {
   char r;
   char g;
   char b;
} Tpixel;

#define ROWS 5000
#define COLUMNS 20000

Tpixel buffer[ROWS][COLUMNS];

void rgb_verd()
{
  unsigned int i,j;
	j = 0;

	int * buffo = (int *)buffer;
	int* buffoFinal = &buffer+1;
	
	while(buffo < buffoFinal-2){
		*buffo = 0x00000100; buffo++;
		*buffo = 0x01000001; buffo++;
		*buffo = 0x00010000; buffo++;
		j+=3;
	}
	
  //Epileg
  for(; &buffer[ROWS][j] < buffoFinal; ++j){
  	buffer[ROWS][j].r = 0;
		buffer[ROWS][j].g = 1;
		buffer[ROWS][j].b = 0;
  }
}

int main(int argc, char *argv[])
{
  rgb_verd();

  write(1, buffer, sizeof(buffer));

  return(0);
}

