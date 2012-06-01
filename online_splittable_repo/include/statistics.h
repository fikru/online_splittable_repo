#ifndef statistics_h
#define statistics_h

#include <vector>

/**Statistics class to collect device states. General traffic statistics - such as lost traffic, total forwarded traffic,
 * 	total power consumption (objective)
 */
class statistics{
private:
	/**Statistics for troubleshooting*/
	double objective;								/**<current total power consumption of the architecture*/
	std::vector<double> forwardedList;				/**<forwarded flows*/
	std::vector<double> lostList;					/**<lost flows*/
	/**statistics for plotting*/
	std::vector<double> objectiveList;				/**<power consumption of the architecture used in forwarding current flows*/
	std::vector<long> time_advancedList;			/**<time_advance = end_time - start_time*/
	std::vector<double> loadList;

public:
	///constructor
	statistics();

	///destructor
	~statistics(){};

	/**get forwarded traffic list
	 * @return forwarded traffic list
	 */
	std::vector<double> getForwardedList();

	/**get lost traffic list
	 * @return lost traffic list
	 */
	std::vector<double> getLostList();

	/**get advanced time
	 * @return time required for packing
	 */
	std::vector<long> getTimeAdvList();

	/**get objective
	 * @return list of total power consumption of the architecture
	 */
	std::vector<double> getObjectiveList();

	/**get load
	 * @return list of loads
	 */
	std::vector<double> getLoadList();

	/**get objective previous
	 * @return total power consumption of the previous configuration of the architecture
	 */
	double getObjective();

	/**set forwarded traffic list
	 * @param forwarded routed traffic
	 */
	void setForwardedList(double );

	/**set lost traffic list
	 * @param lost traffic that is not routed because of lack of capacity
	 */
	void setLostList(double );

	/**set advanced time
	 * @param time_adv time since the beginning of running the heuristic
	 */
	void setTimeAdvList(long );

	/**set current objective
	 * @param objective power consumption of current configuration
	 */
	void setObjective(double );

	/**put objective in the list
	 * @param objective power consumption of current configuration
	 */
	void setObjectiveList(double );

	/**set load
	 * @param load current traffic amount to be forwarded
	 */
	void setLoadList(double );

};

#endif // statistics_h
