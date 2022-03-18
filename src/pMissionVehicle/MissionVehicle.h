/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: MissionVehicle.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef MissionVehicle_HEADER
#define MissionVehicle_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "SearchAreaTask.h"

#define NUMBER_OF_OTHER_SYSTEMS 8
#define NAVIGATION 0
#define MAGNETOMETER 1
#define CAMERA 2
#define LIGHTS 3
#define SONAR 4
#define BATTERY 5
#define CONTROLLER 6
#define ENGINE 7

#define MISSION_OFF_NOT_READY 0
#define MISSION_OFF_READY 1
#define MISSION_ON 0

#define R_WHO "Who"
#define R_SWARM "swarm"

#define STATE_OK 0
#define STATE_ERROR_MISSION_NOT_SCANNING 1
#define STATE_ERROR_REMOTE_PARAMETERS 2
#define STATE_ERROR_REMOTE_JOY 3
#define STATE_ERROR_STOP_ON_SURFACE 4
#define STATE_ERROR_ALL_STOP 5

#define DELAY 500

class MissionVehicle : public AppCastingMOOSApp
{
 public:
   MissionVehicle();
   ~MissionVehicle();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
	SearchAreaTask* m_MissionMsg;
	double m_vehicleDepth;
	bool m_isMission;
	bool m_MissionReady;
	int m_state;
	string m_helmState;
 
	static bool isForMe(string msg, string my_name);
	void stopMission();
	static void* runMission(void* me);
	int generateReportMission();
};

#endif 
