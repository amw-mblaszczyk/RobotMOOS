/*
 * SearchAreaTask.h
 *
 *  Created on: 25 mar 2021
 *      Author: marek
 */

#ifndef SRC_PSEARCHAREA_SEARCHAREATASK_H_
#define SRC_PSEARCHAREA_SEARCHAREATASK_H_

#include <vector>
#include <iostream>
#include <math.h>
#include "XYPoint.h"
#include "XYSegList.h"
#include "XYPolygon.h"

using namespace std;

static const double DEG2RAD = M_PI/180.0;
static const double RAD2DEG = 180.0/M_PI;

static const string MISSION_MSG = "MISION";
static const string MISSION_CONF_MSG = "MISION_CONF";

static const short ACTION_SAVE = 0;
static const short ACTION_START = 1;
static const short ACTION_STOP = 2;
static const short ACTION_CREATE = 3;
static const short ACTION_READ = 4;
static const short ACTION_UPDATE = 5;
static const short ACTION_DELETE = 6;
static const short ACTION_CALCULATE = 7;
static const short ACTION_ADDPOINT = 8;
static const short ACTION_ADDSAFETYPOINT = 9;
static const short ACTION_PUBLISH = 10;
static const short ACTION_LIST = 11;

//MISSION
static const string M_ACTION = "Action";
static const string M_ACTION_SAVE = "save";
static const string M_ACTION_START = "start";
static const string M_ACTION_STOP = "stop";

//MISSION_CONF
static const string M_ACTION_CREATE = "create";
static const string M_ACTION_READ = "read";
static const string M_ACTION_UPDATE = "update";
static const string M_ACTION_DELETE = "delete";
static const string M_ACTION_CALCULATE = "calculate";
static const string M_ACTION_ADDPOINT = "addPoint";
static const string M_ACTION_ADDSAFETYPOINT = "addSafetyPoint";
static const string M_ACTION_PUBLISH = "publish";
static const string M_ACTION_LIST = "list";

//MISSION
static const string M_WHO = "Who";
static const string M_NAME = "Name";
static const string M_SPEED = "Speed";
static const string M_DEPTH = "Depth";
static const string M_DEPTH_DIST = "DepthDistance";
static const string M_DISTANCE = "Distance";
static const string M_FORMATION = "Formation";
static const string M_START_POINT = "StartPoint";
static const string M_END_POINT = "EndPoint";
static const string M_RECTANGLE = "Rectangle";
static const string M_POINTS = "Points";
static const string M_TIME_START = "TimeStart";
static const string M_SAFETY_REGION = "SafetyReg";
static const string M_MAX_TIME = "MaxTime";
static const string M_MAX_DEPTH = "MaxDepth";

//MISSION_CONF
static const string M_POINT = "point";
static const string M_POINTS1 = "points1";
static const string M_POINTS2 = "points2";
static const string M_POINTS3 = "points3";
static const string M_LANE_OFFSET = "lane_offset";
static const string M_LANE_WIDTH = "lane_width";
static const string M_TURN_WAY = "turn_way";
static const string M_VALID = "valid";
static const string M_ERROR = "error";
static const string M_FORMATION_TRIANGLE = "triangle";

static const string M_SWARM = "swarm";
static const string M_VEHICLE = "vehicle";
static const int NUMBER_OF_VEHICLES = 3; 

static const int NUMBER_OF_ERRORS = 7; 
static const int ERROR_NO_ERROR = 0; 
static const int ERROR_WRONG_VALUE = 1; 
static const int ERROR_IS_NOT_A_NUMBER = 2; 
static const int ERROR_WRONG_VEHICLE_NUMBER = 3; 
static const int ERROR_WRONG_RECTANGLE = 4;
static const int ERROR_WRONG_POINT = 5;
static const int ERROR_TOO_FEW_POINTS = 6;
static const string parsingErrors[NUMBER_OF_ERRORS] = {"No error","Error: wrong value","Error: value is not a number","Error: wrong vehicle number","Error: wrong rectangle","Error: wrong point","Too few points in safety region"};


class SearchAreaTask {
public:

	SearchAreaTask();
	SearchAreaTask(string msg, bool conf=true);
	virtual ~SearchAreaTask();

	void clear();
	void printRegion();
	void printRegion(XYPoint region[4]);
	int getFirstPointIdx();
	void shiftRegion(int s);
	void reverseRegion();
	void lawnMove();
	void validate();
	XYSegList buildSegList(int idx, std::string name);
	XYPolygon builArea(std::string name);
	void genTestTask(double x_center, double y_center, double width, double height, double angle);

	double getLaneWidth() const {
		return lane_width;
	}

	void setLaneWidth(double laneWidth = 5) {
		lane_width = laneWidth;
		is_lane_width_set = true;
	}

	bool isLane_width_set(){
		return is_lane_width_set;
	}

	int getSpeed() const {
		return speed;
	}

	void setSpeed(int speed = 1) {
		this->speed = speed;
		is_speed_set = true;
	}
	
	bool isSpeedSet() const {
		return is_speed_set;
	}

	const XYPoint& getStart_point() const {
		return start_point;
	}

	void setStart_point(double x, double y) {
		start_point.set_vx(x);
		start_point.set_vy(y);
		is_start_point_set = true;
	}
	
	bool isStart_pointSet() const {
		return is_start_point_set;
	}
	
	const XYPoint& getEnd_point() const {
		return end_point;
	}

	void setEnd_point(double x, double y) {
		end_point.set_vx(x);
		end_point.set_vy(y);
		is_end_point_set = true;
	}
	
	bool isEnd_pointSet() const {
		return is_end_point_set;
	}
	
	const vector<XYPoint>& getWaypoints() const {
		return waypoints[0];
	}
	
	const vector<XYPoint>& getWaypoints(int vehicle) const {
		return waypoints[vehicle];
	}
	
	XYSegList getPath();

	void addWaypoint(XYPoint point) {
		waypoints[0].push_back(point);
	}
	
	void addWaypoint(int idx, float x, float y) {
		XYPoint point(x,y);
		waypoints[idx].push_back(point);
	}
	
	void addWaypoint(XYPoint point, int vehicle) {
		waypoints[vehicle].push_back(point);
	}
	
	void setWaypointsSet(bool is_set) {
		is_waypoints_set = is_set;
	}
	
	bool isWaypointsSet() const {
		return is_waypoints_set;
	}
	
	const XYPoint* getRectangle() const {
		return region;
	}
	
	void addRectanglePoint(float x, float y, int point_number) {
		region[point_number].set_vx(x);
		region[point_number].set_vy(y);
	}
	
	string getRectangle_spec();

	void setRectangleSet(bool is_set) {
		is_rectangle_set = is_set;
	}
	
	bool isRectangleSet() const {
		return is_rectangle_set;
	}
	
	XYSegList getSafetyRegion() const {
		return safety_region;
	}
	
	bool isSafetyRegionSet() const {
		return is_safety_region_set;
	}
	
	void setSafetyRegionSet(bool is_set) {
		is_safety_region_set = is_set;
	}
	
	float getDepth() const {
		return depth;
	}

	void setDepth(float depth) {
		this->depth = depth;
		is_depth_set = true;
	}
	
	bool isDepthSet() const {
		return is_depth_set;
	}
	
	float getDepthDistance() const {
			return depth_distance;
		}

		void setDepthDistance(float depth) {
			this->depth_distance = depth;
			is_depth_distance_set = true;
		}

		bool isDepthDistanceSet() const {
			return is_depth_distance_set;
		}

	int getMaxTime() const {
		return max_time;
	}

	void setMaxTime(int max_time) {
		this->max_time = max_time;
		is_max_time_set = true;
	}
	
	bool isMaxTimeSet() const {
		return is_max_time_set;
	}
	
	float getMaxDepth() const {
		return max_depth;
	}

	void setMaxDepth(float max_depth) {
		this->max_depth = max_depth;
		is_max_depth_set = true;
	}
	
	bool isMaxDepthSet() const {
		return is_max_depth_set;
	}
	
	short getWho() const {
		return who;
	}

	void setWho(short who) {
		this->who = who;
		is_who_set = true;
	}
	
	bool isWhoSet() const {
		return is_who_set;
	}
	
	bool isParsingError() const {
		if(parsing_error > 0)
			return true;
		return false;
	}
	
	string getParsingError(bool withExt = false){
		if ((withExt) && (parsing_error>0))
			return parsingErrors[parsing_error]+ " in section:"+err_ext;
		else
			return parsingErrors[parsing_error];
	}
	
	short getAction() const {
		return action;
	}

	void setAction(short action) {
		this->action = action;
		is_action_set = true;
	}
	
	bool isActionSet() const {
		return is_action_set;
	}
	
	float getDistance() const {
		return distance;
	}

	void setDistance(float distance) {
		this->distance = distance;
		is_distance_set = true;
	}
	
	bool isDistanceSet() const {
		return is_distance_set;
	}
	
	short getFormation() const {
		return formation;
	}

	void setFormation(short formation) {
		this->formation = formation;
		is_formation_set = true;
	}
	
	bool isFormationSet() const {
		return is_formation_set;
	}
	
	string getName() const {
		return name;
	}

	void setName(string name) {
		this->name = name;
		is_name_set = true;
	}
	
	bool isNameSet() const {
		return is_name_set;
	}
	
	struct timeval getTimeStart() const {
		return time_start;
	}

	void setTimeStart(struct timeval start) {
		time_start.tv_sec = start.tv_sec;
		time_start.tv_usec = start.tv_usec;
		is_time_start_set = true;
	}
	
	bool isTimeStartSet() const {
		return is_time_start_set;
	}
	
	bool isMissionValidForVehicle();
	static bool isMissionStartOrStop(string msg, short &what_action);

	string get_mission_spec();
	static string getPointSpec(XYPoint p);
	string getWayPoints_spec(int idx);
	string get_action_spec(int action);
	int update(SearchAreaTask updater);

	double getLaneOffset() const {
		return lane_offset;
	}

	void setLaneOffset(double laneOffset = 0.25) {
		lane_offset = laneOffset;
		is_lane_offset_set = true;
	}

	bool isLaneOffsetSet() const {
		return is_lane_offset_set;
	}

	bool isValid() const {
		return valid;
	}

	double getTurnWay() const {
		return turn_way;
	}

	void setTurnWay(double turnWay = 20) {
		turn_way = turnWay;
	}

protected: //protected methods


private : //private methods
	void printWaypoint();
	double getAlfa(XYPoint &p1, XYPoint &p2);
	XYPoint rotatePoint(XYPoint &p, double xs, double ys, double alfa);
	void rotateRegion(XYPoint reg[4], double alfa);
	void rotateWaypoints(double alfa);
	void calculateWaypoints(XYPoint rreg[4]);

	// methods for 'parrarel left' return points
	//   |\   |\   |\
	//   | \  | \  |  \
	//   |  \ |  \ |    \
	//
	void calculateReturnDy();
	void addReturnPoint(XYPoint &p1, XYPoint &p2, int idx);

	// methods for 'corona' return points
	//   |\   /\    /|
	//   | \ /  \  / |
	//   |  X   X  |
	void calculateReturnDy1();
	void addReturnPoint1(XYPoint &p1, XYPoint &p2, int idx);

	
	int parseMission(string msg, bool conf=false);

	void setError(int err_no=0, string str=""){
		err = err_no;
		err_ext = str;
	}



protected: //protected varibles

private: // Configuration variables
	string name;
	//XYPoint region[4] = {XYPoint(50,50), XYPoint(250,50), XYPoint(250,350), XYPoint(50,350)};
	XYPoint region[4];
	XYSegList safety_region;
	XYPoint start_point;
	XYPoint end_point;
	double lane_width = -100;
	short action;
	short who=-1;
	float speed;
	float depth;
	float depth_distance;
	float distance;
	short formation;
	struct timeval time_start;
	int max_time;
	float max_depth;
	double lane_offset = 0.25; //search lanes offset lane_width*lane_offset
	double turn_way = 20;
	std::vector<XYPoint> waypoints[3];
	bool valid = false;
	int err = 0;
	string err_ext;
	
	bool is_action_set;
	bool is_who_set;
	bool is_speed_set;
	bool is_depth_set;
	bool is_depth_distance_set;
	bool is_distance_set;
	bool is_start_point_set;
	bool is_end_point_set;
	bool is_waypoints_set;
	bool is_name_set;
	bool is_formation_set;
	bool is_rectangle_set;
	bool is_time_start_set;
	bool is_safety_region_set;
	bool is_max_time_set;
	bool is_lane_width_set;
	bool is_lane_offset_set;
	bool is_max_depth_set;
	
	int parsing_error;
	string err_str;

 private: // State variables
	double return_dy[3];
};

#endif /* SRC_PSEARCHAREA_SEARCHAREATASK_H_ */
