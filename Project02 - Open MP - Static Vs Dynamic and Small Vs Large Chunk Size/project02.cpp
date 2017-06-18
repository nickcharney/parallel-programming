/*********************************************************************
** Author: Philip Chang
** Date: April 31, 2017 / May 1, 2017
** Description: CS475 - Project #2 - Static Vs Dynamic and Small Vs Large Chunksize
** from http://web.engr.oregonstate.edu/~mjb/cs575/Projects/proj02.html
*********************************************************************/
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define ARRAYSIZE 32 * 1024
#define CHUNKSIZE 1 // 1 or 4096

//global array
float ARRAY[ARRAYSIZE];

#define NUMTRIES 10 //number of tries used to get average performance
#define NUMT 3 // 1 | 2 | 4 | 6 | 8 | 10 | 12 | 14 | 16 threads
//random number function
float Ranf(float low, float high )
{
	float r = (float) rand();		// 0 - RAND_MAX

	return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

float Ranf(float, float);


int main( int argc, char *argv[ ] )
{
	srand(time(NULL));
#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
	return 1;
#endif
  omp_set_num_threads(NUMT);
	double maxMegaMults = 0.;
	double sumMegaMults = 0.;
  //seed
  unsigned int seed = 0;
  //filling the array
  for (int i = 0; i < ARRAYSIZE - 1; i++){
    ARRAY[i] = Ranf(-1.f, 1.f );
  }
//get average and peak performance
	for (int t = 0; t < NUMTRIES; t++){
		double time0 = omp_get_wtime();

//static, 1 ; static, 4096; dynamic 1; dyanmic, 4096

	double product;
#pragma omp parallel for default(none), private(product), schedule(static,CHUNKSIZE), shared(ARRAY)
	for (int i = 0; i< ARRAYSIZE -1; i++){
		 product = 1;
		for (int j = 0; j < i; j++){
			product *= ARRAY[j];
		}
	}
	double time1 = omp_get_wtime();
	long int numMuled = (long int)ARRAYSIZE * (long int)(ARRAYSIZE+1) / 2;
	double megaMults = numMuled / (time1 - time0) / 1000000;
	sumMegaMults += megaMults;
	if (megaMults > maxMegaMults)
		maxMegaMults = megaMults;
	}
//change for static or dynamic
	double avgMegaMults = sumMegaMults / double(NUMTRIES);
	fprintf( stderr, "Threads = %2d; ChunkSize = %5d; Scheduling=static ; Average MegaMults/sec = %10.2lf ; Peak MegaMults = %10.2lf\n",
	NUMT, CHUNKSIZE, avgMegaMults, maxMegaMults );


  return 0;
}
