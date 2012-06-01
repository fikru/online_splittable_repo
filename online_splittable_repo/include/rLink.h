#ifndef rlink_h
#define rlink_h

#include <vector>
#include "device.h"
#include "statistics.h"

/** Router link class reference. It inherits device class and creates a new link
 *
 * <b>Usage Example:</b>
 * @code rLink *l = new rLink(1, "link_1", 0, 0, 0, 0)
 * @endcode
 */
class rLink : public device {
	double residualCapacity;	/**<residual link capacity*/
	bool flag;					/**<flag to indicate if a link is used in routing operation or not*/
	vector<double> lstat;       /**<link statistics - a vector of links' load*/

public:
	/** Construct a link
	 *@param ID link identification number
	 *@param name link name
	 *@param capacity link capacity
	 *@param power link power consumption
	 *@param state link state: ON/OFF
	 *@see device(int ,string ,double ,double ,int ,int )
	 */
	rLink(int , string , double , double , int );

	///Destructor
	~rLink(){};

	/**Gets residual capacity of a link
	 * @return link residual capacity
	 */
	double getResidual();

	/**Gets link usage flag
	 * @return flag
	 */
	bool getFlag();

	/**Gets link flow statistics
	 * @return current flow amount in the link
	 */
	vector<double> getLstat();

	/**Sets residual capacity of a link
	 * @param residual
	 */
	void setResidual(double );

	/**Sets link flag
	 * @param flag link usage flag: 1 if a link is used, otherwise 0
	 */
	void setFlag(bool );

	/**Sets link flow statistics
	 * @param s amount of flow to use a link
	 */
	void setLstat(double );
};

#endif // link_h
