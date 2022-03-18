/************************************************************/
/*    NAME: Tomasz Praczyk                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_AvoidCollisionUpDown.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <fstream>
#include <cstdlib>
#include "MBUtils.h"
#include "AngleUtils.h"
#include "GeomUtils.h"
#include "BuildUtils.h"
#include "ZAIC_PEAK.h"
#include "XYFormatUtilsPoly.h"
//#include "MOOSThread.h"
#include "BHV_AvoidCollisionUpDown.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_AvoidCollisionUpDown::BHV_AvoidCollisionUpDown(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  //IvPBehavior::setParam("name", "avoid_obstacles");
  this->setParam("descriptor", "avoid_obstacles");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "depth");
  
  m_completed_dist    = COMPLETED_DIST;
  m_alert_range = ALERT_RANGE;
  
  myfile.open ("example3.txt");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y, NAV_HEADING");
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_AvoidCollisionUpDown::setParam(string param, string val)
{
  myfile<<"!!!!setParam="<<param<<" "<<val<<endl;
  if(IvPBehavior::setParam(param, val))
    return(true);

  double dval = atof(val.c_str());
  bool   non_neg_number = (isNumber(val) && (dval >= 0));

  // Get the numerical value of the param argument for convenience once
  if((param=="polygon") || (param=="points") || (param=="poly")) {
	  myfile<<"poly="<<val<<endl;
    vector<string> poly_strings = parseString(val,'&');
	if(poly_strings.size() == 2)
		{
		XYPolyPlusDepth new_polygon_depth;
		new_polygon_depth.poly = string2Poly(poly_strings[0]);
		if(!new_polygon_depth.poly.is_convex())
		  return(false);
		vector<string> min_max = parseString(poly_strings[1],',');
		vector<string> min_string = parseString(min_max[0],'=');
		vector<string> max_string = parseString(min_max[1],'=');
		new_polygon_depth.m_min_depth = atof(min_string[1].c_str());
		new_polygon_depth.m_max_depth = atof(max_string[1].c_str());
		myfile<<"setParam:"<<new_polygon_depth.poly.get_spec()<<" "<<new_polygon_depth.m_min_depth<<" "<<new_polygon_depth.m_max_depth<<endl;
		
		vector<XYPolyPlusDepth>::iterator p;
		for(p=m_obstacle_orig.begin(); p!=m_obstacle_orig.end();) 
			{
			XYPolyPlusDepth poly_depth = *p;
			if(poly_depth.poly.get_label() == new_polygon_depth.poly.get_label())
				p = m_obstacle_orig.erase(p);
			else
				++p;
			}
		m_obstacle_orig.push_back(new_polygon_depth);
		}
	else
		return(false);
  }
  else if((param == "completed_dist") && non_neg_number) 
	{
    m_completed_dist = dval;
	myfile<<"m_completed_dist="<<m_completed_dist<<endl;
	}
  else if((param == "alert_range") && non_neg_number) 
    {
    m_alert_range = dval;
	myfile<<"m_alert_range="<<m_alert_range<<endl;
	}
  else
    return(false);
	  

  // If not handled above, then just return false;
  return(true);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_AvoidCollisionUpDown::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_AvoidCollisionUpDown::onHelmStart()
{
	 if(m_update_var == "")
		return;

    myfile<<"onHelmStart "<<m_update_var<<endl;
	 string alert_request = "var=" + m_update_var;
	 alert_request += ",alert_range=" + doubleToStringX(m_alert_range,1);

	 postMessage("OBJ_ALERT_REQUEST", alert_request);
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_AvoidCollisionUpDown::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_AvoidCollisionUpDown::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_AvoidCollisionUpDown::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_AvoidCollisionUpDown::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_AvoidCollisionUpDown::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_AvoidCollisionUpDown::onRunState()
{
	  if(m_obstacle_orig.size() == 0)
		  return 0;
	  
	  updatePlatformInfo();
	  myfile<<"OnRunState start "<<m_obstacle_orig.size()<<endl;
	  vector<XYPolyPlusDepth>::iterator p;
	  for(p=m_obstacle_orig.begin(); p!=m_obstacle_orig.end();) 
		{
		  XYPolyPlusDepth poly_depth = *p;
		  double os_dist_to_poly = (poly_depth.poly).dist_to_poly(m_osx, m_osy);
		  myfile<<"OnRunState os_dist_to_poly="<<os_dist_to_poly<<" "<<m_osx<<" "<<m_osy<<endl;
		  if(os_dist_to_poly > m_completed_dist)
				p = m_obstacle_orig.erase(p);
		  else
				++p;
		}
		
	if(m_obstacle_orig.size() == 0)
		  return 0;
		
	myfile<<"OnRunState 1 "<<m_obstacle_orig.size()<<endl;
  
	int total_relevance = 0;
	double min = MAX_DEPTH;
	bool vector_of_relevance[m_obstacle_orig.size()];
	for(int i=0;i<m_obstacle_orig.size();i++)
		if(!polyAft(m_osx, m_osy, m_osh, m_obstacle_orig[i].poly))
			{
			total_relevance++;
			vector_of_relevance[i] = true;
			if(min > m_obstacle_orig[i].m_min_depth)
				min = m_obstacle_orig[i].m_min_depth;
			}
		else
			vector_of_relevance[i] = false;
		
	myfile<<"OnRunState 2 "<<total_relevance<<endl;
		
	if(total_relevance == 0)
		return 0;
	
	
  
	double newDepth = min-1;
	
	myfile<<"OnRunState newDepth="<<newDepth<<endl;
	
	ZAIC_PEAK crs_zaic(m_domain, "depth");
	double peak_width = newDepth / 2.0;
    crs_zaic.setParams(newDepth, peak_width, 1.6, 20, 0, 100);
	if(crs_zaic.stateOK() == false) {
		string warnings = "Depth ZAIC problems " + crs_zaic.getWarnings();
		postWMessage(warnings);
		return(0);
		}
	
    IvPFunction *crs_ipf = crs_zaic.extractIvPFunction();
	if(!crs_ipf)
		postWMessage("Failure on the SPD ZAIC via ZAIC_PEAK utility");
	
	if(crs_ipf)
		crs_ipf->setPWT(m_priority_wt);
	
	for(int i=0;i<m_obstacle_orig.size();i++)
		if(vector_of_relevance[i] == true)
			{
			string spec = m_obstacle_orig[i].poly.get_spec();
			postMessage("VIEW_POLYGON", spec, "orig"+intToString(i));
			}

	return(crs_ipf);
}

bool BHV_AvoidCollisionUpDown::updatePlatformInfo()
{
  bool ok1, ok2, ok3;
  m_osx = getBufferDoubleVal("NAV_X", ok1);
  m_osy = getBufferDoubleVal("NAV_Y", ok2);
  m_osh = getBufferDoubleVal("NAV_HEADING", ok3);

  if(!ok1 || !ok2) {
    postWMessage("No Ownship NAV_X and/or NAV_Y in info_buffer");
    return(false);
  }
  if(!ok3) {
    postWMessage("No Ownship NAV_HEADING in info_buffer");
    return(false);
  }
  return(true);
}

