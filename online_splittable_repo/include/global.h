#ifndef global_h
#define global_h

#include <vector>
#include <list>
#include "router.h"
#include "statistics.h"
#include "multistage.h"
#include "timer.h"

using namespace std;

/**@file global.h*/

#define _DEBUG						///<to display some results to see if the algorithm is working properly or not
//#define _CONF							///<to display the configuration file
//#define _REVERSE_SORT						///<sorting routers(only sorted based on router power) and links in opposite order to the optimal
//#define _SHUFFLE						///<if sorting routers and links not required randomly shuffle the router list
//#define _ROUTER_POWER_SORT							///<if sorting routers and links required
#define _LINK_ROUTER_POWER_SORT	            ///<consider both link and router power consumption in ordering the routers
#define _FIRST_FIT						///<first fit algorithm
//#define _BEST_FIT						///<best fit algorithm

const int L_MAX_CAPACITY = 10000;			///<possible max link capacity
const int L_MIN_CAPACITY = 100;				///<possible min link capacity

extern statistics mssr_stat, mssr_stat_current, unused_instance;

extern int nr_flows; 					///<nr of traffic flows
extern int nr_routers;					///<nr of routers in the architecture
extern int nr_links;					///<nr of links in a router
extern int nr_r; 						///<nr of routers used in current configuration

extern vector<double> traffic;			///<traffic needed to be routed
extern vector<double> r_capacity;		///<vector of capacity for each router
extern vector<double> r_power;			///<vector of power consumption for each router
extern vector<double> l_capacity;		///<vector of capacity for each link on a  router
extern vector<double> l_power;			///<vector of power consumption of each link on a router
extern vector<vector <double> > all_l_capacity; 	///<vector of links on all routers
extern vector<vector <double> > all_l_power;		///<vector of power consumption of links on all routers

/**@fn input_config(string )
 * @brief reads configuration from text file. The configuration of the architecture
 * is described using text file. The algorithm reads this configuration file and convert
 * it into vectors
 * @param config_file name of the file containing the configuration. The file name passed to
 * the algorithm through main argument
 */
void input_config(string );

/** @brief parse the command line.
 * @param program_name the name of the running program
 * @param exit_code if the programs exits without execution, indicate why it is not
 */
void usage(const char* program_name, int exit_code);

void actualCapacity(multistage *, list<router> &);
void totalRouterPower(list<router> &);

void rsort(list<router>& );
bool rcompare(router& , router& );
bool lcompare(rLink& , rLink& );
list<rLink> lsort(list<rLink> );

statistics bin_packing(double , list<router> & );
void ff(double , list<router> & );

double time_elapsed(timer&);

void display(list<router> , statistics );
void config_file(list<router> );
void prevStat(statistics );
int conf_diff(list<router> &, list<router> &);
//struct stat check_mod_time(char *,struct stat &);
void free_mem(multistage *);
#endif //global_h
