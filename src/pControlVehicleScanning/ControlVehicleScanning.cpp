/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: ControlVehicleScanning.cpp                                        */
/*    DATE:                                                 */
/************************************************************/
using namespace std;

#include <iterator>
#include "MBUtils.h"
#include "AngleUtils.h"
#include "ACTable.h"
#include "XYSegList.h"
#include "XYPoint.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "GeomUtils.h"
#include "ControlVehicleScanning.h"

static void perpSegIntPt1(double x1, double y1, double x2, double y2, 
		  double qx, double qy, double& rx, double& ry)
{
	double delta_x = x2-x1;
	if(x1>=x2)
		delta_x = x1-x2;
	
	double delta_y = y2-y1;
	if(y1>=y2)
		delta_y = y1-y2;

  // handle the special case where the segment is vertical
  // Or nearly vertical
  if(delta_x < 0.0000001) {
    rx = x1;
	ry = qy;
    return;
  }

  // handle the special case where the segment is horizontal
  // Or nearly vertical
  if(delta_y < 0.0000001) { 
    ry = y1;
	rx = qx;
    return;
  }

  // Now handle the general case
  
  //printf("jestem\n");

  double seg_m = (y2-y1) / (x2-x1);
  double seg_b = y1 - (seg_m * x1);
  double oth_m = -1.0 / seg_m;
  double oth_b = qy - (oth_m * qx);
  rx = (oth_b - seg_b) / (seg_m - oth_m);
  ry = (oth_m * rx) + oth_b;
}

NeuralNetwork::NeuralNetwork(Macierz* myNet, int liczbaWejsc, int liczbaWyjsc)
{
	myNetwork = myNet;
	lWejsc = liczbaWejsc;
	lWyjsc = liczbaWyjsc;
}

NeuralNetwork::NeuralNetwork(string myNetName, int liczbaWejsc, int liczbaWyjsc)
{
	myNetwork = getMatrix(myNetName);
	lWejsc = liczbaWejsc;
	lWyjsc = liczbaWyjsc;
}

NeuralNetwork::NeuralNetwork(Macierz* myNet)
{
	myNetwork = myNet;
	lWejsc = -1;
	lWyjsc = -1;
}

void NeuralNetwork::setParameters(Macierz* myNet, int liczbaWejsc, int liczbaWyjsc)
{
	myNetwork = myNet;
	lWejsc = liczbaWejsc;
	lWyjsc = liczbaWyjsc;
}

NeuralNetwork::NeuralNetwork()
{
	myNetwork = NULL;
	lWejsc = 0;
	lWyjsc = 0;
}

Macierz* NeuralNetwork::getMatrix(string myNetName)
{
	Macierz* myMacierz = new Macierz;
	ifstream file(myNetName);
	string str; 
	
	int row_number = 0;
	int column_number = -1;
	while (std::getline(file, str))
		{
		vector<string> values = parseString(str,' ');
		if(column_number == -1)
			column_number = values.size();
		for(int i=0;i<column_number;i++)
			myMacierz->wartosci[row_number][i] = atof(values[i].c_str());
		row_number++;
		}
	myMacierz->liczbaWierszy = row_number;
	myMacierz->liczbaKolumn = column_number;
	return myMacierz;
}

void NeuralNetwork::setLWejsc(int ile)
{
	lWejsc = ile;
}
	
void NeuralNetwork::setLWyjsc(int ile)
{
	lWyjsc = ile;
}

NeuralNetwork::~NeuralNetwork()
{
	//delete []wyjsciaNeuronow;
	//free(wyjsciaNeuronow);
}
	
void NeuralNetwork::setWejscie(int nrWejscia, float wejscie)
{
	if(nrWejscia < lWejsc)
		wyjsciaNeuronow[nrWejscia] = wejscie;
	else
		printf("!!!!!!!!!!!!!blad NeuralNetwork::setWejscie");
}
	
float NeuralNetwork::getWyjscie(int nrWyjscia)
{
	if(nrWyjscia < lWyjsc)
		//return wyjsciaNeuronow[myNetwork->liczbaWierszy-nrWyjscia-1];
		return wyjsciaNeuronow[myNetwork->liczbaWierszy-lWyjsc+nrWyjscia];
 	else
		printf("!!!!!!!!!!!!!blad NeuralNetwork::getWyjscie = %d",nrWyjscia);
	
	return 0;
}
	
void NeuralNetwork::run()
{
	for(int i=lWejsc;i<myNetwork->liczbaWierszy;i++)
		{
		float parameter = myNetwork->wartosci[i][myNetwork->liczbaWierszy+1];
		if(parameter==0)
			parameter=1;	
			
		wyjsciaNeuronow[i] = sigmoidalna(i,1,myNetwork);//sieci wyjsciowe
		}
}

float NeuralNetwork::sigmoidalna(int index, double parametr, Macierz *mac)
{
	double returnWartosc = 0;
	for(int j=0;j<index;j++)
		returnWartosc = returnWartosc + mac->wartosci[j][index]*wyjsciaNeuronow[j];
	returnWartosc = returnWartosc + mac->wartosci[index][myNetwork->liczbaWierszy];
	
	return (2/(1+exp(-parametr*returnWartosc)))-1;
}

float NeuralNetwork::sigmoidalna1(int index, double parametr, Macierz *mac)
{
	double returnWartosc = 0;
	for(int j=0;j<index;j++)
		returnWartosc = returnWartosc + mac->wartosci[j][index]*wyjsciaNeuronow[j];
	returnWartosc = returnWartosc + mac->wartosci[index][myNetwork->liczbaWierszy];
	
	return (1/(1+exp(-parametr*returnWartosc)));
}

float NeuralNetwork::linear(int index, double parametr, Macierz *mac) 
{
	double returnWartosc = 0;
	for(int j=0;j<index;j++)
		returnWartosc = returnWartosc + mac->wartosci[j][index]*wyjsciaNeuronow[j];
	returnWartosc = parametr*returnWartosc + mac->wartosci[index][myNetwork->liczbaWierszy];
	
	return returnWartosc;
}

float NeuralNetwork::linear1(int index, double parametr, Macierz *mac) 
{
	double returnWartosc = 0;
	for(int j=0;j<index;j++)
		returnWartosc = returnWartosc + mac->wartosci[j][index]*wyjsciaNeuronow[j];
	returnWartosc = parametr*returnWartosc + mac->wartosci[index][myNetwork->liczbaWierszy];
	
	if(returnWartosc > 1)
		return 1;
	if(returnWartosc < -1)
		return -1;
	
	return returnWartosc;
}

float NeuralNetwork::linear2(int index, double parametr, Macierz *mac) 
{
	double returnWartosc = 0;
	for(int j=0;j<index;j++)
		returnWartosc = returnWartosc + mac->wartosci[j][index]*wyjsciaNeuronow[j];
	returnWartosc = parametr*returnWartosc + mac->wartosci[index][myNetwork->liczbaWierszy];
	
	if(returnWartosc > 1)
		return 1;
	if(returnWartosc < 0)
		return 0;
	
	return returnWartosc;
}

//---------------------------------------------------------
// Constructor

ControlVehicleScanning::ControlVehicleScanning()
{
	m_heading = 0;
	m_speed = 0;
	m_dist_to_bottom = 100;
	m_scanning = false;
	m_run_mission = false;
	m_go_to_point = false;
	m_return = false;
	m_remote = true;
	m_path_set = false;
	
	m_no_of_waypoint = 0;
	
	m_speed_turn = 0.5;
	m_speed_scanning = 1;
	
	maxBladSynchronizacjiError = 0;
	maxBladSynchronizacji = 0;
	maxBladTrackError = 0;
	maxBladTrack = 0;
	wayPoint = 1;
	dist_to_trackline = 0;
	diffInDisTime = 0;
	
	position[0] = 0;
	position[1] = 0;
	
	m_lead_damper = MAX_LEAD_DAMPER;
	m_turn_rate = TURN_RATE;
	m_wsp_skalowania = 1;
	m_lead_distance = MIN_LEAD_DISTANCE;
	m_wsp_thurn_loss = 1;
	m_thurn_loss = THURN_LOSS;
	m_wsp_acc = 1;
	m_wsp_dcc = 1;
	m_max_acceleration = MAX_ACCELERATION;
	m_max_decceleration = MAX_DECCELERATION;
	m_threshold_for_error_track = 1;
	m_threshold_for_error_time = 1;
	m_speed_factor = SPEED_FACTOR;
	m_rudder_factor = RUDDER_FACTOR;
	
	Macierz* mac = NeuralNetwork::getMatrix("macierz.txt");
	network.setParameters(mac,6,3);
	
	start = false;
}

//---------------------------------------------------------
// Destructor

ControlVehicleScanning::~ControlVehicleScanning()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool ControlVehicleScanning::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
	bool   mstr  = msg.IsString();
	bool   mdbl  = msg.IsDouble();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

    if(key == "SCANNING_UPDATE" && mstr) 
		getScanningParameters(msg.GetString());
	else if(key == "NAV_HEADING" && mdbl)
		m_heading = msg.GetDouble();
	else if(key == "NAV_SPEED" && mdbl)
		m_speed = msg.GetDouble();
	else if(key == "NAV_X" && mdbl)
		position[0] = msg.GetDouble();
	else if(key == "NAV_Y" && mdbl)
		position[1] = msg.GetDouble();
	else if(key == "NAV_DIST_TO_BOTTOM" && mdbl)
		m_dist_to_bottom = msg.GetDouble();
	else if(key == "SCANNING" && mstr)
		{
		string sval  = msg.GetString(); 
		if(sval == "true")
			m_scanning = true;
		else
			m_scanning = false;
		}
	else if(key == "RUN_MISSION" && mstr)
		{
		string sval  = msg.GetString(); 
		if(sval == "true")
			m_run_mission = true;
		else
			m_run_mission = false;
		}
	else if(key == "GO_TO_POINT" && mstr)
		{
		string sval  = msg.GetString(); 
		if(sval == "true")
			m_go_to_point = true;
		else
			m_go_to_point = false;
		}
	else if(key == "RETURN" && mstr)
		{
		string sval  = msg.GetString(); 
		if(sval == "true")
			m_return = true;
		else
			m_return = false;
		}
	else if(key == "REMOTE_CONTROL" && mstr)
		{
		string sval  = msg.GetString(); 
		if(sval == "true")
			m_remote = true;
		else
			m_remote = false;
		}
    else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool ControlVehicleScanning::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool ControlVehicleScanning::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  bool ok = true;
  if(m_scanning && m_run_mission && !m_go_to_point && !m_return && !m_remote && m_path_set)
	{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	if(!start)
		{
		start = true;
		time_start_seconds = tp.tv_sec + tp.tv_usec / 1000000.0;
		czas = 0;
		}
	else
		czas = tp.tv_sec + tp.tv_usec / 1000000.0 - time_start_seconds;
	ok = runControl();
	}
  
  if(!ok)
	  return(false);
  
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool ControlVehicleScanning::OnStartUp()
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
	if(param == "LEAD_DAMPER") {
		handled = true;
		if(isNumber(value,false))
			m_lead_damper = atof(value.c_str()); 
		else
			reportConfigWarning("LEAD_DAMPER is not a number");
	}
	else if(param == "TURN_RATE") {
		handled = true;
		if(isNumber(value,false))
			m_turn_rate = atof(value.c_str()); 
		else
			reportConfigWarning("TURN_RATE is not a number");
	}
	else if(param == "CALIBRATION_RATE") {
		handled = true;
		if(isNumber(value,false))
			m_wsp_skalowania = atof(value.c_str()); 
		else
			reportConfigWarning("CALIBRATION_RATE is not a number");
	}
	else if(param == "LEAD_DISTANCE") {
		handled = true;
		if(isNumber(value,false))
			m_lead_distance = atof(value.c_str()); 
		else
			reportConfigWarning("LEAD_DISTANCE is not a number");
	}
	else if(param == "WSP_THURN_LOSS") {
		handled = true;
		if(isNumber(value,false))
			m_wsp_thurn_loss = atof(value.c_str()); 
		else
			reportConfigWarning("WSP_THURN_LOSS is not a number");
	}
	else if(param == "THURN_LOSS") {
		handled = true;
		if(isNumber(value,false))
			m_thurn_loss = atof(value.c_str()); 
		else
			reportConfigWarning("THURN_LOSS is not a number");
	}
	else if(param == "WSP_ACC") {
		handled = true;
		if(isNumber(value,false))
			m_wsp_acc = atof(value.c_str()); 
		else
			reportConfigWarning("WSP_ACC is not a number");
	}
	else if(param == "WSP_DCC") {
		handled = true;
		if(isNumber(value,false))
			m_wsp_dcc = atof(value.c_str()); 
		else
			reportConfigWarning("WSP_DCC is not a number");
	}
	else if(param == "MAX_ACC") {
		handled = true;
		if(isNumber(value,false))
			m_max_acceleration = atof(value.c_str()); 
		else
			reportConfigWarning("MAX_ACC is not a number");
	}
	else if(param == "MAX_DCC") {
		handled = true;
		if(isNumber(value,false))
			m_max_decceleration = atof(value.c_str()); 
		else
			reportConfigWarning("MAX_DCC is not a number");
	}
	else if(param == "THRESHOLD_ERR_TRACK") {
		handled = true;
		if(isNumber(value,false))
			m_threshold_for_error_track = atof(value.c_str()); 
		else
			reportConfigWarning("THRESHOLD_ERR_TRACK is not a number");
	}
	else if(param == "THRESHOLD_ERR_TIME") {
		handled = true;
		if(isNumber(value,false))
			m_threshold_for_error_time = atof(value.c_str()); 
		else
			reportConfigWarning("THRESHOLD_ERR_TIME is not a number");
	}
	else if(param == "SPEED_FACTOR") {
		handled = true;
		if(isNumber(value,false))
			m_speed_factor = atof(value.c_str()); 
		else
			reportConfigWarning("SPEED_FACTOR is not a number");
	}
	
    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  
  m_vehicle_param = (m_wsp_thurn_loss*m_thurn_loss + m_wsp_acc*m_max_acceleration + m_wsp_dcc*m_max_decceleration);
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void ControlVehicleScanning::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("SCANNING_UPDATE", 0);
  Register("NAV_HEADING", 0);
  Register("NAV_SPEED", 0);
  Register("NAV_DIST_TO_BOTTOM", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  
  Register("SCANNING", 0);
  Register("RUN_MISSION", 0);
  Register("GO_TO_POINT", 0);
  Register("RETURN", 0);
  Register("REMOTE_CONTROL", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool ControlVehicleScanning::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}

void ControlVehicleScanning::getScanningParameters(string str)
{
	vector<string> keyValue = parseString(str, '=');
	
	if(keyValue[0] == SPEED)
		m_speed_scanning = atof(keyValue[1].c_str());
	else if(keyValue[0] == PATH)
		{
		string withoutBraces = stripBraces(keyValue[1]);
		vector<string> points = parseString(withoutBraces, ':');
		for(int j=0;j<points.size();j++)
			{
			vector<string> point = parseString(points[j], ',');
			if(isNumber(point[0],false) && isNumber(point[1],false))
				{
				m_list_of_waypoints.add_vertex(atof(point[0].c_str()),atof(point[1].c_str()));
				m_path_set = true;
				}
			}
		}
}

bool ControlVehicleScanning::runControl()
{
	if(wayPoint < m_list_of_waypoints.size() && maxBladSynchronizacjiError < PROG_TEST && maxBladSynchronizacji < PROG_SYNCH_UCZ && maxBladTrackError < PROG_TEST && maxBladTrack < PROG_TRACK_UCZ && m_dist_to_bottom > 0 )
		{
		double odlOdWayPointa = distPointToPoint(m_list_of_waypoints.get_vx(wayPoint),m_list_of_waypoints.get_vy(wayPoint),position[0],position[1]);
		
		double nx, ny,x,y;
		double katPomiedzyWayPointami = relAng(m_list_of_waypoints.get_vx(wayPoint-1),m_list_of_waypoints.get_vy(wayPoint-1),m_list_of_waypoints.get_vx(wayPoint),m_list_of_waypoints.get_vy(wayPoint));
		perpSegIntPt1(m_list_of_waypoints.get_vx(wayPoint-1),m_list_of_waypoints.get_vy(wayPoint-1),m_list_of_waypoints.get_vx(wayPoint),m_list_of_waypoints.get_vy(wayPoint),position[0], position[1], nx, ny);
		
		double previousErrorTrack = dist_to_trackline;
		dist_to_trackline = distPointToPoint(position[0], position[1], nx, ny);
		
		double damper_factor = 1.0;
		if(dist_to_trackline > m_lead_damper*m_turn_rate)
			damper_factor = 1/(1+10*m_wsp_skalowania*dist_to_trackline);
			
  
		double dist1 = distPointToPoint(nx, ny,m_list_of_waypoints.get_vx(wayPoint),m_list_of_waypoints.get_vy(wayPoint));
		if(dist1 > (m_lead_distance*damper_factor) || (wayPoint-1)%3 == 0) 
			dist1 = m_lead_distance*damper_factor;  
		
		projectPoint(katPomiedzyWayPointami,dist1,nx,ny,x,y);
		
		double headingSet = relAng(position[0],position[1],x,y);
		
		double time1 = 0;
		double time2 = 0;
		int z;
		double x1,y1;
		for(z=0;z<m_list_of_waypoints.size()-1;z++)
			{
			x = m_list_of_waypoints.get_vx(z);
			y = m_list_of_waypoints.get_vy(z);
			x1 = m_list_of_waypoints.get_vx(z+1);
			y1 = m_list_of_waypoints.get_vy(z+1);
			dist1 = distPointToPoint(x,y,x1,y1);
			time2 = time1;
			if(z%3==0 || (z+1)%3==0 || z == 0) //bez wayPoint==2,5,8 ktore sa skretami, za ktorymi predkosc powinna byc mniejsza czyli 0.5*m_speed_scanning
				time1 += 2*dist1/m_speed_scanning;
			else
				time1 += dist1/m_speed_scanning;
			
			if(time1 >= czas)
				break;
			}
			
		time1 = czas - time2;
		double correct_dist_to_cover = time1*m_speed_scanning;
		if(z%3==0 || (z+1)%3==0 || z == 0)
			correct_dist_to_cover =  0.5*correct_dist_to_cover;
		
		katPomiedzyWayPointami = relAng(x, y, x1, y1); 
		
		double m_right_point_x,m_right_point_y;
		projectPoint(katPomiedzyWayPointami,correct_dist_to_cover,x,y,m_right_point_x,m_right_point_y);
		
		previousErrorSynch = diffInDisTime;
		
		if(wayPoint == z+1)
			{
			double distToNextWayPoin = distPointToPoint(nx,ny,m_list_of_waypoints.get_vx(wayPoint),m_list_of_waypoints.get_vy(wayPoint)); 
			double rightDistToWayPoint = distPointToPoint(m_right_point_x,m_right_point_y,m_list_of_waypoints.get_vx(wayPoint),m_list_of_waypoints.get_vy(wayPoint));
			diffInDisTime = distToNextWayPoin - rightDistToWayPoint;
			}
		else if(wayPoint < z+1)
			diffInDisTime = distPointToPoint(m_list_of_waypoints.get_vx(wayPoint),m_list_of_waypoints.get_vy(wayPoint),nx,ny) + distPointToPoint(m_list_of_waypoints.get_vx(wayPoint),m_list_of_waypoints.get_vy(wayPoint),m_right_point_x,m_right_point_y);				
		else
			{
			double distToNextWayPoin = distPointToPoint(nx,ny,m_list_of_waypoints.get_vx(wayPoint),m_list_of_waypoints.get_vy(wayPoint));
			if(distToNextWayPoin > distPointToPoint(m_list_of_waypoints.get_vx(wayPoint),m_list_of_waypoints.get_vy(wayPoint),m_list_of_waypoints.get_vx(wayPoint-1),m_list_of_waypoints.get_vy(wayPoint-1)))
				diffInDisTime = distPointToPoint(nx,ny,m_list_of_waypoints.get_vx(wayPoint-1),m_list_of_waypoints.get_vy(wayPoint-1)) - distPointToPoint(m_list_of_waypoints.get_vx(wayPoint-1),m_list_of_waypoints.get_vy(wayPoint-1),m_right_point_x,m_right_point_y);
			else
				diffInDisTime = -(distPointToPoint(m_list_of_waypoints.get_vx(wayPoint-1),m_list_of_waypoints.get_vy(wayPoint-1),nx,ny) + distPointToPoint(m_list_of_waypoints.get_vx(wayPoint-1),m_list_of_waypoints.get_vy(wayPoint-1),m_right_point_x,m_right_point_y));
			}
		
		double diffInDisTimePositive = fabs(diffInDisTime);
		
		if((wayPoint+1)%3 == 0 && diffInDisTimePositive > maxBladSynchronizacji) //czyli waypointy na prostej, na kawałku trasy podlegajacemu skanowaniu
			maxBladSynchronizacji = diffInDisTimePositive;
			
		if(diffInDisTimePositive > maxBladSynchronizacjiError)
			maxBladSynchronizacjiError = diffInDisTimePositive;
	
		double newSpeedTime = 0;
		
		if(dist_to_trackline > m_threshold_for_error_track*m_vehicle_param || diffInDisTimePositive > m_threshold_for_error_time*m_vehicle_param)
			{
			network.setWejscie(0,0.5*dist_to_trackline);
			network.setWejscie(1,0.5*diffInDisTime);
			network.setWejscie(2,0.5*previousErrorTrack);
			network.setWejscie(3,0.5*previousErrorSynch);
			network.run();
			newSpeedTime = 2*network.getWyjscie(0);
			double dist1 = 10*fabs(network.getWyjscie(1));
			
			projectPoint(katPomiedzyWayPointami,dist1,nx,ny,x,y);
			headingSet = relAng(position[0],position[1],x,y);
			}
				
				
		if((wayPoint+1)%3 == 0 && dist_to_trackline > maxBladTrack) //czyli waypointy na prostej, na kawałku trasy podlegajacemu skanowaniu
				maxBladTrack = dist_to_trackline;
				
		if(dist_to_trackline > maxBladTrackError)
			maxBladTrackError = dist_to_trackline;
				
		double speedSet = m_speed_scanning + newSpeedTime;
		
		if(speedSet > MAX_VEHICLE_SPEED)
			speedSet = MAX_VEHICLE_SPEED;
		if(speedSet < 0)
			speedSet = 0;
		
		if(speedSet <= 0.001)
			Notify("DESIRED_THRUST", 0.0);
		else
			{
			double desired_thrust = speedSet*m_speed_factor;
			if(desired_thrust <= MAX_THRUST)
				Notify("DESIRED_THRUST", desired_thrust);
			else
				Notify("DESIRED_THRUST", MAX_THRUST);
			}
		
		double desiredRudder = m_rudder_factor*angle180(headingSet-m_heading);
		if(fabs(desiredRudder) <= MAX_RUDDER)
			Notify("DESIRED_RUDDER", desiredRudder);
		else
			if(desiredRudder < 0)
				Notify("DESIRED_RUDDER", -MAX_RUDDER);
			else
				Notify("DESIRED_RUDDER", MAX_RUDDER);
		}
	else
		return false;
	
	return true;
}




