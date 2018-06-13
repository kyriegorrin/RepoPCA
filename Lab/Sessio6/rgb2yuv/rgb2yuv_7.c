#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <arm_neon.h>

#define N 4000000
/* #define DEBUG 0 */

#define UNROLL_RGB(_i){\
	yuv->y[_i] = 0.299 * rgb->r[_i] + 0.587 * rgb->g[_i] + 0.114 * rgb->b[_i];\
	yuv->u[_i] = -0.147 * rgb->r[_i] - 0.289 * rgb->g[_i] + 0.436 * rgb->b[_i];\
	yuv->v[_i] = 0.615 * rgb->r[_i] - 0.515 * rgb->g[_i] - 0.100 * rgb->b[_i];}

typedef struct s_pixel_rgb {
	float *r;
	float *g;
	float *b;
} t_pixel_rgb;

typedef struct s_pixel_yuv {
	float *y;
	float *u;
	float *v;
} t_pixel_yuv;

void rgb_to_yuv(t_pixel_rgb *rgb, t_pixel_yuv *yuv, int len, int N_iter)
{
	int i,j;

	//Declaracio de merdes auxiliars necessaries
	float32x4_t vR, vG, vB, vY, vU, vV, vAux;	

	//Matrix per vector product version
 	for (j=0; j<N_iter; j++){
		for (i=0; i < len - 3; i+=4) {

			//Carreguem els valors RGB
			vR = vld1q_f32(&rgb->r[i]);
			vG = vld1q_f32(&rgb->g[i]);
			vB = vld1q_f32(&rgb->b[i]);
			
			//Fem les operacions necessaries
			//y[i]---y[i+3]
			vY = vmulq_n_f32(vR, 0.299);
			vAux = vmulq_n_f32(vG, 0.587);
			vY = vaddq_f32(vY, vAux);
			vAux = vmulq_n_f32(vB, 0.114);
			vY = vaddq_f32(vY, vAux);
						
			//Fem les operacions necessaries
			//u[i]---u[i+3]
			vU = vmulq_n_f32(vR, -0.147);
			vAux = vmulq_n_f32(vG, -0.289);
			vU = vaddq_f32(vU, vAux);
			vAux = vmulq_n_f32(vB, 0.436);
			vU = vaddq_f32(vU, vAux);

			//Fem les operacions necessaries
			//v[i]---v[i+3]
			vV = vmulq_n_f32(vR, 0.615);
			vAux = vmulq_n_f32(vG, -0.515);
			vV = vaddq_f32(vV, vAux);
			vAux = vmulq_n_f32(vB, -0.100);
			vV = vaddq_f32(vV, vAux);

			//Carreguem valors a memoria de nou titu
			vst1q_f32(&yuv->y[i], vY);	
			vst1q_f32(&yuv->u[i], vU);	
			vst1q_f32(&yuv->v[i], vV);	
		}
		for(; i < len; ++i){
			UNROLL_RGB(i);
		}
	}
}

int main (int argc, char *argv[])
{
  t_pixel_rgb rgb; t_pixel_yuv yuv; // Vectores para malloc

  int i;
  int n=N; // tamanyo vectores
  int NIt=50; // Num.iters

  if (argc>1) NIt = atoi(argv[1]);
  if (argc>2) n = atoi(argv[2]);

  if (n>N) { printf("Maxima longitud vector: %d\n",N); exit(-1); }

  //-----------RESERVA RGB------------------//
  if (posix_memalign((void**)&rgb.r, 16, n*sizeof(float)) !=0) {
      fprintf(stderr, "No memory.\n");
      exit(-1);
  }
  if (posix_memalign((void**)&rgb.g, 16, n*sizeof(float)) !=0) {
      fprintf(stderr, "No memory.\n");
      exit(-1);
  }		
  if (posix_memalign((void**)&rgb.b, 16, n*sizeof(float)) !=0) {
      fprintf(stderr, "No memory.\n");
      exit(-1);
  }
  
  //--------RESERVA YUV---------------------//
  if (posix_memalign((void**)&yuv.y, 16, n*sizeof(float)) !=0) {
      fprintf(stderr, "No memory.\n");
      exit(-1);
  }
  if (posix_memalign((void**)&yuv.u, 16, n*sizeof(float)) !=0) {
      fprintf(stderr, "No memory.\n");
      exit(-1);
  }
  if (posix_memalign((void**)&yuv.v, 16, n*sizeof(float)) !=0) {
      fprintf(stderr, "No memory.\n");
      exit(-1);
  }
 
  //---------------INICIALITZAR ESTRUCTURES-------//
  for (i=0;i<n;i++) {
     rgb.r[i]=(float) ((i<<26) ^ ((i + 340) << 22) ^ i ^ 0xf215fabc);
     rgb.g[i]=(float) (((i+450)<<27) ^ ((i + 567) <<20) ^ i ^ 0xb152e8ca);
     rgb.b[i]=(float) (((i+7823454) << 25) ^0xbad51cde);
  }

  rgb_to_yuv(&rgb, &yuv, n, NIt);
 
  //TODO: Arreglar les escriptures
  write(1, yuv.y, n*sizeof(float));
  write(1, yuv.u, n*sizeof(float));
  write(1, yuv.v, n*sizeof(float));
  
  return(0);
}
