/********************************************************************************************
** Author: Philip Chang
** Date: May 23, 2017
** Description: CS475 - Project #5 - Vectorized Array Multiplication and Reduction using SSE
** from http://web.engr.oregonstate.edu/~mjb/cs575/Projects/proj05.html
*********************************************************************************************/
#include "simd.p5.h"
//number of tries - check average with peak within 20% of eachother
#define NUMTRIES 10


//my own array multipulcation code
float array_mul( float *a, float *b,   float *c,   int len ){
  for (int i = 0; i < len; i++){
    c[i] = a[i] * b[i];
  }
}
//my own array multiplication + reduction code
float array_mulsum( float *a, float *b, int len ){
  float sum = 0;
  for (int i = 0; i < len; i++){
    sum += a[i] * b[i];
  }
  return sum;
}


float Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}
float arrayfill(float *a, float *b){
  //fill array with stuff
  for (int i = 0; i < ARRAYSIZE - 1; i++){
    unsigned int seed = 0;
    a[i] = Ranf( &seed, -1.f, 1.f );
    b[i] = Ranf( &seed, -1.f, 1.f );
  }
}
//prototypes
float array_mul(    float *, float *,  float *, int );
float array_mulsum( float *, float *, int );
float Ranf( unsigned int *,  float, float );
float arrayfill(float *, float*);



int main( int argc, char *argv[ ] ){
  #ifndef _OPENMP
  	fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
  	return 1;
  #endif

  printf("ARRAYSIZE = %d\n", ARRAYSIZE);

//case 1 : SimdMul
//case 2: array_mul
//case 3: SimdMulSum
//case 4: array_mulsum

//SimdMUL
double maxMegaOps = 0.;
double sumMegaOps = 0.;


for( int t = 0; t < NUMTRIES; t++ )
{
        float *A = new float[ARRAYSIZE];
        float *B = new float[ARRAYSIZE];
        float *C = new float[ARRAYSIZE];
        arrayfill(A,B);
        double time0 = omp_get_wtime( );
        SimdMul(A, B, C, ARRAYSIZE);
        double time1 = omp_get_wtime( );
        double megaOps = (double)ARRAYSIZE/(time1-time0)/1000000.;
        sumMegaOps += megaOps;
        if( megaOps > maxMegaOps )
                maxMegaOps = megaOps;
}

double avgMegaOps = sumMegaOps/(double)NUMTRIES;
printf( "SimdMUL Peak Performance = %f Average Performance = %f\n", maxMegaOps, avgMegaOps);

//array_mul
maxMegaOps = 0.;
sumMegaOps = 0.;


for( int t = 0; t < NUMTRIES; t++ )
{
        float *A = new float[ARRAYSIZE];
        float *B = new float[ARRAYSIZE];
        float *C = new float[ARRAYSIZE];
        arrayfill(A,B);
        double time0 = omp_get_wtime( );
        array_mul(A, B, C, ARRAYSIZE);
        double time1 = omp_get_wtime( );
        double megaOps = (double)ARRAYSIZE/(time1-time0)/1000000.;
        sumMegaOps += megaOps;
        if( megaOps > maxMegaOps )
                maxMegaOps = megaOps;
}

avgMegaOps = sumMegaOps/(double)NUMTRIES;
printf( "array_mul Peak Performance = %f Average Performance = %f\n", maxMegaOps, avgMegaOps);

//SimdMulSum
maxMegaOps = 0.;
sumMegaOps = 0.;


for( int t = 0; t < NUMTRIES; t++ )
{
        float *A = new float[ARRAYSIZE];
        float *B = new float[ARRAYSIZE];
        float *C = new float[ARRAYSIZE];
        arrayfill(A,B);
        double time0 = omp_get_wtime( );
        float sum = SimdMulSum(A, B, ARRAYSIZE);
        double time1 = omp_get_wtime( );
        double megaOps = (double)ARRAYSIZE/(time1-time0)/1000000.;
        sumMegaOps += megaOps;
        if( megaOps > maxMegaOps )
                maxMegaOps = megaOps;
}

avgMegaOps = sumMegaOps/(double)NUMTRIES;
printf( "SimdMulSum Peak Performance = %f Average Performance = %f\n", maxMegaOps, avgMegaOps);

//array_mulsum
maxMegaOps = 0.;
sumMegaOps = 0.;


for( int t = 0; t < NUMTRIES; t++ )
{
        float *A = new float[ARRAYSIZE];
        float *B = new float[ARRAYSIZE];
        float *C = new float[ARRAYSIZE];
        arrayfill(A,B);
        double time0 = omp_get_wtime( );
        float sum = array_mulsum(A, B, ARRAYSIZE);
        double time1 = omp_get_wtime( );
        double megaOps = (double)ARRAYSIZE/(time1-time0)/1000000.;
        sumMegaOps += megaOps;
        if( megaOps > maxMegaOps )
                maxMegaOps = megaOps;
}

avgMegaOps = sumMegaOps/(double)NUMTRIES;
printf( "array_mulsum Peak Performance = %f Average Performance = %f\n", maxMegaOps, avgMegaOps);


return 0;
}
