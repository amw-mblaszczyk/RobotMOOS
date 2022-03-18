/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: ObstacleManager.h                                    */
/*    DATE: Aug 27th 2014                                        */
/*****************************************************************/

#ifndef P_OBSTACLE_MANAGER_HEADER
#define P_OBSTACLE_MANAGER_HEADER

#define DEPTH_THRESHOLD_AVOID 1
#define DIST_THRESHOLD_ADD_POINT 2
#define DEPTH_THRESHOLD_ADD_POINT 1

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYPolygon.h"

class ObstacleManager : public AppCastingMOOSApp
{
public:
  ObstacleManager();
  ~ObstacleManager() {};
  
  ofstream myfile;
  
protected: // Standard MOOSApp functions to overload  
  bool OnNewMail(MOOSMSG_LIST &NewMail);
  bool Iterate();
  bool OnConnectToServer();
  bool OnStartUp();
  
protected: // Standard AppCastingMOOSApp function to overload 
  bool buildReport();
  
protected:
  void registerVariables();

  bool handleMailNewPoint(std::string);
  bool handleMailAlertRequest(std::string);

  bool handleGivenObstacle(std::string);
  
  void postConvexHullUpdates();
  void postConvexHullUpdate(std::string obstacle_key);
  
  XYPolygon genPseudoHull(const std::vector<XYPoint>& pts, double radius);
  
  XYPolygon placeholderConvexHull(std::string obstacle_key);
  
  XYPoint customStringToPoint(std::string point_str);

  void updatePolyRanges();
  void manageMemory();
  
  
  
  
private: // Configuration variables
  std::string  m_point_var;            // incoming points

  std::string  m_alert_var;
  double       m_alert_range;
  double 		m_depth_threshold_avoid;
  double m_dist_threshold_add_point;
  double m_depth_threshold_add_point;
  
  // Managing incoming points
  double       m_ignore_range;
  unsigned int m_max_pts_per_cluster;
  double       m_max_age_per_point;

  // Configuring Lasso option
  bool         m_lasso;
  unsigned int m_lasso_points;
  double       m_lasso_radius;

  bool         m_post_dist_to_polys;
  bool         m_post_view_polys;
  
private: // State variables
  double m_nav_x;
  double m_nav_y;
  double m_nav_z;
  
  unsigned int  m_points_total;
  unsigned int  m_points_invalid;
  unsigned int  m_points_ignored;

  unsigned int  m_clusters_released;
  
  // Each map is keyed on the obstacle_key
  std::map<std::string, std::list<XYPoint> > m_map_points;
  std::map<std::string, unsigned int>        m_map_points_total;
  std::map<std::string, XYPolygon>           m_map_poly_convex;
  std::map<std::string, double>              m_map_poly_range;
  std::map<std::string, bool>                m_map_poly_given;
  std::map<std::string, bool>                m_map_poly_changed;
  std::map<std::string, unsigned int>        m_map_updates_total;
  std::map<std::string, float>        m_map_max_depth;
  std::map<std::string, float>        m_map_min_depth;
};

#endif 
