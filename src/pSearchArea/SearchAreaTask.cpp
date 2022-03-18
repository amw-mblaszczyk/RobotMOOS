/*
 * SearchAreaTask.cpp
 *
 *  Created on: 25 mar 2021
 *      Author: marek
 */
#include <math.h>
#include <iostream>
#include <limits>
#include "SearchAreaTask.h"
#include "GeomUtils.h"

using namespace std;

SearchAreaTask::SearchAreaTask() {
	// TODO Auto-generated constructor stub

}

SearchAreaTask::~SearchAreaTask() {
	// TODO Auto-generated destructor stub
}

double SearchAreaTask::distance(XYPoint p1, XYPoint p2) {
	double dx = p1.x() - p2.x();
	double dy = p1.y() - p2.y();
	double dist = distPointToPoint(p1, p2);
	//cout<<"distance:"<<dist<<endl;
	//return sqrt(dx*dx+dy*dy);
	return dist;
}

int SearchAreaTask::getFirstPointIdx() {
	double dist[4];
	int idx = -1;
	double mindist = numeric_limits<double>::max();
	for (int var = 0; var < 4; ++var) {
		dist[var] = distance(region[var], start_point);
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

	if (distance(region[0], region[1]) > distance(region[0], region[3]))
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
	cout << "CHECK TURN2:" << distance(waypoints[idx].back(), p) << " "
			<< distance(p, p1) << " "
			<< distance(waypoints[idx].back(), p) + distance(p, p1) << endl;

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
	double dist = distance(rreg[0], rreg[1]);
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
