#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>

#define N 16000000

typedef struct s_pixel_rgb {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} t_pixel_rgb;


void increase_brightness (t_pixel_rgb *rgb, int len, unsigned char inc, int N_iter)
{
	int i,j,k;
	unsigned char * rgbchar = (unsigned char *) rgb; 

	/*
     for (j=0; j<N_iter; j++){
		for (i=0; i<len; i++) {
			if ((rgb[i].r + inc) > 255) {
				rgb[i].r = 255;
			} else {
				rgb[i].r += inc;
			}

			if ((rgb[i].g + inc) > 255) {
				rgb[i].g = 255;
			} else {
				rgb[i].g += inc;
			}

			if ((rgb[i].b + inc) > 255) {
				rgb[i].b = 255;
			} else {
				rgb[i].b += inc;
			}		
		}
	}
	*/

	//Setup de vectors
	uint8x16_t vRGB1, vRGB2, vRGB3, vInc;

	//Setejar increment
	vInc = vmovq_n_u8((uint8_t) inc);

	//Optimized to the max version vectorial vectors very vector indeed version
    for (j=0; j<N_iter; j++){
		for(i = 0, k = 0; i < len - 47; i+=48, k+=16){
			vRGB1 = vld1q_u8((uint8_t *) &rgbchar[i]);
			vRGB2 = vld1q_u8((uint8_t *) &rgbchar[i+16]);
			vRGB3 = vld1q_u8((uint8_t *) &rgbchar[i+32]);

			vRGB1 = vqaddq_u8(vRGB1, vInc);
			vRGB2 = vqaddq_u8(vRGB2, vInc);
			vRGB3 = vqaddq_u8(vRGB3, vInc);

			vst1q_u8((uint8_t *)&rgbchar[i], vRGB1);
			vst1q_u8((uint8_t *)&rgbchar[i+16], vRGB2);
			vst1q_u8((uint8_t *)&rgbchar[i+32], vRGB3);
		}
		//Epíleg mortal fantasmagòric
		for(; k < len; ++k){
			if ((rgb[k].r + inc) > 255) {
				rgb[k].r = 255;
			} else {
				rgb[k].r += inc;
			}

			if ((rgb[k].g + inc) > 255) {
				rgb[k].g = 255;
			} else {
				rgb[k].g += inc;
			}

			if ((rgb[k].b + inc) > 255) {
				rgb[k].b = 255;
			} else {
				rgb[k].b += inc;
			}		
		}
	}
}

int main (int argc, char *argv[])
{

  t_pixel_rgb *A;
  int i;
  int n=N; // tamanyo vectores
  int NIt=50; // Num.iters
  unsigned char incr=1;

  if (argc>1) NIt = atoi(argv[1]);
  if (argc>2) n = atoi(argv[2]);
  if (argc>3) incr = (unsigned char) atoi(argv[3]);

  if (n>N) { printf("Maxima longitud vector: %d\n",N); exit(-1); }

  if (posix_memalign((void**)&A, 16, n*sizeof(t_pixel_rgb)) != 0) {
      fprintf(stderr, "No hay memoria.\n");
      exit(-1);
  }

   /* Inicialitzacio nombres "aleatoris" */
   for (i=0;i<n;i++) {
     A[i].r= i^(i-54) & 0x2f;
     A[i].g= (i+89)^(i-200) & 0x2f;
     A[i].b= (i+70)^(i+325) &0x2f;
   }

   increase_brightness (A, n, incr, NIt); 

   write(1, A, n*sizeof(t_pixel_rgb));

   return(0);
}
