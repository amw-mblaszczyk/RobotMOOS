/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: BHV_NewWaypoint.h                                       */
/*    DATE: Nov 2004 (original version - many changes since)     */
/*                                                               */
/* This file is part of MOOS-IvP                                 */
/*                                                               */
/* MOOS-IvP is free software: you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation, either version  */
/* 3 of the License, or (at your option) any later version.      */
/*                                                               */
/* MOOS-IvP is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty   */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See  */
/* the GNU General Public License for more details.              */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with MOOS-IvP.  If not, see                     */
/* <http://www.gnu.org/licenses/>.                               */
/*****************************************************************/
 
#ifndef BHV_NewWaypoint_HEADER
#define BHV_NewWaypoint_HEADER

#include <string>
#include "IvPBehavior.h"
#include "WaypointEngine.h"
#include "XYPoint.h"

class BHV_NewWaypoint : public IvPBehavior {
public:
  BHV_NewWaypoint(IvPDomain);
  ~BHV_NewWaypoint() {}
  
  bool           setParam(std::string, std::string);
  IvPFunction*   onRunState();
  void           onIdleState();
  void           onIdleToRunState();
  BehaviorReport onRunState(std::string);
  void           onRunToIdleState();
  void           onSetParamComplete();
  void           onCompleteState() {postErasables();}

protected:
  bool         updateInfoIn();
  bool         setNextWaypoint();
  IvPFunction* buildOF(std::string);

  void         postStatusReport();
  void         postViewableSegList();
  void         postErasableSegList();
  void         postErasables();
  void         postCycleFlags();
  void         postWptFlags(double x, double y);
  void         handleVisualHint(std::string);
  void         updateOdoDistance();
  void         markOdoLeg();
  
  void 		 setNewSpeed();

protected: 
  WaypointEngine m_waypoint_engine;

protected: // configuration parameters
  double 	  m_cruise_speed_constant;
  double      m_cruise_speed;
  double      m_cruise_speed_alt;
  bool        m_lead_to_start;
  bool        m_use_alt_speed;
  double      m_lead_distance;
  double      m_lead_damper;
  float 		m_max_vehicle_speed;
  float 		m_currentP;//PID z pradem
  float 		m_threshold_for_error_track;
  double    m_os_sog;
  std::string m_efficiency_measure;
  std::string m_ipf_type;

  // Configurable names of MOOS variables for reports
  std::string m_var_report;
  std::string m_var_index;
  std::string m_var_cyindex;
  std::string m_var_suffix;
  std::string m_var_dist_to_prev;
  std::string m_var_dist_to_next;

  // Var-Data flags for posting when behavior finishes cycle
  std::vector<VarDataPair> m_cycle_flags;
  std::vector<VarDataPair> m_wpt_flags;

  // Visual hints affecting properties of polygons/points
  std::string m_hint_vertex_color;
  std::string m_hint_edge_color;
  std::string m_hint_label_color;
  std::string m_hint_nextpt_color;
  std::string m_hint_nextpt_lcolor;
  double      m_hint_vertex_size;
  double      m_hint_edge_size;
  double      m_hint_nextpt_vertex_size;

protected: // intermediate or object global variables.
  double    m_osh;  // Ownship heading
  double    m_osv;  // Ownship velocity
  double    m_osx;  // Ownship x position
  double    m_osy;  // Ownship y position
  
  // Time/Distance efficiency state information
  bool      m_odo_set_flag;
  bool      m_odo_leg_disq;
  double    m_odo_settime;
  double    m_odo_setx;
  double    m_odo_sety;
  bool      m_odo_virgin;

  double    m_dist_leg_odo;

  double    m_dist_total_odo;
  double    m_dist_total_linear;
  double    m_time_total_odo;
  double    m_time_total_linear;

  double    m_course_pct;
  double    m_speed_pct;

  double    m_osx_prev;
  double    m_osy_prev;

  XYPoint   m_nextpt;
  XYPoint   m_trackpt;
  XYPoint   m_markpt;

  XYPoint   m_prevpt;
  XYPoint   m_perp_point;
  XYPoint   m_right_point;
  //XYPoint   m_starting_point;
  
  //bool m_start_mission;
  float m_dist_to_trackline;

  bool      m_greedy_tour_pending;
  
  //std::ofstream ofs; ////////!!!!!!!!!!!!!!!1
  
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_NewWaypoint(domain);}
}

#endif




