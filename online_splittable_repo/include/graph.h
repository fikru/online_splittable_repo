#ifndef graph_h
#define graph_h

#include <vector>

using namespace std;
/**Graph class used for result display*/
class graph{
private:
	vector<float> load; 		/**<list of loads in time*/
	vector<double> power;		/**<list of objective function corresponding to the list of loads in time*/
public:
	/**constructor*/
	graph();
	/**destructor*/
	~graph(){};
	/**gets list of traffic load*/
	vector<float> getLoad();
	/**gets list of architecture power consumption corresponding to each load*/
	vector<double> getPower();
	/**sets traffic load
	 * @param load current traffic load
	 */
	void setLoad(float );
	/**set current objective
	 * @param power current power consumption of the architecture
	 */
	void setpower(double );
};

#endif //graph_h
