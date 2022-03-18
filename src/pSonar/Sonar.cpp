/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: Sonar.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Sonar.h"

using namespace std;

//---------------------------------------------------------
// Constructor

Sonar::Sonar()
{
}

//---------------------------------------------------------
// Destructor

Sonar::~Sonar()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Sonar::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
	
	if(key == "NEW_OBSTACLE" && mstr)
		{
		vector<string> pointCoordinates = parseString(sval, ",");
		
		Notify("TRACKED_FEATURE", "x=" + pointCoordinates[0] + ", y=" + pointCoordinates[1] + ", z=" + pointCoordinates[2]);
		cout<<"TRACKED_FEATURE x=" + pointCoordinates[0] + ", y=" + pointCoordinates[1] + ", z=" + pointCoordinates[2]<<endl;
		}
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Sonar::OnConnectToServer()
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

bool Sonar::Iterate()
{
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Sonar::OnStartUp()
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

void Sonar::RegisterVariables()
{
  Register("NEW_OBSTACLE", 0);
}

