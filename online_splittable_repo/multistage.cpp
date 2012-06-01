#include <string>
#include <sstream>
#include "global.h"
#include "multistage.h"
#include "router.h"
#include <iostream>
#include <new>

router *r;
list<router> rList;

/*
 * Name: multistage
 * Description: constructs a multi-stage router architecture with an initial
 *              state, and creates the routers with a given parameters
 * INPUT:
 * 		parameters: ID, name, capacity, power, state, nr_routers
 * OUTPUT:
 * 		parameter: none
 * 		return: none
 * PROCESS:
 * 		. construct a new multi-stage architecture
 * 		. create new routers in the architecture with give parameters
 * 		. add the routers to list rList
 */
multistage::multistage(int ID, string name, double capacity, double power, int state, int nr_routers) : device(ID, name, capacity, power, state, nr_routers){
	for(int rid=0; rid < this->getNr_device(); ++rid){
		stringstream sstrm;
		string type="R";
		sstrm << type << rid;
		name = sstrm.str();
		r = new router(rid, name, r_capacity[rid], r_power[rid], 0, nr_links);
		if(r == 0)
			cout << "error allocating memory space for the routers";
		setRList(*r);
	}
}

/*
 * Name: 	getRList()
 * Description: 	returns router list rList
 * Input:
 * 		parameters: none
 * Output:
 * 		return: list<router> rList
 */
list<router> multistage::getRList(){
	return rList;
}

/*
 * Name: 	setRList()
 * Description: 	add a router to a list rList
 * Input:
 * 		parameters: router r
 * Output:
 * 		return: none
 */
void multistage::setRList(router r){
	rList.push_back(r);
}
