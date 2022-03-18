/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: RemoteVehicle.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef RemoteVehicle_HEADER
#define RemoteVehicle_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MsgRemote.h"

#define MAX_VEHICLE_SPEED 2
#define MAX_RANGE_OF_VEHICLE 500
#define RUDDER_STEP 5
#define MAX_RUDDER 100
#define THRUST_STEP 5
#define MAX_THRUST 100
#define MIN_THRUST -50
#define ELEVATOR_STEP 5
#define MAX_ELEVATOR 100
#define MAX_DEPTH 10

#define STATE_OK 0
#define STATE_ERROR_MISSION_NOT_SCANNING 1
#define STATE_ERROR_REMOTE_PARAMETERS 2
#define STATE_ERROR_REMOTE_JOY 3
#define STATE_ERROR_STOP_ON_SURFACE 4
#define STATE_ERROR_ALL_STOP 5

#define DELAY 500

class RemoteVehicle : public AppCastingMOOSApp
{
 public:
   RemoteVehicle();
   ~RemoteVehicle();

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
 
	float m_maxVehicleSpeed;
	float m_maxVehicleDepth;
	float m_maxRangeOfVehicle;
	float m_rudderStep;
	float m_currentRudder;
	float m_maxRudder;
	float m_thrustStep;
	float m_currentThrust;
	float m_maxThrust;
	float m_minThrust;
	float m_elevatorStep;
	float m_currentElevator;
	float m_maxElevator;
	
	double m_vehicleDepth;
	bool m_isMission;
	
	string m_erroInParsingInputData;
	int m_state;

 private: // State variables
	MsgRemote* m_msgRemote;
	string m_goToPointString;
	string m_goToPointSpeed;
	string m_goToPointDepth;
	//bool m_sendRemoteGoToPoint;
	
 private:
	void runRemote();
	void sendRemoteGoToPoint();
};

#endif 
