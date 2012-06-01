#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int nr_flows, nr_routers, nr_links;
vector<double> traffic;
vector<double> r_capacity, r_power;
vector<double> l_capacity,l_power;
vector<vector <double> > all_l_capacity;
vector<vector <double> > all_l_power;

/*
 * isOdd -- check if an integer value is odd or not
 */
bool isOdd(int );

/* NAME: read_input(string )
 *
 * DESCRIPTION: read backend router parameters from CPLEX_OPL configuration file
 *
 * INPUT:
 *		parameter: CPLEX_OPL formate configuration file
 * OUTPUT:
 *		global: traffic
 *		return: r_list - list of routers class
 *	PROCESS:
 *		. parse the input file
 *		. save the different parameters to the corresponding object
 */
void input_config(string config_file)
{
	double t;
	string nextToken, param;
	int count = 0, paramGroup;
	ifstream input_data;
	input_data.open(config_file.c_str());

	if (input_data.fail()) {
		cerr << "Unable to open file for reading " + config_file << endl;
	}

	while (input_data >> nextToken) {
		if(nextToken == "T" || nextToken == "K" || nextToken == "N" || nextToken == "M"){
			paramGroup = 0;
			param = nextToken;
			count = 1;
		}else if(nextToken == "RC" || nextToken == "RP"){
			paramGroup = 1;
			param = nextToken;
			count = 1;
		}else if(nextToken == "LC" || nextToken == "LP"){
			paramGroup = 2;
			param = nextToken;
			count = 1;
		}
		/*-- get the number of flows, routers and links per router
		 * from the configuration file
		 */
		if(count == 3 && paramGroup == 0){
			string sb;
			istringstream s(nextToken);
			while(s){
				if (!getline(s,sb,';'))
					break;
				istringstream vi(sb);
				vi>>t;								// >> operator converts the vi string to double
				if(param == "T"){
					traffic.push_back(t);
				}else if(param == "K"){
					nr_flows = t;
				}else if(param == "N"){
					nr_links = t;
				}else if(param == "M"){
					nr_routers = t;
				}
			}
		}
		/*-- if the line start with T, RC or RP,
		 * update the corresponding vector with data from the configuration file--*/
		if(count == 4 && paramGroup == 1){
			string sr;
			istringstream s(nextToken);
			while(s){
				if (!getline(s,sr,','))
					break;
				istringstream vi(sr);
				vi>>t;								// >> operator converts the vi string to double
				if(param == "RC"){
					r_capacity.push_back(t);
				}else if(param == "RP"){
					r_power.push_back(t);
				}
			}
		}

		/*-- if a line start with LC or LP,
		 * update the corresponding vector with data from the configuration file--*/
		if(isOdd(count) && count >= 5 && paramGroup == 2){
			string sl;
			l_capacity.clear(),l_power.clear();
			istringstream s(nextToken);
			while(s){
				if (!getline(s,sl,','))
					break;
					istringstream vi(sl);
					vi>>t;
					if(param == "LC"){
						l_capacity.push_back(t);
					}else if(param == "LP"){
						l_power.push_back(t);
					}
			}
			if(param == "LC"){
				all_l_capacity.push_back(l_capacity);
			}else if(param == "LP"){
				all_l_power.push_back(l_power);
			}
		}
		++count;
	}
	input_data.close();
}

/*
 * isOdd - check if a parameter (integer value) is odd or even
 */
inline bool isOdd( int count )
{
  if ( count % 2 == 0 )
     return false;
  else
     return true;
}
