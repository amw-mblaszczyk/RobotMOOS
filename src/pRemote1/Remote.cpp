/************************************************************/
/*    NAME: m.blaszczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: Remote.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Remote.h"
#include "Joystick.h"

using namespace std;

//---------------------------------------------------------
// Constructor

Remote::Remote()
{
}

//---------------------------------------------------------
// Destructor

Remote::~Remote()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Remote::OnNewMail(MOOSMSG_LIST &NewMail)
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
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Remote::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Remote::Iterate()
{
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Remote::OnStartUp()
{

	  cout<<"AAAAAAAAAAAAAAA"<<endl;
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
/*
  cout<<"AAAAAAAAAAAAAAA"<<endl;
  Joystick joy;
  cout<<"AAAAAAAAAAAAAAA"<<endl;
  joy.setRemoteApp(this);
  cout<<"AAAAAAAAAAAAAAA"<<endl;
  joy.findJoystick(60);
  cout<<"Joy:"<<joy.getJoystickDev()<<endl;
  string dev = Joystick::listDevices(true);
  cout<<"Dev:"<<dev<<endl;

  joy.joystickThread();
*/
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Remote::RegisterVariables()
{
  Register("FOOBAR", 0);
}

bool Remote::sendJoystick(int x, int y){
	string msg="x="+to_string(x)+",y="+to_string(y);
	return Notify("Joystick", msg);
}

bool Remote::sendRemoteMsg(string msg) {
	cout<<"SENDING:"<<msg<<endl;
	return Notify("REMOTE", "");
}
