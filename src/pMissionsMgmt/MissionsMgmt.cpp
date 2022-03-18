/************************************************************/
/*    NAME: MarekB                                              */
/*    ORGN: MIT                                             */
/*    FILE: MissionsMgmt.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>

#include "MissionsMgmt.h"

using namespace std;

//---------------------------------------------------------
// Constructor

MissionsMgmt::MissionsMgmt()
{
}

//---------------------------------------------------------
// Destructor

MissionsMgmt::~MissionsMgmt()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool MissionsMgmt::OnNewMail(MOOSMSG_LIST &NewMail)
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


     if(key == "FOO") 
       cout << "great!";
     else if (key == "MISSION_CONF"){
    	 SearchAreaTask searchAreaConf = parseSearchArea(msg.GetString());
    	 reportEvent("MissionsMgmt: Who set to:["+intToString(searchAreaConf.getWho())+"]");
    	 if (searchAreaConf.isActionSet())
    		 reportEvent("MissionsMgmt: action set to:["+intToString(searchAreaConf.getAction())+"]");
    	 else
    		 reportEvent("MissionsMgmt: action not set");
    	 if (!searchAreaConf.isParsingError())
    		 processSearchAreaTask(searchAreaConf);

    	 //cout<<"MMMM:"<<searchAreaConf.get_mission_spec()<<endl;
     }

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool MissionsMgmt::OnConnectToServer()
{
   registerVariables();

   if ((getDirname() == NULL) || (getDirname().length() == 0))
	   reportConfigWarning("Dirname for storing missions not defined. Define 'dir=dirname' in config file.");
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool MissionsMgmt::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool MissionsMgmt::OnStartUp()
{
	cout<<endl<<"======================= START"<<endl;
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }
    else if(param == "dir") {
    	if (value[value.length()-1] != '/')
    		value += '/';

    	setDirname(value);
    	reportEvent("MissionMgmt default directory for missions files set to: "+getDirname());
        handled = true;
    }


    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	

  //test();
 //testMgmnt();

  return(true);
}

string MissionsMgmt::readTextFile(string filename) {
	string line;
	string s;
	ifstream file;

	file.open(filename, ios::in);
	if (file.is_open()) {
		while (getline(file, line)) {
			s += line;
			if (!file.eof())
				s += '\n';
		}
		file.close();
	} else {
		err_code = errno;
		err_string = strerror(errno);

		reportEvent("Reading mission file "+filename+" error:"+err_string);
	}
	return s;
}

int MissionsMgmt::writeTextFile(string s, string filename) {

		ofstream file;

	file.open(filename, ios::out);
		if (file.is_open()) {
			file<<s;
			file.close();
			return s.length();
		} else {
			err_code = errno;
			err_string = strerror(errno);

			reportEvent("Writing mission file "+filename+" error:"+err_string);
			return -1;
		}
}

//---------------------------------------------------------
// Procedure: registerVariables

void MissionsMgmt::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("MISSION_CONF", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool MissionsMgmt::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File:                                       " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}

void MissionsMgmt::processSearchAreaTask(SearchAreaTask searchAreaConf) {
	reportEvent("MissionsMgmt: Who recived:"+intToString(searchAreaConf.getWho())+" local:"+intToString(searchAreaTask.getWho()));
	if (searchAreaConf.isActionSet()){
		//Process Action
		if (searchAreaConf.getAction() == ACTION_CREATE){
			//creates new mission and writes it to file
			string filename=getDirname()+searchAreaConf.getName()+MISSION_EXT;

			reportEvent("MissionsMgmt: create mission for file:"+filename);
			if (is_searchAreaTask_set)
				searchAreaTask.clear();
			searchAreaTask = SearchAreaTask(searchAreaConf.get_mission_spec());
			is_searchAreaTask_set = true;
			//reportEvent("MissionsMgmt: write mission:"+searchAreaConf.get_mission_spec()+"\n"+"\t to file:"+filename);
			writeTextFile(searchAreaConf.get_mission_spec(), filename);
		}else if (searchAreaConf.getAction() == ACTION_UPDATE){
			//Read, update and write back the mission
			string filename=getDirname()+searchAreaConf.getName()+".mission";
			string str = readTextFile(filename);
			SearchAreaTask searchAreaTaskRead(str);
			searchAreaTask = searchAreaTaskRead;
			if (searchAreaTask.isParsingError())
				reportEvent("MissionMgmt: mission reading error:"+searchAreaTask.getParsingError(true));
			searchAreaTask.update(searchAreaConf);
			writeTextFile(searchAreaTask.get_mission_spec(), filename);
			reportEvent("MissionsMgmt: Who updated:"+intToString(searchAreaTask.getWho()));
			reportEvent("MissionMgmt: mission:"+searchAreaConf.getName()+" successfully updated and stored in "+filename);
		}else if (searchAreaConf.getAction() == ACTION_READ){
			//Read mision from file and send do MOOSDB
			string filename=getDirname()+searchAreaConf.getName()+".mission";
			string str = readTextFile(filename);
			Notify("MISSION", str);
			reportEvent("MissionMgmt: mission:"+searchAreaConf.getName()+" published.");
		}else if (searchAreaConf.getAction() == ACTION_DELETE){
			//Clear local mission  and remove corresponding file
			if (searchAreaConf.getName().compare(searchAreaTask.getName()) == 0){
				searchAreaTask.clear();
				is_searchAreaTask_set = true;
			}
			string filename=getDirname()+searchAreaConf.getName()+".mission";
			if ( remove( filename.c_str()) != 0)
				reportEvent("MissionMgmt: can\'t delete mission file:"+filename);
			else
				reportEvent("MissionMgmt: delete mission file:"+filename + " success.");
		} else if (searchAreaConf.getAction() == ACTION_SAVE) {
			//Save local mission to filesystem.
			if (searchAreaConf.getName().compare(searchAreaTask.getName()) == 0) {
				string filename = getDirname() + searchAreaConf.getName()	+ ".mission";
				reportEvent("MissionsMgmt: Who saved:"+intToString(searchAreaTask.getWho()));
				writeTextFile(searchAreaTask.get_mission_spec(), filename);
				reportEvent("MissionMgmt: mission:" + searchAreaTask.getName()	+ " saved.");
			}
		}else if (searchAreaConf.getAction() == ACTION_CALCULATE) {
			//Calculates local mission.
			if (searchAreaConf.getName().compare(searchAreaTask.getName()) == 0) {
				searchAreaTask.validate();
				if (searchAreaTask.isValid()){
					searchAreaTask.lawnMove();
					reportEvent("MissionMgmt: mission:" + searchAreaTask.getName()	+ " calculated.");
				}else{
					reportEvent("MissionMgmt: mission:" + searchAreaTask.getName()	+ " invalid.");
				}
			}
		}else if (searchAreaConf.getAction() == ACTION_LIST) {
			//read mission files and set list of missions.
			string missions_list = M_ACTION+"="+M_ACTION_LIST+","+getListMissions();
			Notify("MISSIONS", missions_list);
		}else if (searchAreaConf.getAction() == ACTION_PUBLISH) {
			//read mission files and publish mission to vehicles.
			string filename=getDirname()+searchAreaConf.getName()+".mission";
			string str = readTextFile(filename);
			SearchAreaTask searchAreaTask(str);
			if (searchAreaTask.isParsingError())
				reportRunWarning("MissionMgmt: mission reading error:"+searchAreaTask.getParsingError(true));
			else {
				searchAreaTask.setAction(ACTION_SAVE);
				for (int var = 0; var < 3; ++var) {
					searchAreaTask.setWho(var+1);
					Notify("MISSION", searchAreaTask.get_mission_spec());
				}
			reportEvent("MissionMgmt: mission:"+searchAreaConf.getName()+" published.");
			}
		}
	}

	reportEvent("MissionsMgmt: Who2 recived:"+intToString(searchAreaConf.getWho())+" local:"+intToString(searchAreaTask.getWho()));
}

void MissionsMgmt::testMgmnt() {
	cout<<"Start test mgmnt:"<<endl;
	//SearchArea test
	SearchAreaTask* searchTask = new SearchAreaTask();

	  searchTask->setName("test1");
	  searchTask->setAction(ACTION_CREATE);
	  searchTask->setWho(0);
	  searchTask->setSpeed(1.5);
	  searchTask->setDepth(5.5);
	  searchTask->setFormation(0);
	  searchTask->setStart_point(-50, -50);
	  searchTask->setEnd_point(-51.5, -55.3);
	  searchTask->genTestTask(-10, -30, 35, 50, 30);
	  searchTask->setMaxDepth(15);
	  searchTask->setMaxTime(3600);
	  searchTask->setLaneWidth(4);
	  searchTask->lawnMove();

	  cout<<"TEST CREATE MISSION:"<<searchTask->get_mission_spec()<<endl;
	  processSearchAreaTask(*searchTask);

	  cout<<"Testing update."<<endl;
	  searchTask->clear();
	  searchTask->setName("test1");
	  searchTask->setAction(ACTION_UPDATE);
	  searchTask->setSpeed(2);
	  searchTask->setDepth(5.2);
	  searchTask->setStart_point(-51, -51);
	  searchTask->setEnd_point(-52.5, -56.3);
	  searchTask->setMaxDepth(17);
	  searchTask->setMaxTime(3622);
	  searchTask->setLaneWidth(4.1);
	  searchTask->setLaneOffset(5.2);
	  searchTask->genTestTask(-12, -32, 37, 52, 37);

	  cout<<"Testing mission update: processing ..."<<endl;
	  processSearchAreaTask(*searchTask);
}

SearchAreaTask MissionsMgmt::parseSearchArea(string value) {
	SearchAreaTask searchAreaConf = SearchAreaTask(value);
	if (searchAreaConf.isParsingError()){
		reportEvent("MissionMgmnt mission message parsing error:"+searchAreaConf.getParsingError(true));
	}
	return searchAreaConf;
}

string MissionsMgmt::getListMissions() {
	DIR *dir;
	struct dirent *diread;
	vector<char*> files;
	string flist;

	if ((dir = opendir(getDirname().c_str())) != nullptr) {
		while ((diread = readdir(dir)) != nullptr) {
			if (diread->d_type == DT_REG) {
				string fname = diread->d_name;
cout<<"FILE:"<<fname<<endl;
				if (fname.length() > MISSION_EXT.length()) {
					string ext = fname.substr(
							fname.length() - MISSION_EXT.length());
cout<<"FILEEXT:"<<ext<<endl;
					if (ext == MISSION_EXT) {
						if (flist.length() > 1)
							flist += ',';
						flist += '\"';
						flist += fname.substr(0,
								fname.length() - MISSION_EXT.length());
						flist += '\"';
cout<<"FLIST:"<<flist<<endl;
					}
				}
			}
		}
		closedir(dir);
	}
	return flist;
}
