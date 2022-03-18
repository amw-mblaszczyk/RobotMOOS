/************************************************************/
/*    NAME: m.blaszczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: Remote.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Remote_HEADER
#define Remote_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MsgRemote.h"

#include <string>

class Remote : public CMOOSApp
{
 public:
   Remote();
   ~Remote();

   bool sendJoystick(int x, int y);
   bool sendRemoteMsg(string msg);

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
