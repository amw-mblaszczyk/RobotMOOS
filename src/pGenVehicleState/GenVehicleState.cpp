/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenVehicleState.cpp                                        */
/*    DATE:                                                 */
/************************************************************/
using namespace std;
#include <iterator>
#include <iostream>
#include "MBUtils.h"
#include "GenVehicleState.h"

//---------------------------------------------------------
// Constructor

GenVehicleState::GenVehicleState()
{
	for(int i=0;i<NUMBER_OF_OTHER_SYSTEMS;i++)
		m_stateOfAllSystems[i] = 0;
	m_state = STATE_OK;
	
	m_RunMission = false;
	m_GoToPoint = false;
	m_Return = false;
	m_RemoteControl = true;
	m_Scanning = false;
	m_internalState = INTERNAL_NOTHING;
	m_stopOnSurfaceDone = false;
	m_RemoteJoyDone = false;
	m_RemoteParametersDone = false;
	m_NotScanningDone = false;
	m_AllStopDone = false;
	m_BehaviorError = false;
	m_depth = 0;
}

//---------------------------------------------------------
// Destructor

GenVehicleState::~GenVehicleState()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenVehicleState::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

	string key   = msg.GetKey();
	bool   mdbl  = msg.IsDouble();
	bool   mstr  = msg.IsString();

	if(key == "REPORT_NAV" && mdbl)
		m_stateOfAllSystems[NAVIGATION] = (int)msg.GetDouble();
	else if(key == "REPORT_MAG" && mdbl)
		m_stateOfAllSystems[MAGNETOMETER] = (int)msg.GetDouble();
	else if(key == "REPORT_CAM" && mdbl)
		m_stateOfAllSystems[CAMERA] = (int)msg.GetDouble();
	else if(key == "REPORT_LIGHTS" && mdbl)
		m_stateOfAllSystems[LIGHTS] = (int)msg.GetDouble();
	else if(key == "REPORT_SON" && mdbl)
		m_stateOfAllSystems[SONAR] = (int)msg.GetDouble();
	else if(key == "REPORT_BAT" && mdbl)
		m_stateOfAllSystems[BATTERY] = (int)msg.GetDouble();
	else if(key == "REPORT_REG" && mdbl)
		m_stateOfAllSystems[CONTROLLER] = (int)msg.GetDouble();
	else if(key == "REPORT_ENG" && mdbl)
		m_stateOfAllSystems[ENGINE] = (int)msg.GetDouble();
	else if(key == "NAV_DEPTH" && mdbl)
		m_depth = msg.GetDouble();
	else if(mstr)
		{
		string sval  = msg.GetString(); 
		if(key == "RUN_MISSION")
			{
			if(sval == "true")
				m_RunMission = true;
			else
				m_RunMission = false;
			}
		else if(key == "GO_TO_POINT")
			{
			if(sval == "true")
				m_GoToPoint = true;
			else
				m_GoToPoint = false;
			}
		else if(key == "RETURN")
			{
			if(sval == "true")
				m_Return = true;
			else
				m_Return = false;
			}
		else if(key == "REMOTE_CONTROL")
			{
			if(sval == "true")
				m_RemoteControl = true;
			else
				m_RemoteControl = false;
			}
		else if(key == "SCANNING")
			{
			if(sval == "true")
				m_Scanning = true;
			else
				m_Scanning = false;
			}
		else if(key == "IVPHELM_ALLSTOP")
			{
			if(sval == "BehaviorError")
				{
				m_BehaviorError = true;
				}
			}
		}
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenVehicleState::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool GenVehicleState::Iterate()
{
	m_state = getStateOfVehicle();
	Notify("VEHICLE_STATE", m_state);
	actionInState();
	Notify("REPORT_SHORT", genShortVehicleReport());
	setAllStop();
	return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenVehicleState::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void GenVehicleState::RegisterVariables()
{
	Register("REPORT_NAV", 0);
	Register("REPORT_MAG", 0);
	Register("REPORT_LIGHT", 0);
	Register("REPORT_CAM", 0);
	Register("REPORT_SON", 0);
	Register("REPORT_BAT", 0);
	Register("REPORT_REG", 0);
	Register("REPORT_ENG", 0);
	
	Register("RUN_MISSION", 0);
	Register("GO_TO_POINT", 0);
	Register("RETURN", 0);
	Register("REMOTE_CONTROL", 0);
	Register("SCANNING", 0);
	Register("NAV_DEPTH", 0);
	Register("IVPHELM_ALLSTOP", 0);
}

int GenVehicleState::getStateOfVehicle()
{
	if(m_stateOfAllSystems[ENGINE] != 0 || m_stateOfAllSystems[BATTERY] != 0) 
		return STATE_ERROR_STOP_ON_SURFACE;
	if(m_stateOfAllSystems[NAVIGATION] != 0 || m_stateOfAllSystems[CONTROLLER] != 0)
		return STATE_ERROR_REMOTE_JOY;
	if(m_stateOfAllSystems[SONAR] != 0 || m_BehaviorError) 
		return STATE_ERROR_REMOTE_PARAMETERS;
	if(m_stateOfAllSystems[MAGNETOMETER] != 0 || m_stateOfAllSystems[CAMERA] != 0 || m_stateOfAllSystems[LIGHTS] != 0) 
		return STATE_ERROR_MISSION_NOT_SCANNING;
	return STATE_OK;
}

void GenVehicleState::actionInState()
{
	if(m_state >= STATE_ERROR_STOP_ON_SURFACE)
		millipause(DELAY);
	
	if(m_state != STATE_OK)
		switch(m_state)
			{
		case STATE_ERROR_STOP_ON_SURFACE: stopOnSurface();
			break;
		case STATE_ERROR_REMOTE_JOY: remoteJoy();
			break;
		case STATE_ERROR_REMOTE_PARAMETERS: remoteParameters();
			break;
		case STATE_ERROR_MISSION_NOT_SCANNING: notScanning();
			break;
			}
}

void GenVehicleState::stopOnSurface()
{
	if(!m_stopOnSurfaceDone)
		{
		Notify("RUN_MISSION", "false");
		Notify("GO_TO_POINT", "false");
		Notify("RETURN", "false");
		Notify("REMOTE_CONTROL", "false");
		Notify("MOOS_MANUAL_OVERRIDE","true"); 
		millipause(DELAY);
		
		if(m_stateOfAllSystems[CONTROLLER] == 0)
			{
			Notify("CONTROLLER_ACTIVE","true");
			millipause(DELAY);
			Notify("DESIRED_DEPTH",0.0);
			Notify("DESIRED_SPEED",0.0);
			}
		else
			{
			Notify("CONTROLLER_ACTIVE","false");
			millipause(DELAY);
			Notify("DESIRED_THRUST",0.0);
			Notify("DESIRED_ELEVATOR",-10.0);
			}
		m_stopOnSurfaceDone = true;
		}
}

void GenVehicleState::remoteJoy()
{
	if(!m_RemoteJoyDone)
		{
		if(m_internalState == INTERNAL_NOTHING)
			{
			Notify("REMOTE_CONTROL","true");
			millipause(DELAY);
			Notify("MOOS_MANUAL_OVERRIDE","true"); 
			Notify("CONTROLLER_ACTIVE","false");
			millipause(DELAY);
			Notify("DESIRED_THRUST",0.0);
			Notify("DESIRED_ELEVATOR",-10.0);
			m_internalState = INTERNAL_GO_UP;
			
			Notify("RUN_MISSION", "false");
			Notify("GO_TO_POINT", "false");
			Notify("RETURN", "false");
			}
		else
			{
			if(m_depth <= 0)
				{
				m_internalState = INTERNAL_NOTHING;
				m_RemoteJoyDone = true;
				}
			}
		}
}

void GenVehicleState::remoteParameters()
{
	if(!m_RemoteParametersDone && !m_RemoteControl && m_RunMission && !m_GoToPoint && !m_Return)
		{
		if(m_internalState == INTERNAL_NOTHING)
			{
			Notify("REMOTE_CONTROL","true");
			millipause(DELAY);
			Notify("MOOS_MANUAL_OVERRIDE","true"); 
			Notify("CONTROLLER_ACTIVE","true");
			millipause(DELAY);
			Notify("DESIRED_DEPTH",0.0);
			Notify("DESIRED_SPEED",0.0);
			m_internalState = INTERNAL_GO_UP;
			
			Notify("RUN_MISSION", "false");
			Notify("GO_TO_POINT", "false");
			Notify("RETURN", "false");
			}
		else
			{
			if(m_depth <= 0)
				{
				m_internalState = INTERNAL_NOTHING;
				m_RemoteParametersDone = true;
				}
			}
		}
}

void GenVehicleState::notScanning()
{
	if(!m_RemoteControl && m_RunMission && !m_GoToPoint && !m_Return && m_Scanning && !m_NotScanningDone)
		{
		if(m_internalState == INTERNAL_NOTHING)
			{
			Notify("REMOTE_CONTROL","true");
			millipause(DELAY);
			Notify("MOOS_MANUAL_OVERRIDE","true"); 
			Notify("CONTROLLER_ACTIVE","true");
			millipause(DELAY);
			Notify("DESIRED_DEPTH",0.0);
			Notify("DESIRED_SPEED",0.0);
			m_internalState = INTERNAL_GO_UP;
			
			Notify("SCANNING", "false");
			}
		else
			{
			if(m_depth <= 0)
				{
				m_internalState = INTERNAL_NOTHING;
				m_NotScanningDone = true;
				}
			}
		}
}

void GenVehicleState::setAllStop()
{
	if(m_state == STATE_ERROR_STOP_ON_SURFACE && !m_AllStopDone)
		{
		if(m_internalState == INTERNAL_NOTHING)
			{
			m_timeStart = time(NULL);
			m_internalState = INTERNAL_GO_UP;
			}
		else
			{
			int currentTime = time(NULL);
			if(currentTime - m_timeStart > MAX_TIME_TO_APPEAR_ON_SURFACE && m_depth > 0)
				{
				Notify("VEHICLE_STATE", STATE_ERROR_ALL_STOP);
				m_internalState = INTERNAL_NOTHING;
				m_AllStopDone = true;
				}
			}
		}
}

string GenVehicleState::genShortVehicleReport()
{
	string returnString("");
	if(m_RunMission)
		returnString += "1|";
	else
		returnString += "0|";
	if(m_GoToPoint)
		returnString += "1|";
	else
		returnString += "0|";
	if(m_Return)
		returnString += "1|";
	else
		returnString += "0|";
	if(m_RemoteControl)
		returnString += "1|";
	else
		returnString += "0|";
	if(m_Scanning)
		returnString += "1|";
	else
		returnString += "0|";
	for(int i=0;i<NUMBER_OF_OTHER_SYSTEMS;i++)
		returnString += intToString(m_stateOfAllSystems[i]) + "|";
	return returnString;
}

