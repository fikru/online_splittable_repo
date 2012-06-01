/*
 * timer.h
 *
 *  Created on: Nov 2, 2011
 *      Author: fikru
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <sys/time.h>
/** Basic class to calculate time difference*/
class timer{
private:
	timeval startTime,endTime;
public:
	/**constructor*/
	timer();
	/**destructor*/
	~timer();
	/**saves the beginning of the simulation run*/
	void setStart();
	/**saves the end of the simulation run*/
	void setEnd();

	/**get simulation start time
	 * @return simulation start time - since epoch time
	 */
	timeval getStart();

	/**get simulation end time
	 * @return simulation end time - since epoch time
	 */
	timeval getEnd();
};

#endif /* TIMER_H_ */
