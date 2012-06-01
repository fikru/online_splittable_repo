#include "trafficgen.h"
#include "global.h"
#include <stdio.h>
#include <math.h>

#define MODULE  2147483647
#define A       16807
#define LASTXN  127773
#define UPTOMOD -2836
#define RATIO   0.46566128e-9            /* 1/MODULE */

trafficgen::trafficgen(){}

trafficgen::~trafficgen(){}

/*
 * uniformly distributed random traffic generator in the range min and max
 * return the load
 */
double trafficgen::setLoad(double min, double max, long *seed){
	lLoad = uniform(min, max, seed);
	mssr_stat.setLoadList(lLoad);
	return lLoad;
}

/*
**  Function: long rnd32(long seed)
**  Remarks : Congruential generator of pseudorandom sequences of numbers
**            uniformly distributed between 1 and 2147483646, using the con-
**            gruential relation: Xn+1 = 16807 * Xn  mod  2147483647 .
**            The input to the routine is the integer Xn, while the returned
**            integer is the number Xn+1.
*/
long rnd32(long seed)
{
 long times, rest, prod1, prod2;

 times = seed / LASTXN;
 rest  = seed - times * LASTXN;
 prod1 = times * UPTOMOD;
 prod2 = rest * A;
 seed  = prod1 + prod2;
 if (seed < 0) seed = seed + MODULE;
 return (seed);
}


/*
**  Function: double uniform(double a, double b, long *seed)
**  Return  : a value uniformly distributed between 'a' and 'b'
**  Remarks : The value of '*seed' is changed
*/
double uniform(double a, double b, long *seed)
{
 double u;
 *seed = rnd32 (*seed);
 u = (*seed) * RATIO;
 u = a + u * (b-a);
 return (u);
}
