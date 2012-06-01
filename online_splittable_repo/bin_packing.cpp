#include <limits>
#include "global.h"
#include "rLink.h"
#include <iostream>
using namespace std;

int objective;					//total power consumption of devices used in current flow forwarding
bool packed_flag;				//set if all traffic is packed in a given number of routers
bool lpacked_flag;				//set if all traffic is packed in a given number of links in a given number of routers
void pack_routers(double &, router &);
void pack_links(double &, rLink &);
int nr_r;

statistics bin_packing(double load_leftover, list<router> &rList){
	/*-- bin packing algorithms
	 * REFERENCE: Approximation algorithms for bin packing: A survey
	 * 		First fit (ff)
	 * 		First fit decreasing (ffd)
	 * 		Best fit (bf)
	 * 		Best fit decreasing (bfd)
	 * 		Worst fit (wf)
	 * 		Worst fir decreasing (wfd)
	 */
	nr_r = 0;
	#ifdef _FIRST_FIT
		ff(load_leftover, rList);
	#endif

	//Not yet implemented
	#ifdef _BEST_FIT
		bf(load_leftover, rList);
	#endif
	return mssr_stat_current;
}

/*
 * NAME: ff()
 * DESCRIPTION: First Fit bin packing algorithm -- forward a flow using the first available backend
 * 				router and the first available link on that router which have a capacity to handle the flow.
 * 				If such a router is not available, drop the flow.
 * INPUTS:
 * 		parameters: traffic, rList
 * OUTPUTS:
 * 		return: rList
 * PROCESS:
 * 		. forward a flow using the first available router and the first link available on that
 * 		  router that can handle the flow
 * 		. update different flow statistics such as residual capacity of a currently used link,
 * 		  lost flows, objective value, etc., on the way
 */
void ff(double load_leftover, list<router>& rList){
	mssr_stat_current = unused_instance;
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end();++r_itr){
		if(load_leftover == 0){									//flow totally packed
			break;
		}
		++nr_r;
		pack_routers(load_leftover, *r_itr);
	}
}

void pack_routers(double& load_leftover, router& r_itr){
	double r_residual = r_itr.getResidual();					//residual capacity of this router
	list<rLink> lList = r_itr.getRListLink();					//list of links on this router

	if(r_residual > 0){
		if(!r_itr.getFlag()){
			r_itr.setFlag(1);									//indicate the router is under use
			objective += r_itr.getPower();						//Increment the objective by this router power consumption
		}
		if(load_leftover >= r_residual){
			load_leftover -= r_residual;
			r_itr.setResidual(0);
			mssr_stat.setForwardedList(r_residual);
			for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end();++l_itr){
				pack_links(r_residual, *l_itr);
				if(r_residual == 0){							//router links are fully packed
					break;
				}
			}
		}else{
			r_residual -= load_leftover;
			r_itr.setResidual(r_residual);
			mssr_stat.setForwardedList(load_leftover);
			for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end();++l_itr){
				pack_links(load_leftover, *l_itr);
				if(load_leftover == 0){							//flow totally packed
					break;
				}
			}
		}

		/*-- if all routers used but the load_leftover is not zero, record the amount
		* of load_leftover as dropped traffic --*/
		if(load_leftover != 0 &&  nr_r == nr_routers){
			mssr_stat.setLostList(load_leftover);
			load_leftover = 0;
		}
		r_itr.setRLinkList(lList);
	}
}

void pack_links(double& load, rLink& l_itr){
	double l_residual = l_itr.getResidual();					//residual capacity of a link on this router
	if(l_residual > 0){
		if(!l_itr.getFlag()){
			l_itr.setFlag(1);									//set used flag
			objective += l_itr.getPower();
		}
		if(load >= l_residual){
			load -= l_residual;
			l_itr.setResidual(0); 								//set no residual link capacity
			l_itr.setLstat(l_residual);
		}else{
			l_residual -= load;
			load = 0;
			l_itr.setResidual(l_residual); 						//set the residual link capacity
			l_itr.setLstat(load);
		}
	}
}
