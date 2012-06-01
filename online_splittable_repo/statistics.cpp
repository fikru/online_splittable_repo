#include <sys/time.h>
#include "statistics.h"

//constructor
statistics::statistics(){
	forwardedList.clear();
	lostList.clear();
	time_advancedList.clear();
	objectiveList.clear();
	loadList.clear();
	objective = 0;
}

//get forwarded traffic list
std::vector<double> statistics::getForwardedList(){
	return this->forwardedList;
}

//get lost traffic list
std::vector<double> statistics::getLostList(){
	return this->lostList;
}

//get advanced time
std::vector<long> statistics::getTimeAdvList(){
	return this->time_advancedList;
}

//get objective vector
std::vector<double> statistics::getObjectiveList(){
	return this->objectiveList;
}

//get load vector
std::vector<double> statistics::getLoadList(){
	return this->loadList;
}

//get objective current
double statistics::getObjective(){
	return this->objective;
}

//set forwarded traffic list
void statistics::setForwardedList(double forwarded){
	this->forwardedList.push_back(forwarded);
}

//set lost traffic list
void statistics::setLostList(double lost){
	this->lostList.push_back(lost);
}

//set time advanced
void statistics::setTimeAdvList(long time_adv){
	this->time_advancedList.push_back(time_adv);
}

//set objective current
void statistics::setObjective(double objective){
	this->objective = objective;
}

//set objective
void statistics::setObjectiveList(double objective){
	this->objectiveList.push_back(objective);
}

//set load
void statistics::setLoadList(double load){
	this->loadList.push_back(load);
}
