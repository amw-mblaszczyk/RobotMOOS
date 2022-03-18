/*
 * SearchAreaTask.cpp
 *
 *  Created on: 25 mar 2021
 *      Author: marek
 */
#include <math.h>
#include <iostream>
#include <limits>
#include <time.h>
#include "SearchAreaTask.h"
#include "GeomUtils.h"

using namespace std;

SearchAreaTask::SearchAreaTask() {
	// TODO Auto-generated constructor stub
	clear();
}

SearchAreaTask::SearchAreaTask(string msg) {
	clear();
	parsing_error = parseMission(msg);
}

SearchAreaTask::~SearchAreaTask() {
	// TODO Auto-generated destructor stub
}

void SearchAreaTask::clear() {
	is_action_set = false;
	is_who_set = false;
	is_speed_set = false;
	is_depth_set = false;
	is_distance_set = false;
	is_start_point_set = false;
	is_end_point_set = false;
	is_waypoints_set = false;
	is_name_set = false;
	is_formation_set = false;
	is_rectangle_set = false;
	is_time_start_set = false;
	is_safety_region_set = false;
	is_max_time_set = false;
	is_max_depth_set = false;
	
	for (int j = 0; j < 3; j++)
		waypoints[j].clear();
	
	safety_region.clear();
}

int SearchAreaTask::getFirstPointIdx() {
	double dist[4];
	int idx = -1;
	double mindist = numeric_limits<double>::max();
	for (int var = 0; var < 4; ++var) {
		dist[var] = distPointToPoint(region[var], start_point);
		if (mindist>dist[var]){
			mindist = dist[var];
			idx = var;
		}
	}
	return idx;
}

void SearchAreaTask::printRegion() {
	for (int var = 0; var < 4; ++var) {
		XYPoint point = region[var];
		cout<<"point["<<var<<"]:"<<point.get_spec()<<endl;
	}
}

void SearchAreaTask::shiftRegion(int s) {
	if ((s==0) || (s>3)) return;
	XYPoint tmp = region[0];
	int idx = s+1;
	for (int var = 0; var < 3; ++var) {
		region[var] = region[var+1];
	}
	region[3] = tmp;
	shiftRegion(s-1);
}

void SearchAreaTask::reverseRegion() {
	XYPoint tmp = region[1];
	region[1] = region[3];
	region[3] = tmp;
}

void SearchAreaTask::lawnMove() {

	int idx = getFirstPointIdx();
	shiftRegion(idx);

	if (distPointToPoint(region[0], region[1]) > distPointToPoint(region[0], region[3]))
		reverseRegion();
	printRegion();

	double alfa = getAlfa(region[0], region[1]);
	XYPoint rregion[4];
	rotateRegion(rregion, -alfa);
	cout << "********************************" << endl;
	printRegion(rregion);
	cout << "********************************" << endl;

	calculateReturnDy();
	calculateWaypoints(rregion);
	cout << "-------------------------------------" << endl;

	printRegion();
	rotateWaypoints(alfa);

	//cout<<"Lanes:"<<l<<" dl:"<<dl<< " dx:"<<dx<<" dy:"<<dy<<" waypoints:"<<waypoints[0].size()<<endl;
	printWaypoint();
}

XYSegList SearchAreaTask::buildSegList(int idx, string name) {
	XYSegList segList;
	for (int i = 0; i < waypoints[idx].size(); i++) {
		segList.add_vertex(waypoints[idx][i]);
	}
	segList.set_label(name);
	return segList;
}

XYPolygon SearchAreaTask::builArea(std::string name) {
	XYPolygon polygon;
	for (int i = 0; i < 4; i++) {
		polygon.add_vertex(region[i].x(), region[i].y());
	}
	return polygon;
}

void SearchAreaTask::printWaypoint() {
	for (int j = 0; j < 3; j++) {
		cout << "points:" << waypoints[j].size() << ":";
		for (int i = 0; i < waypoints[j].size(); i++) {
			XYPoint p = waypoints[j][i];
			cout << "(" << p.x() << "," << p.y() << ")";
		}
		cout << endl;
		for (int i = 0; i < waypoints[j].size(); i++) {
			XYPoint p = waypoints[j][i];
			cout << "(" << p.get_spec() << ")";
		}
		cout << endl;
	}
}

void SearchAreaTask::addReturnPoint(XYPoint &p1, XYPoint &p2, int idx) {

	double dir_y = p1.y() - p2.y();
	double py;
	XYPoint p = XYPoint(waypoints[idx].back().x(), p1.y());
	if (idx == 0) {
		if (dir_y > 0)
			py = waypoints[idx].back().y() + return_dy[0];
		else
			py = waypoints[idx].back().y() - return_dy[0];
		p.set_vy(py);
	} else if (idx == 2) {
		if (dir_y > 0)
			py = waypoints[idx].back().y() + return_dy[0];
		else
			py = waypoints[idx].back().y() - return_dy[0];
		p.set_vy(py);
		p.set_vx(p1.x());
	} else if (idx == 1) {
		if (dir_y > 0)
			py = waypoints[idx].back().y() + return_dy[1];
		else
			py = waypoints[idx].back().y() - return_dy[1];
		p.set_vy(py);
		double x = p1.x() + (waypoints[idx].back().x()-p1.x())/2;
		p.set_vx(x);
	}
	cout << "CHECK TURN2:" << distPointToPoint(waypoints[idx].back(), p) << " "
			<< distPointToPoint(p, p1) << " "
			<< distPointToPoint(waypoints[idx].back(), p) + distPointToPoint(p, p1) << endl;

	waypoints[idx].push_back(p);
	waypoints[idx].push_back(p1);
	waypoints[idx].push_back(p2);

}


void SearchAreaTask::genTestTask(double x_center, double y_center, double width,
		double height, double angle) {
	double w = width/2;
	double h = height/2;

	double s = sin(angle*DEG2RAD);
	double c = cos(angle*DEG2RAD);

	double x0 = -w * c + h * s;
	double y0 = -w * s - h * c;
	double x1 = w * c + h * s;
	double y1 = w * s - h * c;
	double x2 = w * c - h * s;
	double y2 = w * s + h * c;
	double x3 = -w * c - h * s;
	double y3 = -w * s + h * c;


	region[0] = XYPoint(x0 + x_center, y0 + y_center);
	region[1] = XYPoint(x1 + x_center, y1 + y_center);
	region[2] = XYPoint(x2 + x_center, y2 + y_center);
	region[3] = XYPoint(x3 + x_center, y3 + y_center);

	//x1 = x * cos(alfa) - y * sin(alfa);
	//y1 = x * sin(alfa) + y * cos(alfa);
}

double SearchAreaTask::getAlfa(XYPoint &p1, XYPoint &p2) {
	double dx = p2.x() - p1.x();
	double dy = p2.y() - p1.y();
	double alfa = atan(dy/dx);
	cout<<"ALFA:"<<alfa*RAD2DEG<<endl;
	return alfa;
}


XYPoint SearchAreaTask::rotatePoint(XYPoint &p, double xs, double ys,
		double alfa) {

	double s = sin(alfa);
	double c = cos(alfa);

	double x = (p.x()-xs)*c - (p.y()-ys)*s;
	double y = (p.x()-xs)*s + (p.y()-ys)*c;

	XYPoint point(x,y);
	point.set_label(p.get_label());
	return point;
}

void SearchAreaTask::printRegion(XYPoint reg[4]) {
	for (int var = 0; var < 4; ++var) {
		XYPoint point = reg[var];
		cout << "point[" << var << "]:" << point.get_spec() << endl;
	}
}

void SearchAreaTask::rotateRegion(XYPoint reg[4], double alfa) {
	for (int j = 0; j < 4; j++) {
		reg[j] = rotatePoint(region[j], region[0].x(), region[0].y(), alfa);
	}
}

void SearchAreaTask::calculateWaypoints(XYPoint rreg[4]) {

	double dx = -lane_width;
	if (rreg[0].x() < rreg[1].x())
		dx = lane_width;
	double dist = distPointToPoint(rreg[0], rreg[1]);
	int l = dist / ((3-lane_offset)*lane_width) + 1;

	double x0 = rreg[0].x();
	double y0 = rreg[0].y();
	double y1 = rreg[3].y();
	double xa = x0;
	int i = 0;

	XYPoint p1;
	XYPoint p2;
	while (i < l) {
		for (int j = 0; j < 3; j++) {
			if ((i == 0) && (j==0)) {
				xa = x0 + dx / 2;
			} else if ( j==0 ){
				xa = x0 + dx*(1-lane_offset);
			}else{
				xa = x0 + dx;
			}
			if (i%2 == 0){
				p1 = XYPoint(xa, y0);
				p2 = XYPoint(xa, y1);
			}else{
				p1 = XYPoint(xa, y1);
				p2 = XYPoint(xa, y0);
			}
			if ( i == 0){
				waypoints[j].push_back(p1);
				waypoints[j].push_back(p2);
			}else{
				addReturnPoint(p1, p2, j);
			}
			x0 = xa;
		}
		i++;
	}

	cout<<"Lanes:"<<l<< " dx:"<<dx<<" waypoints:"<<waypoints[0].size()<<endl;
	printWaypoint();
}

void SearchAreaTask::calculateReturnDy() {
	//return_dy[0] = (lane_width*turn_way*turn_way -3 + lane_offset)/(2*turn_way);
	double s = 3 - lane_offset;
	double tw = turn_way*lane_width;
	double s2 = s*s;
	double lw2 = lane_width*lane_width;
	return_dy[0] = tw/2 -(s2*lw2)/(2*tw);
	return_dy[1] = sqrt(tw*tw-s2*lw2)/2;
	return_dy[2] = return_dy[0];
}

void SearchAreaTask::rotateWaypoints(double alfa) {

	for (int j = 0; j < 3; j++) {
		std::vector<XYPoint> way;
		for (vector<XYPoint>::iterator it = waypoints[j].begin(); it != waypoints[j].end(); ++it) {
			XYPoint p = rotatePoint(*it, 0, 0, alfa);
			p.set_vx(p.x()+region[0].x());
			p.set_vy(p.y()+region[0].y());
			way.push_back(p);
		}
		waypoints[j].clear();
		waypoints[j] = way;
	}
}

int SearchAreaTask::parseMission(string msg) {
	msg = removeWhite(msg);
	string resultParse;
	int err = -1;
	
	vector<string> vectorOfKeyValues = parseStringQ(msg, ',');
	for(int i=0;i<vectorOfKeyValues.size();i++){
		vector<string> keyValue = parseString(vectorOfKeyValues[i], '=');
		
		if(keyValue[0] == M_ACTION){
			resultParse = keyValue[1];
			if(resultParse == M_ACTION_SAVE)
				setAction(0);
			else if(resultParse == M_ACTION_START)
				setAction(1);
			else if(resultParse == M_ACTION_STOP)
				setAction(2);
			else
				err = ERROR_WRONG_VALUE;
		}
		else if(keyValue[0] == M_WHO){
			resultParse = keyValue[1];
			if(resultParse == M_SWARM)
				setWho(0);
			else if(strBegins(resultParse,M_VEHICLE,false)){
				string vehicleNumber = truncString(resultParse,1,"back");
				if(isNumber(vehicleNumber)){
					int vehicleN = atoi(vehicleNumber.c_str());
					if(vehicleN <= NUMBER_OF_VEHICLES && vehicleN > 0)
						setWho(vehicleN);
					else
						err = ERROR_WRONG_VEHICLE_NUMBER;
				}
				else
					err = ERROR_WRONG_VALUE;
			}
			else
				err = ERROR_WRONG_VALUE;
		}
		else if(keyValue[0] == M_DEPTH){
			resultParse = keyValue[1];
			if(isNumber(resultParse,false))
				setDepth(atof(resultParse.c_str()));
			else
				err = ERROR_IS_NOT_A_NUMBER;
		}
		else if(keyValue[0] == M_MAX_DEPTH){
			resultParse = keyValue[1];
			if(isNumber(resultParse,false))
				setMaxDepth(atof(resultParse.c_str()));
			else
				err = ERROR_IS_NOT_A_NUMBER;
		}
		else if(keyValue[0] == M_MAX_TIME){
			resultParse = keyValue[1];
			if(isNumber(resultParse,false))
				setMaxTime(atoi(resultParse.c_str()));
			else
				err = ERROR_IS_NOT_A_NUMBER;
		}
		else if(keyValue[0] == M_SPEED){
			resultParse = keyValue[1];
			if(isNumber(resultParse,false))
				setSpeed(atof(resultParse.c_str()));
			else
				err = ERROR_IS_NOT_A_NUMBER;
		}
		else if(keyValue[0] == M_DISTANCE){
			resultParse = keyValue[1];
			if(isNumber(resultParse,false))
				setDistance(atof(resultParse.c_str()));
			else
				err = ERROR_IS_NOT_A_NUMBER;
		}
		else if(keyValue[0] == M_FORMATION){
			resultParse = keyValue[1];
			if(resultParse == M_FORMATION_TRIANGLE)
				setFormation(0);
			else
				err = ERROR_WRONG_VALUE;
		}
		else if(keyValue[0] == M_NAME){
			resultParse = keyValue[1];
				setName(resultParse);
		}
		else if(keyValue[0] == M_START_POINT && isBraced(keyValue[1])){
			string withoutBraces = stripBraces(keyValue[1]);
			vector<string> point = parseString(withoutBraces, ',');
			if(isNumber(point[0],false) && isNumber(point[1],false))
				setStart_point(atof(point[0].c_str()),atof(point[1].c_str()));
			else
				err = ERROR_IS_NOT_A_NUMBER;
		}
		else if(keyValue[0] == M_END_POINT && isBraced(keyValue[1])){
			string withoutBraces = stripBraces(keyValue[1]);
			vector<string> point = parseString(withoutBraces, ',');
			if(isNumber(point[0],false) && isNumber(point[1],false))
				setEnd_point(atof(point[0].c_str()),atof(point[1].c_str()));
			else
				err = ERROR_IS_NOT_A_NUMBER;
		}
		else if(keyValue[0] == M_RECTANGLE && isBraced(keyValue[1])){
			string withoutBraces = stripBraces(keyValue[1]);
			vector<string> points = parseString(withoutBraces, ':');
			if(points.size() == 4)
				for(int j=0;j<4;j++){
					vector<string> point = parseString(points[j], ',');
					if(isNumber(point[0],false) && isNumber(point[1],false))
						addRectanglePoint(atof(point[0].c_str()),atof(point[1].c_str()),j);
					else
						err = ERROR_WRONG_RECTANGLE;
				}
			else
				err = ERROR_WRONG_RECTANGLE;
			if(err == -1)
				setRectangleSet(true);
		}
		else if(keyValue[0] == M_POINTS && isBraced(keyValue[1])){
			string withoutBraces = stripBraces(keyValue[1]);
			vector<string> points = parseString(withoutBraces, ':');
			for(int j=0;j<points.size();j++){
				vector<string> point = parseString(points[j], ',');
				if(isNumber(point[0],false) && isNumber(point[1],false))
					addWaypoint(atof(point[0].c_str()),atof(point[1].c_str()));
				else
					err = ERROR_WRONG_POINT;
			}
			if(err == -1)
				setWaypointsSet(true);
		}
		else if(keyValue[0] == M_TIME_START && isBraced(keyValue[1])){
			string withoutBraces = stripBraces(keyValue[1]);
			vector<string> point = parseString(withoutBraces, ',');
			if(isNumber(point[0],false) && isNumber(point[1],false)){
				struct timeval start;
				start.tv_sec = atoi(point[0].c_str());
				start.tv_usec = atoi(point[1].c_str());
				setTimeStart(start);
			}
			else
				err = ERROR_IS_NOT_A_NUMBER;
		}
		else if(keyValue[0] == M_SAFETY_REGION && isBraced(keyValue[1])){
			string withoutBraces = stripBraces(keyValue[1]);
			vector<string> points = parseString(withoutBraces, ':');
			for(int j=0;j<points.size();j++){
				vector<string> point = parseString(points[j], ',');
				if(isNumber(point[0],false) && isNumber(point[1],false))
					safety_region.add_vertex(atof(point[0].c_str()),atof(point[1].c_str()));
				else
					err = ERROR_WRONG_POINT;
			}
			if(safety_region.size() < 3)
				err = ERROR_TOO_FEW_POINTS;
			if(err == -1)
				setSafetyRegionSet(true);
		}
	}
	
	if(err > 0){
		clear();
		return err;
	}
	
	return ERROR_NO_ERROR;
}

bool SearchAreaTask::isMissionStartOrStop(string msg, short &what_action) {
	if(strContains(msg,M_ACTION)){
		string action = tokStringParse(msg, M_ACTION, ',', '=');
		if(action == M_ACTION_START) {
			what_action = 1;
			return true;
		}
		else if(action == M_ACTION_STOP) {
			what_action = 2;
			return true;
		}
	}
	return false;
}

bool SearchAreaTask::isMissionValidForVehicle() {
	if(is_action_set){
		if(action == 0 && (is_depth_set || is_distance_set) && is_start_point_set && is_end_point_set && is_waypoints_set && is_time_start_set && is_safety_region_set && is_max_time_set && is_max_depth_set)
			return true;
	}
	//cout<<action<<" "<<is_depth_set<<" "<<is_distance_set<<" "<<is_start_point_set<<" "<<is_end_point_set<<" "<<is_waypoints_set<<" "<<is_time_start_set<<endl;
	return false;
}

XYSegList SearchAreaTask::getPath()
{
	XYSegList segList;
	segList.add_vertex(getStart_point());
	for (int i = 0; i < waypoints[0].size(); i++) {
		segList.add_vertex(waypoints[0][i]);
	}
	return segList;
}