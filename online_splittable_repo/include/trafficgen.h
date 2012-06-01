#ifndef LOAD_h
#define LOAD_h

#include <vector>
/**@file trafficgen.h*/

/** Traffic generator. Generate a uniformly distributed traffic load in a given range.
 * The maximum is set to the architecture capacity*/
class trafficgen{
	private:
		double lLoad;			/**<current load*/
	public:
		/**constructor*/
		trafficgen();
		/**destructor*/
		~trafficgen();

		/**sets and return a new load
		 * @param min the minimum traffic = 0
		 * @param max the maximum traffic = the maximum architecture capacity
		 * @param seed to seed the random generator
		 * @return lLoad current trafic load
		 */
		double setLoad(double, double, long *);
};
/**@fn long rnd32(long seed).
 * @brief Congruential generator of pseudorandom sequences of numbers
 * uniformly distributed between 1 and 2147483646, using the congruential relation:
 * Xn+1 = 16807 * Xn  mod  2147483647 .
 * The input to the routine is the integer Xn, while the returned integer is the number Xn+1.
 * @param seed to seed the random generator
 */
long rnd32(long );
/**@fn double uniform(double a, double b, long *seed)
 * @param a minimum of the range
 * @param b maximum of the range
 * @param seed to seed the random generator
 * @return a value uniformly distributed between 'a' and 'b'
 */
double uniform(double , double , long *);

#endif //LOAD_h
