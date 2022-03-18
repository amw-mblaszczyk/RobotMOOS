/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: TworzenieMisji.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include <vector>
#include <string>
#include "XYPoint.h"
#include "XYMarker.h"
#include "XYSegList.h"
#include "GeomUtils.h"
#include "MBUtils.h"
#include "TworzenieMisji.h"

using namespace std;

//---------------------------------------------------------
// Constructor

TworzenieMisji::TworzenieMisji()
{
 changeOfPoint = false;
 m_missionGenerated = false;
 m_labelOfPoint = 1;
 m_theFirstPoint = true;
}

//---------------------------------------------------------
// Destructor

TworzenieMisji::~TworzenieMisji()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool TworzenieMisji::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
  
  string key   = msg.GetKey();
cout<<key<<endl;
  if(key == "PUNKT_MISJI" && msg.IsString())
	{
	XYPoint point;
	parseXYPoint(msg.GetString(),point);
	
	if(!m_missionGenerated)
			{
			where = -1;
			if(punktyMisji.size() > 0)
				where = whereIsPointInside(point);
			if(where >= 0)
				{
				Notify("TRAIL_RESET","kom1");
				for(int i=0;i<punktyMisji.size();i++)
					if(i != where)
						drawMarker(punktyMisji.at(i),"square");
					else
						drawMarker(punktyMisji.at(i),"triangle",",msg=add/change/remove/add operation?");
				pointMemorized = point;
				pointMemorizedString = msg.GetString();
				if(punktyMisji.size() > 1)
					drawXYSegList();
				changeOfPoint = true;
				}
			else
				{
				punktyMisjiString.push_back(msg.GetString());
				punktyMisji.push_back(point);
				punktyMisjiOperacje.push_back(-1);
			
				drawMarker(point,"square");
				if(punktyMisji.size() > 1)
					drawXYSegList();
				}
		}
	/*else
			{
			if(!m_theFirstPoint)
					{
					double distance = distPointToPoint(m_theLastObstaclePoint,point); 
					if(distance > PROG_BLISKOSCI_PRZESZKODY)
							m_labelOfPoint++;
					}
			else
					m_theFirstPoint = false;
				
			m_theLastObstaclePoint.set_vx(point.x());
			m_theLastObstaclePoint.set_vy(point.y());
			m_theLastObstaclePoint.set_vz(0);
			
			Notify("TRACKED_FEATURE", "x=" + doubleToString(point.x()) + ", y=" + doubleToString(point.y()) + ", z=5, label=ala" + intToString(m_labelOfPoint));
			drawMarker(m_theLastObstaclePoint,"circle");
			}*/
	}
  else if(msg.IsString() && (key == "PRZESZKODA1" || key == "PRZESZKODA2" || key == "PRZESZKODA3"))
	{
	XYPoint point;
	parseXYPoint(msg.GetString(),point);
	cout<<key<<endl;
	cout<<"wsp:"<<point.get_spec()<<endl;
	
	if(key == "PRZESZKODA1")
		point.set_vz(10);
	else if(key == "PRZESZKODA2")
		point.set_vz(20);
	else
		point.set_vz(30);
	
	cout<<"wsp1:"<<point.get_spec()<<endl;
	
	Notify("NEW_OBSTACLE", doubleToString(point.x()) + "," + doubleToString(point.y()) + "," + doubleToString(point.z()));
	if(key == "PRZESZKODA1")
		drawMarker(point,"circle",",msg=10");
	else if(key == "PRZESZKODA2")
		drawMarker(point,"triangle",",msg=20");
	else
		drawMarker(point,"square",",msg=50");
		
	}
  else if(key == "GENERATE_MISSION" && msg.IsString() && !msg.GetString().compare("true"))
	{
	string myPoints = generatePoints();
	cout<<myPoints<<endl;
	Notify("WPT_UPDATE",myPoints);
	Notify("MISSION_DEPTH","10,5,10,5,10,5,10"); //zmienic tutaj depth jest na stale!!!!!!!!!!!!!!!!!!!!!!!
	
	string oper;
	for(int i=0;i<punktyMisjiOperacje.size();i++)
		{
		oper = oper + intToString(punktyMisjiOperacje.at(i));
		if(i != punktyMisjiOperacje.size() -1)
			oper = oper + ",";
		}
	Notify("MISSION_OPER",oper);
	m_missionGenerated = true;
	}
   else if(key == "ADD_POINT" && changeOfPoint)
	{
	cout<<"ADD_POINT"<<endl;
	
	Notify("TRAIL_RESET","kom1");

	punktyMisjiString.push_back(pointMemorizedString);
	punktyMisji.push_back(pointMemorized);
	punktyMisjiOperacje.push_back(-1);
		
	for(int i=0;i<punktyMisji.size();i++)
		drawMarker(punktyMisji.at(i),"square");
	if(punktyMisji.size() > 1)
		drawXYSegList();
	changeOfPoint = false;
	}
   else if(key == "CHANGE_POINT" && changeOfPoint)
	{
	cout<<"CHANGE_POINT"<<endl;

	Notify("TRAIL_RESET","kom1");

	punktyMisjiString[where] = pointMemorizedString;
	punktyMisji[where] = pointMemorized;

	for(int i=0;i<punktyMisji.size();i++)
		drawMarker(punktyMisji.at(i),"square");
	if(punktyMisji.size() > 1)
		drawXYSegList();
	changeOfPoint = false;
	}
   else if(key == "REMOVE_POINT" && changeOfPoint)
	{
	cout<<"REMOVE_POINT"<<endl;

	Notify("TRAIL_RESET","kom1");

	punktyMisjiString.erase(punktyMisjiString.begin()+where);
	punktyMisji.erase(punktyMisji.begin()+where);
	punktyMisjiOperacje.erase(punktyMisjiOperacje.begin()+where);

	for(int i=0;i<punktyMisji.size();i++)
		drawMarker(punktyMisji.at(i),"square");
	if(punktyMisji.size() > 1)
		drawXYSegList();
	changeOfPoint = false;
	}
   else if((key == "ADD_OPERATION1" || key == "ADD_OPERATION2") && changeOfPoint)
	{
	cout<<"ADD_OPERATION1-2"<<endl;

	if(key == "ADD_OPERATION1")
		punktyMisjiOperacje[where] = 1;
	else
		punktyMisjiOperacje[where] = 2;

	Notify("TRAIL_RESET","kom1");

	for(int i=0;i<punktyMisji.size();i++)
		drawMarker(punktyMisji.at(i),"square");
	if(punktyMisji.size() > 1)
		drawXYSegList();
	changeOfPoint = false;
	}

 //cout<<"changeOfPoint="<<changeOfPoint<<endl;

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
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool TworzenieMisji::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   Notify("GENERATE_MISSION", "false");
   Notify("KAMERA_OPERATION", "false");
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool TworzenieMisji::Iterate()
{
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool TworzenieMisji::OnStartUp()
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

void TworzenieMisji::RegisterVariables()
{
  // Register("FOOBAR", 0);
 Register("PUNKT_MISJI", 0);
 Register("GENERATE_MISSION", 0);
 Register("ADD_POINT", 0);
 Register("CHANGE_POINT", 0);
 Register("REMOVE_POINT", 0);
 Register("ADD_OPERATION1", 0);
 Register("ADD_OPERATION2", 0);
 Register("PRZESZKODA1", 0);
 Register("PRZESZKODA2", 0);
 Register("PRZESZKODA3", 0);
}

//---------------------------------------------------------
// Procedure: parseXYPoint: zamiana stringa np. "21.5, -92.5" na XYPoint

void TworzenieMisji::parseXYPoint(string pointString, XYPoint& point)
{
 int pozycja = pointString.find_first_of(",");

 string v = pointString.substr(0,pozycja);

 //cout<<"x="<<v<<endl;
 point.set_vx(atof(v.c_str()));	

 v = pointString.substr(pozycja+1);
 //cout<<"y="<<v<<endl;
 point.set_vy(atof(v.c_str()));
}

//---------------------------------------------------------
// Procedure: generatePoints: funkcja składa wiele stringów np. "21.5, -92.5"
// w jeden string gdzie poszczególne współrzędne XY są oddzielone ":"

string TworzenieMisji::generatePoints()
{
 string points("polygon=");
 for(int i=0;i<punktyMisjiString.size();i++)
	{
	points = points + punktyMisjiString.at(i);
	if(i != punktyMisjiString.size() - 1)
		points = points + ":";
	//cout<<points<<endl;
	}
 return points;
}


//---------------------------------------------------------
// Procedure: isPointInside: sprawdza czy dany punkt jest w liście punktów
bool TworzenieMisji::isPointInside(XYPoint point)
{
 for(int i=0;i<punktyMisji.size();i++)
	if(fabs(point.get_vx() - punktyMisji.at(i).get_vx()) < PROG_BLISKOSCI_PUNKTU)
		return true;

 return false;
}

//---------------------------------------------------------
// Procedure: whereIsPointInside: sprawdza czy dany punkt jest w liście punktów i podaje jego index
int TworzenieMisji::whereIsPointInside(XYPoint point)
{
 for(int i=0;i<punktyMisji.size();i++)
	if(fabs(point.get_vx() - punktyMisji.at(i).get_vx()) + fabs(point.get_vy() - punktyMisji.at(i).get_vy()) < PROG_BLISKOSCI_PUNKTU)
		return i;

 return -1;
}

//---------------------------------------------------------
// Procedure: drawMarker: rysuje marker w punkcie point
void TworzenieMisji::drawMarker(XYPoint point, string type, string msg)
{
 XYMarker marker;
 marker.set_type(type);
 marker.set_vx(point.x());
 marker.set_vy(point.y());
 marker.set_width(5.0);
 marker.set_range(1.0);

 string str = marker.get_spec();
 str = str + msg;
 //cout<<str<<endl;
 Notify("VIEW_MARKER", str);
}

//---------------------------------------------------------
// Procedure: drawXYSegList: 
void TworzenieMisji::drawXYSegList()
{
 XYSegList list;
 
 for(int i=0;i<punktyMisji.size();i++)
	list.add_vertex(punktyMisji.at(i));

 string str = list.get_spec();
 str = str + ", label=misja";
 Notify("VIEW_SEGLIST",str);
}



