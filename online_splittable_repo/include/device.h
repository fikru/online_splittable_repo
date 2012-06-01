#ifndef device_h
#define device_h

#include <string>
using namespace std;

/**@mainpage Multistage software router energy saving scheme
 * @section intro Introduction
 * An energy saving scheme for <a href="multistage_software_router.pdf" target="_blank"><b>multistage software router</b></a>.
 */

/** Basic class for devices in the multistage architecture. Inherited by multistage, routers and links
 *
 */
class device {
	private:
	   int ID;				/**<device id*/
	   string name;			/**<device name - type of device*/
	   double capacity;		/**<device capacity*/
	   double power;		/**<device power consumption*/
	   bool state;			/**<device state - on or off*/
	   int nr_device; 		/**<number of device contained in a given device (for example: a multistage contains routers and a router contain links)*/

	 public:
	   /** Construct multistage architecture and routers there in
	    *@param ID device identification number
	    *@param name device name
	    *@param capacity device capacity
	    *@param power device power consumption
	    *@param nr_device number of devices contained inside: multistage contain routers and routers contain links
	    *@param state device state: ON/OFF*/
	    device(){};
	    device(int ,string ,double ,double ,int ,int );
	    /** Construct multistage architecture and routers there in
	     *@param ID device identification
	     *@param name device name
	     *@param capacity device capacity
	     *@param power device power consumption
	     *@param state device state: ON/OFF*/
	    device(int ,string ,double ,double ,int );
	    /**destructor*/
		~device(){};
		/** Gets device identification number
		 * @return device identification number*/
		int getID();
		/** Gets device name
		 * @return device name*/
		string getName();
		/** Gets device capacity
		 * @return device capacity*/
		double getCapacity();
		/** Gets device power consumption
		 * @return device power*/
		double getPower();
		/** Gets device state
		 * @return device state*/
		bool getState();
		/** Gets number of devices contained inside a device
		 * @return Number of devices contained inside a device. For example the number of routers
		 * in a multistage architecture*/
		int getNr_device();
		/**Sets device capacity
		 * @param capacity capacity of the device
		 */
		void setCapacity(double );
		/**Sets device power consumption
		 * @param power power consumption of the device
		 */
		void setPower(double );
		/**Sets device state
		 * @param state the stare fo the device: ON/OFF
		 */
		void setState(bool );
};
#endif // device_h
