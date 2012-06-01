#include <iostream>
#include <sstream>
#include "router.h"
#include "rLink.h"
#include "global.h"

rLink *l;
list<rLink> lList;

router::router(int ID, string name, double capacity, double power, int state, int nr_links) : device(ID, name, capacity, power, state, nr_links){
	for(int lid=0; lid < this->getNr_device(); ++lid){
		stringstream sstrm;
		string type="L";
		sstrm << type << lid;
		name = sstrm.str();
		l = new (nothrow) rLink(lid, name, all_l_capacity[ID][lid], all_l_power[ID][lid], 0);
		if(l == 0)
			cout << "error allocating memory space for the links";
		setLList(*l);
	}
}

list<rLink> router::getRListLink(){
	return lList;
}

double router::getActualCapacity(){
	return actualCapacity;
}

double router::getTotalPower(){
	return totalPower;
}

double router::getResidual(){
	return this->residualCapacity;
}

bool router::getFlag(){
	return this->flag;
}

void router::setLList(rLink l){
	this->lList.push_back(l);
}

void router::setRLinkList(list<rLink> lList){
	this->lList = lList;
}

void router::setActualCapacity(double actualCapacity){
	this->actualCapacity = actualCapacity;
}

void router::setTotalPower(double totalP){
	this->totalPower = totalP;
}

void router::setResidual(double residual){
	this->residualCapacity = residual;
}

void router::setFlag(bool flag){
	this->flag = flag;
}
