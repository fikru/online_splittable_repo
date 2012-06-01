#include <iostream>
#include "global.h"
#include "router.h"
#include "statistics.h"

void display(list<router> rList,statistics stat){
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++r_itr){
		list<rLink> lList = r_itr->getRListLink();
		cout << "Router " << r_itr->getID() << " = { ";
		for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end(); ++l_itr){
			//cout << l_itr->getFlag() << " ";
			cout << l_itr->getResidual() << " ";
		}
		cout << "}" << endl;
	}
	cout << endl;
	cout << "Total Load = " << stat.getLoadList().back() << endl;
	cout << "Forwarded = { ";
	vector<double> forwarded = stat.getForwardedList();
	for(vector<double>::iterator f_itr = forwarded.begin(); f_itr != forwarded.end(); ++f_itr){
		cout << *f_itr << " ";
	}
	cout << "}" << endl;

	cout << "Lost = { ";
		vector<double> lost = stat.getLostList();
		for(vector<double>::iterator l_itr = lost.begin(); l_itr != lost.end(); ++l_itr){
			cout << *l_itr << " ";
		}
		cout << "}" << endl;

	cout << "Objective = " << stat.getObjectiveList().back() << endl;
	cout << "Time = " << stat.getTimeAdvList().back() << " sec" << endl << endl;
}

/*
 * NAME: config_file
 * DESCRIPTION: display the input configuration file after parsing the CPLEX_OPL format
 * 				configuration file
 * INPUT:
 * 		parameter: r_list
 * OUTPUT: display configuration file
 *
 */
void config_file(list<router> rList){
	list<rLink> lList;
	cout << "Configuration file" << endl;
	cout << "==================" << endl;
	cout << "K = " << nr_flows << endl;
	cout << "N = " << nr_links << endl;
	cout << "M = " << nr_routers << endl;

	cout << "T = [ ";
	for(vector<double>::iterator t_itr = traffic.begin(); t_itr != traffic.end(); ++t_itr)
		cout << *t_itr << " ";
	cout << "]" << endl;

	cout << "RC = [ ";
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++ r_itr)
		cout << r_itr->getCapacity() << " ";
	cout << "]" << endl;

	cout << "RP = [ ";
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++ r_itr)
		cout << r_itr->getPower() << " ";
	cout << "]" << endl;

	cout << "LC = [ ";
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++ r_itr){
		lList = r_itr->getRListLink();
		cout << "[ ";
		for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end(); ++l_itr){
			cout << l_itr->getCapacity() << " ";
		}
		cout << "]";
	}
	cout << " ]" << endl;

	cout << "LP = [ ";
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++ r_itr){
		lList = r_itr->getRListLink();
		cout << "[ ";
		for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end(); ++l_itr){
			cout << l_itr->getPower() << " ";
		}
		cout << "]";
	}
	cout << " ]" << endl << endl;
}
