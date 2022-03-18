/************************************************************/
/*    NAME: m.blaszczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: SearchArea.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef SearchArea_HEADER
#define SearchArea_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

class SearchAreaTask;

class SearchArea : public CMOOSApp
{
 public:
   SearchArea();
   ~SearchArea();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();
   void viewSearchTask();

 private: // Configuration variables
   SearchAreaTask *searchTask;

 private: // State variables
};

#endif 
