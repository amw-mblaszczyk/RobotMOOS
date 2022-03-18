/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_AvoidCollisionUpDown.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef AvoidCollisionUpDown_HEADER
#define AvoidCollisionUpDown_HEADER

#define COMPLETED_DIST 50
#define ALERT_RANGE 20
#define MAX_DEPTH 50

#include <string>
#include "IvPBehavior.h"

struct XYPolyPlusDepth
{
	XYPolygon poly;
	double m_min_depth;
	double m_max_depth;
};

class BHV_AvoidCollisionUpDown : public IvPBehavior {
public:
  BHV_AvoidCollisionUpDown(IvPDomain);
  ~BHV_AvoidCollisionUpDown() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();
  
  ofstream myfile;

protected: // Local Utility functions

	 bool   updatePlatformInfo();

	double  m_completed_dist;
	double m_alert_range;
	
	vector<XYPolyPlusDepth>  m_obstacle_orig;
	
	double  m_osx;
	double  m_osy;
	double  m_osh;

protected: // Configuration parameters

protected: // State variables
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_AvoidCollisionUpDown(domain);}
}
#endif
