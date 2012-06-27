#include <iostream>
#include <algorithm>
#include "global.h"
#include "router.h"
#include "rLink.h"

using namespace std;

/**
 * @brief The sorting algorithm is a modified first fit bin packing algorithm.
 * After reading the list of routers from a text configuration file, the algorithm
 * sorts the routers according to their efficiency if _ROUTER_POWER_SORT is defined.
 *
 * Procedure:
 * \li sort the routers according to (only router power considered for sorting the routers):
 *		\f{eqnarray}
 *			\eta_{_{NIC-}} = \frac{C_{r}}{P_{r}}
 *		\f}
 *	OR (both link and router power considered for sorting the routers)
 *		\f{eqnarray}
 *			\eta_{_{NIC+}} = \frac{C_{r}}{P_{r} + \sum_{i=1}^N{P_{rl}}}
 *		\f}
 * \li sort the links on each router according to:
 *		\f{eqnarray}
 *			\eta_{_l} = \frac{C_{rl}}{P_{rl}}
 *		\f}
 * @param rList list of routers
 * @return rList ordered list of routers
 */
/*
 * 		compare each routers with the others
 * 		First - sort the routers according to:
 * 			actualCapacity/router power
 * 		Second - sort the links on each router
 */
void rsort(list<router> &rList) {
	list<rLink> lList;
	/*--Order the routes and links as per their efficiency using predicate
	 If the efficiency of two routers are the same, use their power consumption
	 for comparison --*/
	rList.sort(rcompare);
	#ifdef _EFFICIENCY
		rList.reverse();
	#endif
	for (list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++r_itr) {
		lList = lsort(r_itr->getRListLink());
		r_itr->setRLinkList(lList);
	}
}

/*
 * NAME: 		rcompare
 * DESCRIPTION:	compares two routers efficiency = actualCapacity/power
 * PARAMETERS:	two consecutive routers in a routers list
 * RETURN:		boolean value indicating which of the two routers is more efficient
 */
bool rcompare(router& lr, router& rr) {
	double eff_lr, eff_rr; //efficiency of left and right router
	#ifdef _EFFICIENCY
		#ifdef _LINK_ROUTER_POWER_SUM				//if link power is added to the router power for sorting purpose
			eff_lr = lr.getActualCapacity() / (lr.getTotalPower());
			eff_rr = rr.getActualCapacity() / (rr.getTotalPower());
			//cout << eff_lr << " " << eff_rr << endl;
			/*-- if the efficiency of two routers are the same,
			 * then use their power consumption for comparison --*/
			if (eff_lr == eff_rr) {
				eff_lr = lr.getTotalPower();
				eff_rr = rr.getTotalPower();
			}
		#else
			eff_lr = lr.getActualCapacity()/(lr.getPower());
			eff_rr = rr.getActualCapacity()/(rr.getPower());
			//cout << eff_lr << " " << eff_rr << endl;
			/*-- if the efficiency of two routers are the same,
			 * then use their power consumption for comparison --*/
			if (eff_lr == eff_rr) {
				eff_lr = lr.getPower();
				eff_rr = rr.getPower();
			}
		#endif
	#else
		#ifdef _LINK_ROUTER_POWER_SUM				//if link power is added to the router power for sorting purpose
			eff_lr = lr.getTotalPower();
			eff_rr = rr.getTotalPower();
		#else
			eff_lr = lr.getPower();
			eff_rr = rr.getPower();
		#endif

	#endif
	return eff_lr < eff_rr;
}

/*
 * NAME:		lsort
 * DESCRIPTION: sorts the links on a router
 * INPUTS:
 * 		Parameters: lList -- list of links to be sorted
 * OUTPUTS:
 * 		Return: sorted lList
 * PROCESS:
 * 		compare each links with all the other
 * 		sort the links on each router
 */
list<rLink> lsort(list<rLink> lList) {
	lList.sort(lcompare);
	#ifdef _EFFICIENCY
		lList.reverse();
	#endif
	return lList;
}

/*
 * NAME: 		lcompare
 * DESCRIPTION:	compares two links efficiency = linkCapacity/power
 * PARAMETERS:	two consecutive links
 * RETURN:		boolean value indicating which of the two links is more efficient
 */
bool lcompare(rLink& ll, rLink& rl) {
	double eff_ll, eff_rl;
	#ifdef _EFFICIENCY
		eff_ll = ll.getCapacity() / (ll.getPower());
		eff_rl = rl.getCapacity() / (rl.getPower());
		/*-- if the efficiency of two routers are the same,
		 * then use their power consumption for comparison --*/
		if (eff_ll == eff_rl) {
			eff_ll = ll.getPower();
			eff_rl = rl.getPower();
		}
	#else
		eff_ll = ll.getPower();
		eff_rl = rl.getPower();
	#endif
	return eff_ll < eff_rl;
}

