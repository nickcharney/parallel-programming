/*********************************************************************
** Author: Philip Chang
** Date: April 31, 2017 / May 1, 2017
** Description: CS475 - Project #3 - False Sharing
** from http://web.engr.oregonstate.edu/~mjb/cs575/Projects/proj03.html
*********************************************************************/
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NUMT 4 // 1, 2, 3, 4
#define NUMPAD 15 // 0-15

void fix1(){
srand(time(NULL));
struct s
{
	float value;
	int pad[NUMPAD];
} Array[4];


	omp_set_num_threads( NUMT );

	const int SomeBigNumber = 100000000;	// keep < 2B

	double time0 = omp_get_wtime();

	#pragma omp parallel for
	for( int i = 0; i < 4; i++ )
	{
		for( unsigned int j = 0; j < SomeBigNumber; j++ )
		{
			Array[ i ].value = Array[ i ].value + (float)rand();
		}
	}
	double time1 = omp_get_wtime();
	double uTime = time1 - time0; //time used
	double megaPlus = (double)(SomeBigNumber * 4) / uTime / 1000000;

	printf("FIX #1 - Threads = %d - Padding = %d - MegaPlus/Sec = %f \n", NUMT, NUMPAD, megaPlus);
}

void fix2(){
srand(time(NULL));

struct s
{
	float value;
} Array[4];

	omp_set_num_threads( NUMT );
	const int SomeBigNumber = 100000000;	// keep < 2B
	double time0 = omp_get_wtime();
	#pragma omp parallel for
	for( int i = 0; i < 4; i++ )
	{
		float tmp = Array[i].value;
		for( unsigned int j = 0; j < SomeBigNumber; j++ )
		{
			tmp = tmp + (float)rand();
		}
		Array[i].value = tmp;
	}
	double time1 = omp_get_wtime();
	double uTime = time1 - time0; //time used
	double megaPlus = (double)(SomeBigNumber * 4) / uTime / 1000000;

	printf("FIX #2 - Threads = %d - MegaPlus/Sec = %f \n", NUMT, megaPlus);
}
void fix1();
void fix2();

int main( int argc, char *argv[ ] ){
#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
	return 1;
#endif

	fix1();//NUMPAD fix1
	//fix2();//private variable fix 2

  return 0;
}
