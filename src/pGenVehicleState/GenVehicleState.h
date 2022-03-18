/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenVehicleState.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef GenVehicleState_HEADER
#define GenVehicleState_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

#define NUMBER_OF_OTHER_SYSTEMS 8
#define NAVIGATION 0
#define MAGNETOMETER 1
#define CAMERA 2
#define LIGHTS 3
#define SONAR 4
#define BATTERY 5
#define CONTROLLER 6
#define ENGINE 7

#define STATE_OK 0
#define STATE_ERROR_MISSION_NOT_SCANNING 1
#define STATE_ERROR_REMOTE_PARAMETERS 2
#define STATE_ERROR_REMOTE_JOY 3
#define STATE_ERROR_STOP_ON_SURFACE 4
#define STATE_ERROR_ALL_STOP 5

#define DELAY 500

#define INTERNAL_NOTHING 0
#define INTERNAL_GO_UP 1

#define MAX_TIME_TO_APPEAR_ON_SURFACE 600

class GenVehicleState : public CMOOSApp
{
 public:
   GenVehicleState();
   ~GenVehicleState();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
 int m_stateOfAllSystems[NUMBER_OF_OTHER_SYSTEMS];
 int m_state;
 bool m_RunMission;
 bool m_GoToPoint;
 bool m_Return;
 bool m_RemoteControl;
 bool m_Scanning;
 
 int m_internalState;
 bool m_stopOnSurfaceDone;
 bool m_RemoteJoyDone;
 bool m_RemoteParametersDone;
 bool m_NotScanningDone;
 bool m_AllStopDone;
 bool m_BehaviorError;
 
 int m_timeStart;
 float m_depth;
 
 int getStateOfVehicle();
 string genShortVehicleReport();
 void setAllStop();
 
 void actionInState();
 void stopOnSurface();
 void remoteJoy();
 void remoteParameters();
 void notScanning();
};

#endif 
