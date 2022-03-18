/************************************************************/
/*    NAME: m.blaszczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: Remote.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "Remote.h"
#include "Joystick.h"

using namespace std;

//---------------------------------------------------------
// Constructor

Remote::Remote()
{
	 joy = new Joystick();
}

//---------------------------------------------------------
// Destructor

Remote::~Remote()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail
bool Remote::OnNewMail(MOOSMSG_LIST &NewMail) {
	MOOSMSG_LIST::iterator p;

	for (p = NewMail.begin(); p != NewMail.end(); p++) {
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

		string key = msg.GetKey();
		string sval = msg.GetString();
		bool handled = false;
		if (key == "REMOTE_CONF")
			handled = handleRemoteConf(sval);
		if (!handled)
			reportRunWarning("Remote:Unhandled Mail: " + key);
	}

	return (true);
}


//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Remote::OnConnectToServer()
{
	cout<<"pRemote Connected to server"<<endl;
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

  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      bool correct = false;
	  if(param == "joy_rudder_code") {
		  if (isNumber(value)){
			  joy_rudder_code = atoi(value.c_str());
			  correct = true;
		  }
      }else if(param == "joy_thrust_code") {
		  if (isNumber(value)){
			  joy_thrust_code = atoi(value.c_str());
			  correct = true;
		  }
      }else if(param == "joy_elevator_code") {
		  if (isNumber(value)){
			  joy_elevator_code = atoi(value.c_str());
			  correct = true;
		  }
      }else if(param == "joy_btn_rudder_0") {
		  if (isNumber(value)){
			  joy_btn_rudder_0 = atoi(value.c_str());
			  correct = true;
		  }
      }else if(param == "joy_btn_rudder_0_alt") {
		  if (isNumber(value)){
			  joy_btn_rudder_0_alt = atoi(value.c_str());
			  correct = true;
		  }
      }else if(param == "joy_btn_elevator_0") {
		  if (isNumber(value)){
			  joy_btn_elevator_0 = atoi(value.c_str());
			  correct = true;
		  }
      }else if(param == "joy_btn_elevator_0_alt") {
		  if (isNumber(value)){
			  joy_btn_elevator_0_alt = atoi(value.c_str());
			  correct = true;
		  }
      }else{
    	  reportConfigWarning("Unhandled param:"+param+" value:"+value);
    	  correct = true;
      }

	  if (!correct)
		  reportConfigWarning("Incorrect param "+param+":"+value);
    }
  }
  
  RegisterVariables();	

  joy->setRemoteApp(this);
  joy->joystickThread();

  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Remote::RegisterVariables()
{
  Register("REMOTE_CONF", 0);
}

//------------------------------------------------------------
// Procedure: buildReport()

bool Remote::buildReport()
{

  return(true);
}

bool Remote::sendRemoteMsg(const string msg) {
	string s(msg);
	//cout<<"SENDING:"<<msg<<endl;
	bool r = Notify("REMOTE", s);
	cout<<"SENDING REMOTE message:["<<s<<"] RESULT:"<<r<<endl;
	return r;
}

bool Remote::handleRemoteConf(string str) {
	bool sendRemote = false;
	MsgRemote msg;
	string rmsg = "";
	vector<string> svector = parseStringQ(str, ',');
	for (unsigned int i = 0; i < svector.size(); i++) {
		string param = biteStringX(svector[i], '=');
		string value = svector[i];

		if (param == R_JOY_WHO) //&& isNumber(value))
			joy->setWho(value);
		else if (param == R_JOY_ACTIVE){
			if (value == "true")
				joy->setActive(true);
			else
				joy->setActive(false);
		}
		else{
			if (rmsg.size()>0) rmsg += ',';
			rmsg += param+'='+value;
			sendRemote = true;
		}
	}
	if (sendRemote)
		sendRemoteMsg(rmsg);

	return true;
}

