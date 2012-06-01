#include "rLink.h"
#include "statistics.h"

rLink::rLink(int ID, string name, double capacity, double power, int state) : device(ID, name, capacity, power, state){
}

//get residual capacity of this link
double rLink::getResidual(){
	return this->residualCapacity;
}

//get flag
bool rLink::getFlag(){
	return this->flag;
}

//get link flow statistics list
vector<double> rLink::getLstat(){
	return this->lstat;
}

//set residual capacity of this link
void rLink::setResidual(double residual){
	this->residualCapacity = residual;
}

//set flag
void rLink::setFlag(bool flag){
	this->flag = flag;
}

//set link flow statistics
void rLink::setLstat(double s){
	this->lstat.push_back(s);
}
