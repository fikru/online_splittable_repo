#include<vector>
#include "graph.h"

vector<float> graph::getLoad(){
	return this->load;
}

vector<double> graph::getPower(){
	return this->power;
}

void graph::setLoad(float load){
	this->load.push_back(load);
}

void graph::setpower(double power){
	this->power.push_back(power);
}
