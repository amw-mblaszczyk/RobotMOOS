/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: Sonar.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Sonar_HEADER
#define Sonar_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

class Sonar : public CMOOSApp
{
 public:
   Sonar();
   ~Sonar();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
