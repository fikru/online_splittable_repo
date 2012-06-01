/*
 * SCESApplication - server cluster energy saving application
 *
 */

//To open, read, write and manipulate file
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>  	//for sort()
//exit
#include <stdlib.h>
#include <vector>
#include <list>
#include <new>
#include <getopt.h> 	//to parse the command line options
#include <assert.h>		//to check unexpected conditions in the code

//To obtain file status
#include <sys/types.h>
#include <sys/stat.h>	//for checking file last modified time
#include <unistd.h>     //to handle errors

//non-standard library
#include "global.h"
#include "multistage.h"
#include "router.h"
#include "timer.h"

using namespace std;

/*-- bin packing statistics --*/
statistics mssr_stat, 			//statistics from the beginning of the simulation
		   mssr_stat_current,   //current load statistics
		   unused_instance; 	//empty packing statistics

extern list<router> rList;		//list of routers in the architecture - also shows current configuration
list<router> prev_rList; 		//list of routers in the architecture - also shows previous configuration
extern list<rLink> lList; 		//list of routers in the architecture
string opt;						//parameters for the application
int conf_change_count;				//count the number of configuration changes - in terms of the difference
									//between number of current configuration devices and previous one

int conf_diff(list<router> &rList, list<router> &prev_rList){
	conf_change_count = 0;
	list<router>::iterator r_itr_left = rList.begin();
	list<router>::iterator r_itr_right = prev_rList.begin();
	while(r_itr_left != rList.end() && r_itr_right != prev_rList.end()){
		//if(r_itr_right->getFlag() == 1){
			if(r_itr_left->getFlag() != r_itr_right->getFlag()){
				conf_change_count++;
			}
		//}
		r_itr_left++;
		r_itr_right++;
	}
	return conf_change_count;
}
/*
 * NAME: 			actualCapacity()
 * DESCRIPTION: 	computes the actual capacity (min(router capacity, sum of all link capacity on that router)
 * 				 	of each backend routers
 * INPUTS:
 * 		Parameters: list<routers> rList
 * OUTPUTS:
 * 		Return: none
 * PROCESS:
 * 		. Give priority to higher capacity links to compute actual router capacity (It is reasonable to give
 * 		  high priority for hign capacity links. Because to reach that capacity with less capacity links, the
 * 		  power consumption becomes higher)
 * 		. Sum up link capacity per router
 * 		. sum up the corresponding power consumption
 * 		. Update router capacity with min(router capacity, sum of link capacity of that router)
 */
void actualCapacity(multistage *mssr, list<router> &rList){
	list<rLink> lList;   							//list of links on router r
	int mssrCapacity = 0, mssrPower = 0;			//actual capacity and power consumption of mssr architecture
	for(list<router>::iterator r_itr=rList.begin(); r_itr != rList.end();++r_itr){
		int link_total_cap = 0;			  			//total link capacity per router
		int l_capacity = L_MAX_CAPACITY; 			//the maximum link capacity a router can have
		double totalP = 0;
		lList = r_itr->getRListLink();
		while(l_capacity >= L_MIN_CAPACITY){        //a protection not to go below the smallest link considered
			for(list<rLink>::iterator l_itr=lList.begin(); l_itr != lList.end();++l_itr){
				if(l_itr->getCapacity() == l_capacity){
					link_total_cap += l_itr->getCapacity();
					totalP += l_itr->getPower();
					if (link_total_cap >= r_itr->getCapacity()){
						list<rLink>::iterator l_ptr = ++l_itr;
						/*-- 1. reset all the capacity **after this** pointer whose capacity <= l_capacity to zero --*/
						for(list<rLink>::iterator l_itr=l_ptr; l_itr != lList.end();++l_itr){
							if(l_itr->getCapacity() <= l_capacity){
								l_itr->setCapacity(0);
								l_itr->setResidual(0);
							}
						}
						/*-- 2. reset all the capacity < l_capacity from the beginning to this pointer to zero --*/
						for(list<rLink>::iterator l_itr=lList.begin(); l_itr != l_ptr;++l_itr){
							if(l_itr->getCapacity() < l_capacity){
								l_itr->setCapacity(0);
								l_itr->setResidual(0);
							}
						}
						l_capacity = 1;
						break;
					}
				}
			}
			l_capacity = l_capacity/10;
		}
		totalP += r_itr->getPower();
		r_itr->setTotalPower(totalP);
		if (link_total_cap < r_itr->getCapacity()){
			r_itr->setActualCapacity(link_total_cap);
			r_itr->setResidual(link_total_cap);
		}else{
			r_itr->setActualCapacity(r_itr->getCapacity());
			r_itr->setResidual(r_itr->getCapacity());
			r_itr->setRLinkList(lList);
		}
		mssrCapacity += r_itr->getActualCapacity();
		mssrPower += r_itr->getPower();
	}
	mssr->setCapacity(mssrCapacity);
	mssr->setPower(mssrPower);
}

/*
 * 	Name:			empty_routers()
 *
 * 	Description:	reset the flag of each router and links thereof to indicate
 * 					that it is empty and set the residual to max capacity
 * 	INPUTS:
 * 		Parameters: list<routers> rList
 *	OUTPUTS:
 * 		Return: none
 */
void empty_routers(list<router> &rList){
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++r_itr){
		lList = r_itr->getRListLink();
		r_itr->setFlag(0);
		r_itr->setResidual(r_itr->getActualCapacity());
		for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end(); ++l_itr){
			l_itr->setFlag(0);
			l_itr->setResidual(l_itr->getCapacity());
			l_itr->setLstat(0);
			l_itr->getLstat().clear();
		}
		r_itr->setRLinkList(lList);
	}
}

/*
 * usage: command line usage information
 */
void usage(const char* program_name, int exit_code) {
	cout << "\n Usage: " << program_name << " options" << " [configuration_file]"
		 << "\n\n"
	     << "options:\n"
	     << "-f --file \t CPLEX_OPL format input configuration file\n"
	     << "-h --help \t Display this message and exit\n"
	     << "-v --verbose \t Print verbose message\n";
	exit(exit_code);
}


/*
 * Name: 		main
 * Description: Server Cluster Energy Saving (SCES) Application is an energy saving scheme
 * 				under a distributed scenario (e.g. server cluster). The algorithm is
 * 				meant to compare on-line differential energy saving scheme with the	optimal
 * Inputs:
 *		Parameters:
 *			configuration file (.dat file from CPLEX output)
 * Outputs:
 *		Return: None
 *		Others:
 *			- output files
 *			- some statistical data (class)
 * Process:
 *		- parse the command line arguments
 *		- read the configuration file
 *		- create multistage software router architecture from configuration file
 *		- update some variables in the architecture
 *		- sort routers if required
 *		- check if there is any event (change in traffic load)
 *				- check if there is change in timestamp of the file
 *				- compare the load amount change in the configuration file
 *		- if there is some event, then start the packing
 *		- register different statistics and prepare data for plot
 */

int main(int argc, char** argv){
	string 	data, script, 		//file name from command line (configuration file)
			load_line, 			//traffic record in file input
			s1,s2,s3,
			sub_string1, sub_string2;			//substring of the traffic record (particularly intersted in traffic amount)
	multistage *mssr;			//a multistage software router architecture
	timer pack_time;			//packing time

	int mssrCapacity = 0, mssrPower = 0,
		first_run = 1;				//number of runs
		//s_stat;					//return status of system()
	long 	init_time,			//the first run time
			start, 				//each run starting time
			end = 0, 				//each run end time
			act_time = 0;			//time elapsed since init_start time
	double  time_advanced, 		//time elapsed since the beginning of simulation
			objective,
			prev_load = 10000000, // some large number greater than the capacity of the architecture
			current_load = 0,		//current packing load amount
			packed_total = 0;
	ofstream out_file;			//output file for plot data
	ios_base::openmode mode;	//mode of the out_file
	ifstream in_file;			//input file - configuration file

	struct stat buf;			//configuration file status check
	timespec now;		//last configuration modification time
	long file_last_mod_time = 0,
		 file_now_mod_time = 0,
		 traffic_last_mod_time = 0,
		 traffic_now_mod_time = 0;

	/*-- definition to parse the command line --*/
	const char* program_name = "SCESApplication";
	int next_option;					//getopt_long() return value
	int verbose = 0;					//whether to display verbose message
	const char* short_option="hf:v";    	//command line short option specifier. The colon means the option before takes an argument
	//command line long option specifier
	const struct option long_option[] ={
			{"help", 0, NULL, 'h'},
			{"file", 1, NULL, 'f'},
			{"verbose", 0, NULL, 'v'},
			{NULL, 0, NULL, 0} 			//required at end of array
	};
	/*-- parse the command line arguments --*/
	do{
		next_option=getopt_long(argc, argv, short_option, long_option, NULL);
		switch(next_option)
		{
		case 'h': 	//-h or --help
			/*User requested usage information. Print it to standard output and exit with exit code 0 (normal termination)*/
			usage(program_name, 0);
		case 'f': 	//-d or --data
			/*this option takes an argument, the name of the input configuration file*/
			data = argv[2];
			break;
		case 'v': 	//-v or --verbose
			verbose = 1;
			break;
		case '?':	//the user specified an invalid option
			/*print usage information to standard output, and exit with exit code 1(abnormal termination)*/
			usage(program_name, 1);
		case -1:	//Done with the options
			break;
		default:	//something else: unexpected
			abort();
		}
	}while(next_option != -1);	//done with options. OPTIND points to first nonoption argument

	if(verbose){
		int i;
		for(i = optind; i < argc; ++i){
			cout << "Argument: " << argv[i];
		}
	}

	/*File last modified time*/
	if (stat(argv[2], &buf) == -1) {
			perror(argv[2]);
			exit(EXIT_FAILURE);
	}
	now = buf.st_mtim;
	file_last_mod_time = now.tv_sec*1000000 + now.tv_nsec;

	/*Check if the file data = argv[2] is modified or not*/
	while(1){
		/*check file last modification time*/
		if (stat(argv[2], &buf) == -1) {
			perror(argv[2]);
			exit(EXIT_FAILURE);
		}
		now = buf.st_mtim;
		file_now_mod_time = now.tv_sec*1000000 + now.tv_nsec;

		/*If data = argv[2] is modified, run the heuristic equivalent of the CPLEX optimization*/
		if (file_last_mod_time != file_now_mod_time){
			//Give time for the first run - this gives time to synchronize with CPLEX. It takes time
			//for CPLEX to write the file
			sleep(1);

			/*reset traffic modification time*/
			traffic_now_mod_time = 0;

			/*Reset the statistics for current heuristic run*/
			mssr_stat = unused_instance;

			/*-- read the configuration file --*/
			input_config(argv[2]);

			/*-- create the multistage architecture --*/
			mssr = new (nothrow) multistage(0, "mssr", mssrCapacity, mssrPower, 0, nr_routers);
			assert(mssr != 0);		//error allocating memory space

			/*-- compute and set actual routing capacity of the architecture and the routers
			 *   and the routers' total power consumption(= router power + sum of link power)--*/
			rList = mssr->getRList();
			actualCapacity(mssr, rList);

			/*-- initialize some variables in the statistics to zero and reset the routers to default--*/
			empty_routers(rList);
			mssr_stat.setLoadList(0);

			/*if sort required, sort the routers and the links thereof as per their efficiency = capacity/power*/
			rsort(rList);

			#ifdef _SHUFFLE
				vector<router> v;
				list<router> rl;
				v.resize (rList.size());
				copy(rList.begin(), rList.end(), v.begin());
				random_shuffle(v.begin(), v.end());
				copy(v.begin(), v.end(), std::back_inserter(rl));
				rList = rl;
			#endif

			#ifdef _CONF
				config_file(rList);
			#endif

			/*If the increment to the load reach to maximum, continue to check if there is a change in the load*/
			while(mssr_stat.getLoadList().back() <= mssr->getCapacity()){
				/*check traffic last modification time - i.e. check if there is a change in the load*/
				if (stat(argv[2], &buf) == -1) {
					perror(argv[2]);
					exit(EXIT_FAILURE);
				}
				now = buf.st_mtim;
				traffic_now_mod_time = now.tv_sec*1000000 + now.tv_nsec;

				/*-- Event triggered architecture re-setup - obtain the traffic statistics from shell generated data file--*/
				in_file.open(argv[2]);
				assert(in_file.good() != 0); 	//error opening the file

				while(!in_file.eof()){
					getline(in_file,load_line);
					istringstream ss(load_line);
					ss >> sub_string1;
					if(sub_string1 == "T"){
						istringstream converttod;
						ss >> sub_string1 >> sub_string2;
						converttod.str(sub_string2);
						converttod >> current_load;
						break;
					}
				}
				in_file.close();

				/*-- if there is a change in load, recompute the architecture and power consumption --*/
				if (traffic_last_mod_time != traffic_now_mod_time && current_load != prev_load){
					/*update the modification time*/
					traffic_last_mod_time = now.tv_sec*1000000 + now.tv_nsec;
					/*update previous traffic with the current - used to control if there is a load change
					 * in the shell generated data file or not*/
					prev_load = current_load;

					//cout << mssr->getCapacity() << '\t' << current_load << endl;

					/*-- re-pack only the already used routers if the load is decreasing
					 *   else keep current solution and add any additional resource required
					 *   If the load is decreasing we just reset the routers*/

					if(current_load < mssr_stat.getLoadList().back()){
						/*-- empty the routers before packing - reset the router to default for the next run --*/
						empty_routers(rList);
						/*-- clear previous statistics --*/
						mssr_stat = unused_instance;
					}else{
						/*-- decrease an already packed amount (previous load) from the current load
						 * we just need some additional routers to handle the extra traffic --*/
						vector<double> forwarded = mssr_stat.getForwardedList();
						packed_total = 0;
						for(vector<double>::iterator forw_itr = forwarded.begin(); forw_itr != forwarded.end(); ++forw_itr){
							packed_total += *forw_itr;
						}
						current_load -= packed_total;
					}

					/*-- next change in load check time --*/
					pack_time.setStart();
					start = pack_time.getStart().tv_sec + pack_time.getStart().tv_usec/1000000;
					if(first_run)
						init_time = start; 			//save the reference time

					/*-- bin packing algorithm - packs the load to free routers and update statistics --*/
					mssr_stat_current = bin_packing(current_load, rList);

					/*update current load vector*/
					mssr_stat.setLoadList(prev_load);

					/*-- stamp the end of simulation and compute simulation time --*/
					pack_time.setEnd();
					end = pack_time.getEnd().tv_sec + pack_time.getEnd().tv_usec/1000000;
					//stat_current.setEnd(end);

					//compute and register elapsed time time
					time_advanced = start - init_time;
					mssr_stat.setTimeAdvList(time_advanced);

					//set objective
					objective = 0;
					for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++r_itr){
						if(r_itr->getFlag()){
							objective += r_itr->getPower();
							list<rLink> lList = r_itr->getRListLink();
							for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end(); ++l_itr){
								if(l_itr->getFlag()){
									objective += l_itr->getPower();
								}
							}
						}
					}
					mssr_stat.setObjectiveList(objective);

					/*-- Copy current statistics stat to use it as a reference for the next run
					 *   Don't use copy constructor however; because we need only some statistics
					 *   to be updated --*/
					prevStat(mssr_stat_current);

					//stat_prev = stat_current;
					//Compare the number of devices involved in current configuration and the previous one
					if(first_run || current_load == 0)
						prev_rList = rList;         //use the first configuration
					conf_change_count = conf_diff(rList, prev_rList);
					prev_rList = rList;

					/*-- display results --*/
					#ifdef _DEBUG
						display(rList, mssr_stat);
					#endif

					/*-- time required by plot --*/
					act_time = start - init_time;

					/*-- write plot data to file --*/
					if(mssr_stat.getLoadList().back() == 0)		//if it is the first run, then create a new empty file
						mode = ios::trunc;
					else		//else append data to file
						mode = ios::app;
					out_file.open("plotdata.txt", mode);
					if( !out_file ) { // file couldn't be opened
						cerr << "Error: file could not be opened" << endl;
						exit(1);
					}
					//cout  << mssr_stat.getLoadList().back() << '\t'  <<  mssr_stat.getObjectiveList().back() << '\t' << conf_change_count << endl;
					out_file << mssr_stat.getObjectiveList().back() << '\t' << conf_change_count << endl;
					out_file.close();

					/*-- the first run already done, reset the first run indicator --*/
					first_run = 0;
				}
				/* If all the traffic is packed, break this while and continue to check if there
				 * is configuration change*/

				if(mssr_stat.getLoadList().back() >= mssr->getCapacity()){
					break;
				}
			}
			//set the file modification time to the traffic modification time
			file_last_mod_time = traffic_now_mod_time;
			//delete all objects and prepare for the next run
			free_mem(mssr);
		}
	}
	return 0;
}

/*Free allocated memory*/
void free_mem(multistage *mssr){
	for(list<router>::iterator r_itr=rList.begin(); r_itr != rList.end(); ++r_itr){
		list<rLink> lList;
		lList = r_itr->getRListLink();
		for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end(); ++l_itr){
			l_itr = lList.erase(l_itr); //delete the current object - link
			--l_itr;					//because for loop increment it
		}
		r_itr = rList.erase(r_itr);		//delete the current object - router
		--r_itr;						//because for loop increment it
	}

	/*vectors used during input data reading*/
	/*r_capacity.clear();
	r_power.clear();
	l_capacity.clear();
	l_power.clear();
	all_l_capacity.clear();
	all_l_power.clear();*/
	delete mssr;
}

/* Name:	 		prevStat
 * Description:		update flow statistics
 */
void prevStat(statistics stat_current){
	std::vector<double> forwarded = mssr_stat_current.getForwardedList();
	std::vector<double> lost = mssr_stat_current.getLostList();
	for(vector<double>::iterator forw_itr = forwarded.begin(); forw_itr != forwarded.end(); ++forw_itr){
		mssr_stat.setForwardedList(*forw_itr);
	}
	for(vector<double>::iterator lost_itr = lost.begin(); lost_itr != lost.end(); ++lost_itr){
		mssr_stat.setLostList(*lost_itr);
	}
}
