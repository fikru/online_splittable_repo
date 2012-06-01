#ifndef mssr_h
#define mssr_h

#include <list>
#include "device.h"
#include "router.h"

/** Multistage architecture class reference. It inherits device class and creates a new multistage architecture
 * with list of routers and sets total architecture actual capacity.
 *
 * <b>Usage Example:</b>
 * @code multistage *m = new multistage(1, "mssr", 0, 0, 0, 0)
 * @endcode
 * @see device(int ,string ,double ,double ,int ,int )
 */
class multistage : public device {
private:
	list<router> rList;		/**<list of router*/
	double rActualCapacity; /**<actual capacity of a multistage architecture*/
public:
	/** Construct a router
	 *@param ID multistage architecture identification number
	 *@param name multistage architecture name
	 *@param capacity multistage architecture capacity
	 *@param power multistage architecture power consumption
	 *@param nr_routers number of routers contained inside a multistage architecture
	 *@param state a multistage architecture state: ON/OFF
	 *@see device(int ,string ,double ,double ,int ,int )
	 */
	multistage(int , string , double , double , int , int );

	///Destructor
	~multistage(){};

	/** Gets list of routers in an architecture
	 * @return a multistage architecture's router list
	 */
	list<router> getRList();

	/**Sets an architecture's router list
	 * @param r router with all its attributes*/
	void setRList(router );
};

#endif // mssr_h
