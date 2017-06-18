/*********************************************************************
** Author: Philip Chang
** Date: May 16, 2017
** Description: CS475 - Project #4 - Functional Decomposition
** from http://web.engr.oregonstate.edu/~mjb/cs575/Projects/proj04.html
*********************************************************************/
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


int width = 10;
// using 4 threads - watcher - grain - deer - Something I pick - pest

//global variables for the system

int	NowYear;		// 2017 - 2022
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population

int pest; //pest - this is mine *****

const float GRAIN_GROWS_PER_MONTH =		8.0;
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				50.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

/************************************************
Units of grain growth are inches.
Units of temperature are degrees Fahrenheit (°F).
Units of precipitation are inches.

              cm = inches * 2.54
              °C = (5./9.)*(°F-32)
              convert later on pdf
*************************************************/
//given functions
unsigned int seedp = 0; // seed for ranf
//square root function
float SQR( float x )
{
        return x*x;
}
//float and int random function
float Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}

float ang(){
  return (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
}
float Temp(){
  float temp = AVG_TEMP - AMP_TEMP * cos( ang() );
  float temp2 = temp + Ranf( &seedp, -RANDOM_TEMP, RANDOM_TEMP );
  return temp2;

}
float Precip(){
  float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang() );
  float precip2 = precip + Ranf( &seedp,  -RANDOM_PRECIP, RANDOM_PRECIP );
  if( precip2 < 0. ){
  	precip2 = 0.;
  }
  return precip2;
}


//function prototypes
float SQR(float);
float Ranf(unsigned int *, float, float);
int Ranf(unsigned int *, int, int);
float Ang();
float Temp();//get the temperature
float Precip();//get the precipitation
void GrainDeer();
void Grain();
void Pest();
void Watcher();

int main( int argc, char *argv[ ] ){
  // starting date and time:
  NowMonth =    0;
  NowYear  = 2017;


  // starting state (feel free to change this if you want):
  NowNumDeer = 1;
  NowHeight =  1.;

  //starting state for temperature and precip
  NowTemp = AVG_TEMP;
  NowPrecip = AVG_PRECIP_PER_MONTH;

  omp_set_num_threads( 4 );	// same as # of sections


#pragma omp parallel sections
{
	#pragma omp section
	{
		GrainDeer( ); //for deers
	}

	#pragma omp section
	{
		Grain( ); //for grains
	}

	#pragma omp section
	{
    Pest( );	// your own i use pest
	}

	#pragma omp section
	{
		Watcher( ); //prints out and updates
	}
}       // implied barrier -- all functions must return in order
	// to allow any of them to get past here

  return 0;
}

void GrainDeer(){
   int graindeer;
   int kill;

   while( NowYear < 2023 ){
    int graindeer = 0;
    int kill = 0;

     if (NowNumDeer > NowHeight){
       kill++;
     }
     else{
       graindeer++;
     }
     // DoneComputing barrier:
 #pragma omp barrier
 NowNumDeer = NowNumDeer + graindeer - kill;
 if (NowNumDeer < 0){
   NowNumDeer = 0;
 }

 // DoneAssigning barrier:
 #pragma omp barrier


 // DonePrinting barrier:
 #pragma omp barrier
   }
}
void Grain(){
  float eat;
  srand(time(NULL));
  while( NowYear < 2023 ){
      float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
      float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
      if (NowHeight < 0){
        NowHeight = 0;
      }

    // DoneComputing barrier:
#pragma omp barrier

// DoneAssigning barrier:
if (pest == 1){
  NowHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
  NowHeight -= ((float)NowNumDeer * ONE_DEER_EATS_PER_MONTH + eat);
  NowHeight *= .8;
}
else{
  NowHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
  NowHeight -= ((float)NowNumDeer * ONE_DEER_EATS_PER_MONTH + eat);
}



if (NowHeight < 0){
  NowHeight = 0;
}
#pragma omp barrier

// DonePrinting barrier:
#pragma omp barrier
  }
}

void Pest(){
  while ( NowYear < 2023){
    int check;

//not sure about this but i assume if its hot and humid there are gonna be pests
    if(NowTemp > AVG_TEMP && NowPrecip > AVG_PRECIP_PER_MONTH){
      check = 1;
    }
    else{
      check = 0;
    }
    // DoneComputing barrier:
#pragma omp barrier
pest = check;
// DoneAssigning barrier:
#pragma omp barrier
// DonePrinting barrier:
#pragma omp barrier
  }
}

void Watcher(){

  int year;
  int month;
  int tprecip;
  int ttemp;

printf ("Year Month  Precip  Temp  Height  Deer  Pest\n\n");
  while (NowYear < 2023){

    // DoneComputing barrier:
#pragma omp barrier
// DoneAssigning barrier:

#pragma omp barrier
// DonePrinting barrier:
month = NowMonth + 1;
if (month > 11){
  month = 0;
  year = NowYear + 1;
}
else{
  year = NowYear;
}
tprecip = Precip();
ttemp = Temp();

NowYear = year;
NowMonth = month;
NowPrecip = tprecip;
NowTemp = ttemp;

//printf ("Year Month Precip  Temp  Height  Deer  Pest\n");
printf("%d %d %lf %lf %lf %d %d \n", NowYear, NowMonth, NowPrecip, NowTemp, NowHeight, NowNumDeer, pest);
#pragma omp barrier

  }

}
