/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: RemoteVehicle.cpp                                        */
/*    DATE:                                                 */
/************************************************************/
#define Tests_On_The_Surface


#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "RemoteVehicle.h"

using namespace std;

//---------------------------------------------------------
// Constructor

RemoteVehicle::RemoteVehicle()
{
	m_maxVehicleSpeed = MAX_VEHICLE_SPEED;
	m_maxRangeOfVehicle = MAX_RANGE_OF_VEHICLE;
	m_rudderStep = RUDDER_STEP;
	m_currentRudder = 0;
	m_maxRudder = MAX_RUDDER;
	m_thrustStep = THRUST_STEP;
	m_currentThrust = 0;
	m_maxThrust = MAX_THRUST;
	m_minThrust = MIN_THRUST;
	m_elevatorStep = ELEVATOR_STEP;
	m_currentElevator = 0;
	m_maxElevator = MAX_ELEVATOR;
	m_maxVehicleDepth = MAX_DEPTH;
	m_state = STATE_OK;
	
	m_vehicleDepth = 0;
	m_isMission = false; 
	m_msgRemote = NULL;
}

//---------------------------------------------------------
// Destructor

RemoteVehicle::~RemoteVehicle()
{
	if(m_msgRemote != NULL)
		delete m_msgRemote;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool RemoteVehicle::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
   string key    = msg.GetKey();
	bool   mstr  = msg.IsString();
	bool   mdbl  = msg.IsDouble();
	string sdata = msg.m_sVal;

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(mstr)
		{
		string sval  = msg.GetString(); 
		if(key == "REMOTE")
			{
#ifdef Tests_On_The_Surface	
			if(!m_isMission)
#else
			if(m_vehicleDepth <=0 && !m_isMission)
#endif		
				{
				if(MsgRemote::isForMe(sval,m_host_community))
					{
					if(m_msgRemote != NULL)
						delete m_msgRemote;
					m_msgRemote = new MsgRemote(sval);
					}
				}
			else
				reportRunWarning("REMOTE ERROR: Remotely operated mode can be only run on the surface and if mission is off");
			}
		else if(key == "RUN_MISSION")
			{
			if(sval == "true")
				m_isMission = true;
			else
				m_isMission = false;
			}
		}
	else if(key == "NAV_DEPTH" && mdbl)
		m_vehicleDepth = msg.GetDouble();
	else if(key == "VEHICLE_STATE" && mdbl)
		m_state = (int)msg.GetDouble();
	else if(key == "DESIRED_RUDDER" && mdbl)
		m_currentRudder = msg.GetDouble();
	else if(key == "DESIRED_THRUST" && mdbl)
		m_currentThrust = msg.GetDouble();
#ifdef Tests_On_The_Surface
	else if(key == "DESIRED_ELEVATOR" && mdbl)
		m_currentElevator = msg.GetDouble();
#endif
    else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool RemoteVehicle::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool RemoteVehicle::Iterate()
{
  AppCastingMOOSApp::Iterate();
  
  if(m_msgRemote != NULL)
	{
	if(!m_msgRemote->isParsingError())
		runRemote();
	else
		reportRunWarning("REMOTE ERROR:"+m_msgRemote->getParsingError());
	delete m_msgRemote;
	m_msgRemote = NULL;
	}
  
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool RemoteVehicle::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "MAX_VEHICLE_SPEED") {
      handled = true;
	  if(isNumber(value,false))
		m_maxVehicleSpeed = atof(value.c_str());
	  else
		reportConfigWarning("MAX_VEHICLE_SPEED is not a number");
    }
	else if(param == "MAX_RANGE_OF_VEHICLE") {
      handled = true;
	  if(isNumber(value,false))
		m_maxRangeOfVehicle = fabs(atof(value.c_str()));
	else
		reportConfigWarning("MAX_RANGE_OF_VEHICLE is not a number");
    }
	else if(param == "MAX_RUDDER") {
      handled = true;
	  if(isNumber(value,false))
		m_maxRudder = atof(value.c_str());
	
    }
	else if(param == "RUDDER_STEP") {
      handled = true;
	  if(isNumber(value,false))
		m_rudderStep = fabs(atof(value.c_str()));
	else
		reportConfigWarning("RUDDER_STEP is not a number");
    }
	else if(param == "MAX_THRUST") {
      handled = true;
	  if(isNumber(value,false))
		m_maxThrust = fabs(atof(value.c_str()));
	else
		reportConfigWarning("MAX_THRUST is not a number");
    }
	else if(param == "MIN_THRUST") {
      handled = true;
	  if(isNumber(value,false))
		m_minThrust = atof(value.c_str());
	else
		reportConfigWarning("MIN_THRUST is not a number");
    }
	else if(param == "THRUST_STEP") {
      handled = true;
	  if(isNumber(value,false))
		m_thrustStep = fabs(atof(value.c_str()));
	else
		reportConfigWarning("THRUST_STEP is not a number");
    }
	else if(param == "MAX_ELEVATOR") {
      handled = true;
	  if(isNumber(value,false))
		m_maxElevator = fabs(atof(value.c_str()));
	else
		reportConfigWarning("MAX_ELEVATOR is not a number");
    }
	else if(param == "ELEVATOR_STEP") {
      handled = true;
	  if(isNumber(value,false))
		m_elevatorStep = fabs(atof(value.c_str()));
	else
		reportConfigWarning("ELEVATOR_STEP is not a number");
    }
	else if(param == "MAX_VEHICLE_DEPTH") {
      handled = true;
	  if(isNumber(value,false))
		m_maxVehicleDepth = fabs(atof(value.c_str()));
	else
		reportConfigWarning("MAX_VEHICLE_DEPTH is not a number");
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void RemoteVehicle::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("REMOTE", 0);
  Register("NAV_DEPTH", 0);
  Register("RUN_MISSION", 0);
  
  Register("VEHICLE_STATE", 0);
  
  Register("DESIRED_RUDDER", 0);
  Register("DESIRED_THRUST", 0);
#ifdef Tests_On_The_Surface
  Register("DESIRED_ELEVATOR", 0);
#endif
}


//------------------------------------------------------------
// Procedure: buildReport()

bool RemoteVehicle::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "RemoteVehicle                                       \n";
  m_msgs << "============================================ \n";

  /*ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();*/

  return(true);
}

void RemoteVehicle::runRemote()
{
	m_erroInParsingInputData = "";
	
	if(m_msgRemote->isMagnetometerSet())
		{
		if(m_msgRemote->getMagnetometer())
			{
			Notify("MAG_SON_ON","true");
			reportEvent("REMOTE: Magnetometer/Sonar ON");
			}
		else
			{
			Notify("MAG_SON_ON","false");
			reportEvent("REMOTE: Magnetometer/Sonar OFF");
			}
		}
		
	if(m_msgRemote->isCameraSet())
		{
		if(m_msgRemote->getCamera())
			{
			Notify("CAMERA_ON","true");
			reportEvent("REMOTE: Camera ON");
			}
		else
			{
			Notify("CAMERA_ON","false");
			reportEvent("REMOTE: Camera OFF");
			}
		}
#ifdef Tests_On_The_Surface		
	if((m_msgRemote->isJoyElevatorSet() || m_msgRemote->isJoyRudderSet() || m_msgRemote->isJoyThrustSet()))
#else
	if((m_msgRemote->isJoyRudderSet() || m_msgRemote->isJoyThrustSet()))
#endif
		{
		if(m_state <= STATE_ERROR_REMOTE_JOY)
			{
			bool okRudder = false;
			if(m_msgRemote->isJoyRudderSet())
				if(m_msgRemote->getJoyRudder() == 0 && fabs(m_currentRudder - m_rudderStep) <= m_maxRudder)//left
					{
					okRudder = true;
					m_currentRudder = m_currentRudder - m_rudderStep;
					reportEvent("REMOTE: Rudder is left: rudder=" + doubleToString(m_currentRudder));
					}
				else if(m_msgRemote->getJoyRudder() == 2 && fabs(m_currentRudder + m_rudderStep) <= m_maxRudder)//right
					{
					okRudder = true;
					m_currentRudder = m_currentRudder + m_rudderStep;
					reportEvent("REMOTE: Rudder is right: rudder=" + doubleToString(m_currentRudder));
					}
				else if(m_msgRemote->getJoyRudder() == 1)
					{
					okRudder = true;
					m_currentRudder = 0;
					reportEvent("REMOTE: Rudder is zero");
					}
				else
					m_erroInParsingInputData += "Wrong operation on Rudder|";
				
			bool okThrust = false;
			if(m_msgRemote->isJoyThrustSet())	
				if(m_msgRemote->getJoyThrust() == 0 && fabs(m_currentThrust + m_thrustStep) <= m_maxThrust)
					{
					okThrust = true;
					m_currentThrust = m_currentThrust + m_thrustStep;
					reportEvent("REMOTE: Thrust is forward: thrust=" + doubleToString(m_currentThrust));
					}
				else if(m_msgRemote->getJoyThrust() == 2 && fabs(m_currentThrust - m_thrustStep) >= m_minThrust)
					{
					okThrust = true;
					m_currentThrust = m_currentThrust - m_thrustStep;
					reportEvent("REMOTE: Thrust is back: thrust=" + doubleToString(m_currentThrust));
					}
				else if(m_msgRemote->getJoyThrust() == 1)
					{
					okThrust = true;
					m_currentThrust = 0;
					reportEvent("REMOTE: Thrust is zero");
					}
				else
					m_erroInParsingInputData += "Wrong operation on Thrust=" + doubleToString(m_currentThrust) + " " + doubleToString(m_thrustStep) + " " + doubleToString(fabs(m_currentThrust - m_thrustStep)) + " " + doubleToString(m_minThrust) + "|";
				
				
#ifdef Tests_On_The_Surface
			bool okElevator = false;
			if(m_msgRemote->isJoyElevatorSet())	
				if(m_msgRemote->getJoyElevator() == 0 && fabs(m_currentElevator + m_elevatorStep) <= m_maxElevator)
					{
					okElevator = true;
					m_currentElevator = m_currentElevator + m_elevatorStep;
					reportEvent("REMOTE: Elevator is down: elevator=" + doubleToString(m_currentElevator));
					}
				else if(m_msgRemote->getJoyElevator() == 2 && fabs(m_currentElevator - m_elevatorStep) <= m_maxElevator)
					{
					okElevator = true;
					m_currentElevator = m_currentElevator - m_elevatorStep;
					reportEvent("REMOTE: Elevator is up: elevator=" + doubleToString(m_currentElevator));
					}
				else if(m_msgRemote->getJoyElevator() == 1)
					{
					okElevator = true;
					m_currentElevator = 0;
					reportEvent("REMOTE: Elevator is zero");
					}
				else
					m_erroInParsingInputData += "Wrong operation on Elevator|";
#endif

#ifdef Tests_On_The_Surface
			if(okRudder || okThrust || okElevator)
#else
			if(okRudder || okThrust)
#endif
				{
				Notify("CONTROLLER_ACTIVE","false");
				Notify("REMOTE_CONTROL","true");
				Notify("GO_TO_POINT","false");
				millipause(DELAY);
				Notify("MOOS_MANUAL_OVERRIDE","true");
				millipause(DELAY);
				}
				
			if(okRudder)
				Notify("DESIRED_RUDDER",m_currentRudder);
			if(okThrust)
				Notify("DESIRED_THRUST",m_currentThrust);
#ifdef Tests_On_The_Surface
			if(okElevator)
				Notify("DESIRED_ELEVATOR",m_currentElevator);
#endif
			}
		else
			m_erroInParsingInputData += "REMOTE ERROR: the state of the vehicle unables remotely operated mode|";
		}
#ifdef Tests_On_The_Surface	
	else if((m_msgRemote->isCourseSet() || m_msgRemote->isDepthSet() || m_msgRemote->isSpeedSet()) && (!m_msgRemote->isPointSet()))
#else
	else if((m_msgRemote->isCourseSet() || m_msgRemote->isSpeedSet()) && (!m_msgRemote->isPointSet()))
#endif
		{
		if(m_state <= STATE_ERROR_REMOTE_PARAMETERS)
			{
			bool okCourse = false;
			if(m_msgRemote->isCourseSet())
				okCourse = true;
				
			bool okSpeed = false;	
			if(m_msgRemote->isSpeedSet())
				if(m_msgRemote->getSpeed()<=m_maxVehicleSpeed)
					okSpeed = true;
				else
					m_erroInParsingInputData += "Remote Speed is out of the range|";
				
#ifdef Tests_On_The_Surface
			bool okDepth = false;
			if(m_msgRemote->isDepthSet())
				if(m_msgRemote->getDepth()<=m_maxVehicleDepth)
					okDepth = true;
				else
					m_erroInParsingInputData += "Remote Depth is out of the range|";
#endif
			
#ifdef Tests_On_The_Surface
			if(okCourse || okSpeed || okDepth)
#else
			if(okCourse || okSpeed)
#endif
				{
				Notify("CONTROLLER_ACTIVE","true");
				Notify("REMOTE_CONTROL","true");
				Notify("GO_TO_POINT","false");
				millipause(DELAY);
				Notify("MOOS_MANUAL_OVERRIDE","true");
				millipause(DELAY);
				}
			if(okCourse)
				{
				Notify("DESIRED_HEADING",m_msgRemote->getCourse());
				reportEvent("REMOTE: Heading =" + doubleToString(m_msgRemote->getCourse()));
				}
				
			if(okSpeed)
				{
				Notify("DESIRED_SPEED",m_msgRemote->getSpeed());
				reportEvent("REMOTE: Speed =" + doubleToString(m_msgRemote->getSpeed()));
				}
				
#ifdef Tests_On_The_Surface
			if(okDepth)
				{
				Notify("DESIRED_DEPTH",m_msgRemote->getDepth());
				reportEvent("REMOTE: Depth =" + doubleToString(m_msgRemote->getDepth()));
				}
#endif
			}
		else
			m_erroInParsingInputData += "REMOTE ERROR: the state of the vehicle unables remotely operated mode|";
		}
	else if(m_msgRemote->isPointSet() && m_msgRemote->isSpeedSet())
		{
		if(m_state <= STATE_ERROR_REMOTE_PARAMETERS)
			{
			bool  isPoint = false;
			bool isSpeed = false;
			bool isDepth = false;

			if(fabs(m_msgRemote->getPointX()) <= m_maxRangeOfVehicle && fabs(m_msgRemote->getPointY()) <= m_maxRangeOfVehicle)
				isPoint = true;
			else
				m_erroInParsingInputData += "Remote Point is out of the range|";
			if(m_msgRemote->getSpeed()<=m_maxVehicleSpeed)
				isSpeed = true;
			else
				m_erroInParsingInputData += "Remote Speed is out of the range|";
#ifdef Tests_On_The_Surface		
			if(m_msgRemote->isDepthSet())
				if(m_msgRemote->getDepth()<=m_maxVehicleDepth)
					isDepth = true;
#endif
			
			if(isSpeed && isPoint)
				{
				m_goToPointString = "points=" + doubleToString(m_msgRemote->getPointX()) + "," + doubleToString(m_msgRemote->getPointY());
				m_goToPointSpeed = "speed=" + doubleToString(m_msgRemote->getSpeed());
				
#ifdef Tests_On_The_Surface
				if(isDepth)
					m_goToPointDepth = "depth=" + doubleToString(m_msgRemote->getDepth());
				else
					m_goToPointDepth = "";
#endif
				sendRemoteGoToPoint();
				}
			}
		else
			m_erroInParsingInputData += "REMOTE ERROR: the state of the vehicle unables remotely operated mode|";
		}
		
	if(m_erroInParsingInputData.size() > 0)
		reportRunWarning(m_erroInParsingInputData);
}

void RemoteVehicle::sendRemoteGoToPoint()
{
	Notify("MOOS_MANUAL_OVERRIDE","false");
	millipause(DELAY);
	
	Notify("POINT_SURFACE_UPDATE",m_goToPointString);
	Notify("POINT_SURFACE_UPDATE",m_goToPointSpeed);
	
#ifdef Tests_On_The_Surface
	if(m_goToPointDepth.length() > 1)
		Notify("CONSTANT_DEPTH_UPDATE",m_goToPointDepth);
#endif
			
	Notify("REMOTE_CONTROL","false");
	Notify("CONTROLLER_ACTIVE","true");
	Notify("GO_TO_POINT","true");
	reportEvent("REMOTE: " + m_goToPointString + " " + m_goToPointSpeed);
}




