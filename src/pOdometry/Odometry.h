/************************************************************/
/*    NAME: Jane Doe                                              */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef Odometry_HEADER
#define Odometry_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class Odometry : public AppCastingMOOSApp
{
 public:
   Odometry();
   ~Odometry();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
   bool   m_first_reading;
    double m_current_x;
    double m_current_y;
    double m_previous_x;
    double m_previous_y;
    double m_total_distance;
};

#endif 
