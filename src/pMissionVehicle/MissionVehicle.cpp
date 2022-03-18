/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: MissionVehicle.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "XYSegList.h"
#include "XYPoint.h"
#include "MissionVehicle.h"

#include <sys/time.h>


using namespace std;

//---------------------------------------------------------
// Constructor

MissionVehicle::MissionVehicle()
{
	m_vehicleDepth = 0;
	m_isMission = false; 
	m_state = STATE_OK;
	m_MissionReady = false;
	m_MissionMsg = NULL;
}

//---------------------------------------------------------
// Destructor

MissionVehicle::~MissionVehicle()
{
	if(m_MissionMsg != NULL)
		delete m_MissionMsg;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool MissionVehicle::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

	bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();

     if(mstr)
		{
		string sval  = msg.GetString(); 
		//reportEvent(sval);
		if(key == "MISSION")
			{
			//reportEvent("jestem1");
			if(isForMe(sval,m_host_community))
				{
				//reportEvent("jestem2");
				short action;
				//Notify(ALA1,sval);
				if(SearchAreaTask::isMissionStartOrStop(sval,action))
					{
					//Notify(ALA,"Stop or Start");
					//reportEvent("Stop or Start");
					bool isRun = false;
					if(action == 1)
						{
						if(!m_isMission && m_MissionReady && m_vehicleDepth <=0)
							{
							if((m_state == STATE_OK && m_MissionMsg->isDistanceSet()) || (m_state <= STATE_ERROR_MISSION_NOT_SCANNING && m_MissionMsg->isDepthSet()))
								{
								pthread_t tid;            
								pthread_create(&tid, NULL, MissionVehicle::runMission, this);
								isRun = true;
								}
							else
								reportRunWarning("MISSION ERROR: the state of vehicle unables running mission");
							}
						else
							{
							string error = "";
							if(m_isMission)
								error += "Mission is on and cannot be re-run|";
							if(!m_MissionReady)
								error += "Mission is not ready|";
							if(m_vehicleDepth > 0)
								error += "Mission can be run only on the surface|";
							reportRunWarning("MISSION ERROR:"+error);
							}
						}
					else 
						{
						if(m_isMission)
							stopMission();
						else
							reportRunWarning("MISSION ERROR: Mission cannot be stopped in remote control mode");
						}
					
					if(!isRun && m_MissionMsg != NULL)
						{
						delete m_MissionMsg;
						m_MissionMsg = NULL;
						}
						
					m_MissionReady = false;
					}
				else 
					{
					//reportEvent("jestem3");
					if(!m_isMission && m_vehicleDepth <=0)
						{
						if(m_MissionMsg != NULL)
							{
							delete m_MissionMsg;
							m_MissionMsg = NULL;
							}
						m_MissionMsg = new SearchAreaTask(sval);
						if(!m_MissionMsg->isParsingError() && m_MissionMsg->isMissionValidForVehicle())
							{
							if((m_state == STATE_OK && m_MissionMsg->isDistanceSet()) || (m_state <= STATE_ERROR_MISSION_NOT_SCANNING && m_MissionMsg->isDepthSet()))
								{
								m_MissionReady = true;
								reportEvent("Mission is ready to run");
								}
							else
								reportRunWarning("MISSION ERROR: the state of vehicle unables saving mission");
							}
						else
							{
							if(m_MissionMsg->isParsingError())
								reportRunWarning("MISSION ERROR:"+m_MissionMsg->getParsingError());
							if(!m_MissionMsg->isMissionValidForVehicle())
								reportRunWarning("MISSION ERROR: some mandatory mission parameters are missing");
							delete m_MissionMsg;
							m_MissionMsg = NULL;
							m_MissionReady = false;
							}
						}
					else
						{
						string error = "";
						if(m_isMission)
							error += "Mission is on and cannot be saved before stopping|";
						if(m_vehicleDepth > 0)
							error += "Mission can be saved only on the surface|";
						reportRunWarning("MISSION ERROR:"+error);
						}
					}
				}
			}
		else if(key == "RUN_MISSION")
			{
			if(sval == "false" && m_isMission)
				m_isMission = false;
			}
		}
	else if(key == "NAV_DEPTH" && mdbl)
		m_vehicleDepth = msg.GetDouble();
	else if(key == "VEHICLE_STATE" && mdbl)
		m_state = (int)msg.GetDouble();
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool MissionVehicle::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool MissionVehicle::Iterate()
{
  AppCastingMOOSApp::Iterate();
  
  Notify("REPORT_MISSION",generateReportMission());
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool MissionVehicle::OnStartUp()
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
    if(param == "FOO") {
      handled = true;
    }
    else if(param == "BAR") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void MissionVehicle::registerVariables()
{
	AppCastingMOOSApp::RegisterVariables();
	Register("MISSION", 0);
	Register("NAV_DEPTH", 0);
  
	Register("VEHICLE_STATE", 0);
	Register("RUN_MISSION", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool MissionVehicle::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  //ACTable actab(4);
  //actab << "Alpha | Bravo | Charlie | Delta";
  //actab.addHeaderLines();
  //actab << "one" << "two" << "three" << "four";
  //m_msgs << actab.getFormattedString();

  return(true);
}

bool MissionVehicle::isForMe(string msg, string my_name) {
	if(strContains(msg,R_WHO)){
		string who = tokStringParse(msg, R_WHO, ',', '=');
		if(who == R_SWARM || who == my_name)
			return true;
	}
	return false;
}

void MissionVehicle::stopMission()
{
	Notify("REMOTE_CONTROL","true");
	millipause(DELAY);	
	Notify("MOOS_MANUAL_OVERRIDE","true");
	
	Notify("CONTROLLER_ACTIVE","false");
	Notify("DESIRED_RUDDER", 0.0);
	Notify("DESIRED_THRUST", 0.0);
	Notify("DESIRED_ELEVATOR", 0.0);
		
	Notify("RUN_MISSION","false");
	Notify("GO_TO_POINT","false");
	Notify("RETURN","false");
		
	reportEvent("Mission is stopped by operator");
	m_isMission = false;
}

void* MissionVehicle::runMission(void* me)
{
	MissionVehicle *missionVehicle = (MissionVehicle *)me;
	
	XYSegList list = missionVehicle->m_MissionMsg->getPath();
	string missionString("points=");
	missionString += list.get_spec();
	//missionVehicle->reportEvent(missionString);
	
	
	string returnPointString("points=");
	returnPointString = returnPointString + doubleToString(missionVehicle->m_MissionMsg->getEnd_point().get_vx()) + "," + doubleToString(missionVehicle->m_MissionMsg->getEnd_point().get_vy());
	
	struct timeval currentTime, startTime;
	long mtime, seconds, useconds;
	
	startTime = missionVehicle->m_MissionMsg->getTimeStart();
	
	if(startTime.tv_sec > 0 || startTime.tv_usec > 0)
		{
		gettimeofday(&currentTime, NULL);
		
		seconds  = startTime.tv_sec  - currentTime.tv_sec;
		useconds = startTime.tv_usec - currentTime.tv_usec;

		mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
		
		if(mtime > 0)
			millipause(mtime);
		}
	
	missionVehicle->Notify("MOOS_MANUAL_OVERRIDE","false"); 
	millipause(DELAY);
	
	if(missionVehicle->m_MissionMsg->isDistanceSet())
		{
		missionVehicle->Notify("CONTROLLER_ACTIVE","false");
		missionVehicle->Notify("SCANNING","true");
		missionVehicle->Notify("SCANNING_UPDATE",missionString);
		missionVehicle->Notify("SCANNING_UPDATE","speed=" + doubleToString(missionVehicle->m_MissionMsg->getSpeed()));
		missionVehicle->Notify("SCANNING_UPDATE","distance=" + doubleToString(missionVehicle->m_MissionMsg->getDistance()));
		//missionVehicle->Notify("CONSTANT_DISTANCE_UPDATE",missionVehicle->m_MissionMsg->getDistance());
		}
	else
		{
		missionVehicle->Notify("CONTROLLER_ACTIVE","true");
		missionVehicle->Notify("SCANNING","false");
		missionVehicle->Notify("WPT_UPDATE",missionString);
		missionVehicle->Notify("WPT_UPDATE","speed=" + doubleToString(missionVehicle->m_MissionMsg->getSpeed()));
		
		string missionDepth("depth=");
		missionDepth = missionDepth + doubleToString(missionVehicle->m_MissionMsg->getDepth());
		missionVehicle->Notify("CONSTANT_DEPTH_UPDATE",missionDepth);
		}
		
	missionVehicle->Notify("RUN_MISSION","true");
	missionVehicle->Notify("GO_TO_POINT","false");
	missionVehicle->Notify("RETURN","false");
	missionVehicle->Notify("REMOTE_CONTROL","false");
	
	missionVehicle->Notify("RETURN_UPDATE",returnPointString);
		
	string safetyRegionString("polygon=");
	safetyRegionString += missionVehicle->m_MissionMsg->getSafetyRegion().get_spec();
	missionVehicle->Notify("OPREGION_UPDATES",safetyRegionString);
	//missionVehicle->reportEvent(safetyRegionString);
	
	if(missionVehicle->m_MissionMsg->getMaxTime() > 0)
		{
		string maxTimeString("max_time=");
		maxTimeString += doubleToString(missionVehicle->m_MissionMsg->getMaxTime());
		missionVehicle->Notify("OPREGION_UPDATES",maxTimeString);
		}
	
	string maxDepthString("max_depth=");
	maxDepthString += doubleToString(missionVehicle->m_MissionMsg->getMaxDepth());
	missionVehicle->Notify("OPREGION_UPDATES",maxDepthString);

	missionVehicle->m_isMission = true;
	missionVehicle->reportEvent("Mission is run");
	
	delete missionVehicle->m_MissionMsg;
	missionVehicle->m_MissionMsg = NULL;
}

int MissionVehicle::generateReportMission()
{
	if(!m_isMission)
		{	
		if(m_MissionReady)
			return MISSION_OFF_READY;
		else
			return MISSION_OFF_NOT_READY;
		}
	else
		return MISSION_ON;
}




