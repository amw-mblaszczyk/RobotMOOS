/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: TworzenieMisji.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef TworzenieMisji_HEADER
#define TworzenieMisji_HEADER

#define PROG_BLISKOSCI_PUNKTU 10
#define PROG_BLISKOSCI_PRZESZKODY 10

#include "MOOS/libMOOS/MOOSLib.h"

using namespace std;

class TworzenieMisji : public CMOOSApp
{
 public:
   TworzenieMisji();
   ~TworzenieMisji();

 static void parseXYPoint(string pointString, XYPoint& point);
 string generatePoints();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables

 vector<string> punktyMisjiString;
 vector<XYPoint> punktyMisji;
 vector<int> punktyMisjiOperacje;

 bool changeOfPoint;
 XYPoint pointMemorized;
 string pointMemorizedString;
 int where;
 bool m_missionGenerated;
 int m_labelOfPoint;
 bool m_theFirstPoint;
 XYPoint m_theLastObstaclePoint;

 bool isPointInside(XYPoint point);
 int whereIsPointInside(XYPoint point);
 void drawMarker(XYPoint point, string type, string msg = "");
 void drawXYSegList();
};

#endif 
