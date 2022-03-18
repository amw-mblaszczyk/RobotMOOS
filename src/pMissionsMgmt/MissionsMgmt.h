/************************************************************/
/*    NAME: MarekB                                              */
/*    ORGN: MIT                                             */
/*    FILE: MissionsMgmt.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef MissionsMgmt_HEADER
#define MissionsMgmt_HEADER

#include <string>

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "SearchAreaTask.h"

using std::string;

// ERROR Codes
static const int ERR_SUCCESS = 0;

//ERROR Messages
static const string ERR_MSG_OK = "Success";

//mission files extension
static const string MISSION_EXT = ".mission";

class MissionsMgmt : public AppCastingMOOSApp
{
 public:
   MissionsMgmt();
   ~MissionsMgmt();

   /**
    * Reads string from file. If error occurs, error code is stored in error_code member and error message
    * is stored in err_string member.
    * @param filename full name of the file to read.
    * @return string content of the file or NULL
    */
   string readTextFile(string filename);

   /**
    * Writes string s into the specified text file. The file content is overwritten. If any error occurs,
    * error code is stored in error_code member and error message is stored in err_string member.
    * @param s string to be written
    * @param filename full file name to write.
    * @return number of chars written or -1
    */
   int writeTextFile(string s, string filename);

	const string& getDirname() const {
		return dirname;
	}

	void setDirname(const string &dirname) {
		this->dirname = dirname;
	}

	int getErrCode() const {
		return err_code;
	}

	const string& getErrString() const {
		return err_string;
	}

	const string& getFilename() const {
		return filename;
	}

	void setFilename(const string &filename) {
		this->filename = filename;
	}

	const string& getMissionName() const {
		return mission_name;
	}

	void setMissionName(const string &missionName) {
		mission_name = missionName;
	}

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

   void processSearchAreaTask(SearchAreaTask searchAreaConf);


 private: //private functions
   void testMgmnt();
   SearchAreaTask parseSearchArea(string value);
   string getListMissions();

 private: // Configuration variables
   string dirname="./";
   string filename;
   string mission_name;

 private: // State variables
   string err_string = "Success";
   int err_code = 0;
   bool is_searchAreaTask_set = false;
   SearchAreaTask searchAreaTask;

};

#endif 
