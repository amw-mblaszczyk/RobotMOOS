/************************************************************/
/*    NAME: m.blaszczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: Remote.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Remote_HEADER
#define Remote_HEADER

class Joystick;

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MOOS/libMOOS/MOOSLib.h"
#include "MsgRemote.h"

#include <string>

class Remote : public AppCastingMOOSApp
{
 public:

	/**
	 * Standard constructor for class Remote.
	 */
   Remote();

   /**
    * Destructor for class Remote.
    */
   ~Remote();

   /**
    * Sends MOOSDB message REMOTE.
    * @param msg REMOTE message content.
    * @return sending results.
    */
	bool sendRemoteMsg(string msg);

 protected: // Standard MOOSApp functions to overload  

   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload
    bool buildReport();

 protected:
   void RegisterVariables();
   bool handleRemoteConf(string sval);

 public: // Configuration variables
   unsigned short joy_rudder_code = 0x00; 	//ABS_X
   unsigned short joy_thrust_code = 0x01; 	//ABS_Y
   unsigned short joy_elevator_code = 0x05; 	//ABS_RZ
   unsigned short joy_btn_rudder_0 = 0x131;		//BTN_B
   unsigned short joy_btn_rudder_0_alt = 0x133;		//BTN_X
   unsigned short joy_btn_elevator_0 = 0x130;		//BTN_A
   unsigned short joy_btn_elevator_0_alt = 0x134;		//BTN_Y

 private: // State variables
   /**
    * Joystick object connected.
    */
   Joystick *joy;
};

#endif 
