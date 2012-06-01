#ifndef router_h
#define router_h

#include <list>
#include "device.h"
#include "rLink.h"

/** Multistage architecture router class reference. It inherits device class and creates a new router
 * with list of links in the same router.
 *
 * <b>Usage Example:</b>
 * @code router *r = new router(1, "router_1", 0, 0, 0, 0)
 * @endcode
 * @see device(int ,string ,double ,double ,int ,int )
 */
class router : public device {
private:
	list<rLink> lList; 			/**<list of links in a given router*/
	double actualCapacity;		/**<actual routing capacity of a router*/
	double residualCapacity;	/**<residual routing capacity of a router*/
	double totalPower;			/**<Total Power consumption of a router - that includes the power consumption of its links*/
	bool flag;					/**<flag to indicate if a router is used in routing operation or not*/

public:
	/** Construct a router
	 *@param ID router identification number
	 *@param name router name
	 *@param capacity router capacity
	 *@param power router power consumption
	 *@param nr_links number of links contained inside a router
	 *@param state router state: ON/OFF
     *@see device(int ,string ,double ,double ,int ,int )
     */
	router(){};
	router(int, string, double, double, int, int );

	///Destructor
	~router(){};

	/** Gets a router's link list
	 * @return list of links on a router
	 */
	list<rLink> getRListLink();

	/**Gets actual capacity of a router
	 * @return actual capacity of a router
	 */
	double getActualCapacity();

	/** Gets residual capacity of a router
	 * @return residual capacity of a router
	 */
	double getResidual();

	/**Get total power of a router = router power + sum of power of the links there of
	 * @return total power of a router
	 */
	double getTotalPower();

	/** Gets if a flag is set or not
	 * @return usage flag indicator
	 */
	bool getFlag();

	/**Sets a router's link list
	 * @param l link with all its attributes*/
	void setLList(rLink );

	/**Overwrite a router's link list if required after some modification
	 * @param lList link list*/
	void setRLinkList(list<rLink> );

	/**Sets a router's capacity
	 * @param actualCapacity router capacity*/
	void setActualCapacity(double );

	/**Sets a router's total power consumption = router power + sum of power consumption of links thereof
	 * @param totalP total power consumption of a router
	 */
	void setTotalPower(double );

	/**Sets residual capacity
	 * @param residual
	 */
	void setResidual(double );

	/**Sets/Resets router flag
	 * @param flag a boolean value: 1 if the router is used in routing operation, 0 otherwise
	 */
	void setFlag(bool );

};

#endif // router_h
