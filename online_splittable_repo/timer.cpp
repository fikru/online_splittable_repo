//#include "global.h"
#include "timer.h"

timer::timer(){}
timer::~timer(){}

void timer::setStart(){
	gettimeofday(&startTime,0);
}

void timer::setEnd(){
	gettimeofday(&endTime,0);
}

//get simulation start time
timeval timer::getStart(){
	return this->startTime;
}

//get simulation end time
timeval timer::getEnd(){
	return this->endTime;
}
