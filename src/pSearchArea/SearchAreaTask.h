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
static const string MISSION_MSG = "MISION";
static const string MISSION_CONF_MSG = "MISION_CONF";

//MISSION
static const string M_ACTION = "action";
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

//MISSION
static const string M_WHO = "who";
static const string M_NAME = "name";
static const string M_SPEED = "speed";
static const string M_DEPTH = "depth_distance";
static const string M_FORMATION = "formation";
static const string M_START_POINT = "start_point";
static const string M_END_POINT = "end_point";
static const string M_RECTANGLE = "rectangle";
static const string M_POINTS = "points";

//MISSION_CONF
static const string M_POINT = "point";
static const string M_POINTS1 = "points1";
static const string M_POINTS2 = "points2";
static const string M_POINTS3 = "points3";
static const string M_LANE_OFFSET = "lane_offset";
static const string M_TURN_WAY = "turn_way";
static const string M_VALID = "valid";
static const string M_ERROR = "error";


class SearchAreaTask {
public:


	SearchAreaTask();
	virtual ~SearchAreaTask();

	void printRegion();
	void printRegion(XYPoint region[4]);
	double distance(XYPoint p1, XYPoint p2);
	int getFirstPointIdx();
	void shiftRegion(int s);
	void reverseRegion();
	void lawnMove();
	XYSegList buildSegList(int idx, std::string name);
	XYPolygon builArea(std::string name);
	void genTestTask(double x_center, double y_center, double width, double height, double angle);

	int getLaneWidth() const {
		return lane_width;
	}

	void setLaneWidth(int laneWidth = 5) {
		lane_width = laneWidth;
	}

	int getSpeed() const {
		return speed;
	}

	void setSpeed(int speed = 1) {
		this->speed = speed;
	}

	const XYPoint& getStart_point() const {
		return start_point;
	}

	void setStart_point(double x, double y) {
		this->start_point.set_vx(x);
		this->start_point.set_vy(y);
	}

protected: //protected methods
	void addReturnPoint(XYPoint &p1, XYPoint &p2, int idx);



private : //private methods
	void printWaypoint();
	double getAlfa(XYPoint &p1, XYPoint &p2);
	XYPoint rotatePoint(XYPoint &p, double xs, double ys, double alfa);
	void rotateRegion(XYPoint reg[4], double alfa);
	void rotateWaypoints(double alfa);
	void calculateWaypoints(XYPoint rreg[4]);
	void calculateReturnDy();



protected: //protected varibles

private: // Configuration variables
	std::string name;
	//XYPoint region[4] = {XYPoint(50,50), XYPoint(250,50), XYPoint(250,350), XYPoint(50,350)};
	XYPoint region[4];
	XYPoint start_point;
	XYPoint end_point;
	double lane_width = -100;
	double speed = -100;
	double depth_distance = -100;
	double lane_offset = 0.25; //search lanes offset lane_width*lane_offset
	double turn_way = 20; //distance of turn = turn_way*lane_width
	std::vector<XYPoint> waypoints[3];
	bool valid = false;
	int err = 0;

 private: // State variables
	const double DEG2RAD = M_PI/180.0;
	const double RAD2DEG = 180.0/M_PI;
	double return_dy[3];
};

#endif /* SRC_PSEARCHAREA_SEARCHAREATASK_H_ */
