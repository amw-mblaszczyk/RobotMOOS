/*
 * MsgRemote.h
 *
 *  Created on: 25 lut 2021
 *      Author: marek
 */

#ifndef MSGREMOTE_H_
#define MSGREMOTE_H_

#include <string>

using namespace std;

	static const string R_START_LA = "StartLa";
	static const string R_START_FI = "StartFi";
	static const string R_COURSE = "course";
	static const string R_DEPTH = "depth";
	static const string R_POINT = "Point";
	static const string R_WHO = "who";
	static const string R_JOY_RUDDER = "JoyRudder";
	static const string R_JOY_THRUST = "JoyThrust";
	static const string R_JOY_ELEVATOR = "JoyElevator";

	static const string R_LEFT = "left";
	static const string R_RIGHT = "right";
	static const string R_FORWARD = "forward";
	static const string R_BACKWARD = "backward";
	static const string R_DOWN = "down";
	static const string R_UP = "up";
	static const string R_ON = "On";
	static const string R_OFF = "Off";
	static const string R_SWARM = "swarm";


class MsgRemote {
public:
	MsgRemote();
	virtual ~MsgRemote();

	void clear();
	string get_spec() const;

	short getCourse() const {
		return course;
	}

	void setCourse(short course = -1) {
		this->course = course;
	}

	double getDepth() const {
		return depth;
	}

	void setDepth(double depth = -1.0) {
		this->depth = depth;
	}

	short getJoyElevator() const {
		return joy_elevator;
	}

	void setJoyElevator(short joyElevator = -1) {
		joy_elevator = joyElevator;
	}

	short getJoyRudder() const {
		return joy_rudder;
	}

	void setJoyRudder(short joyRudder = -1) {
		joy_rudder = joyRudder;
	}

	short getJoyThrust() const {
		return joy_thrust;
	}

	void setJoyThrust(short joyThrust = -1) {
		joy_thrust = joyThrust;
	}

	int getPointX() const {
		return point_x;
	}

	void setPointX(int pointX = -1) {
		point_x = pointX;
	}

	int getPointY() const {
		return point_y;
	}

	void setPointXY(int x=-1, int y = -1){
		setPointX(x);
		setPointY(y);
	}

	void setPointY(int pointY = -1) {
		point_y = pointY;
	}

	double getStartLa() const {
		return start_la;
	}

	void setStartLa(double startLa = -1.0) {
		start_la = startLa;
	}

	double getStartLon() const {
		return start_lon;
	}

	void setStartLon(double startLon = -1.0) {
		start_lon = startLon;
	}

	void setStartLatLon(double la=-1.0, double lon=-1.0){
		setStartLa(la);
		setStartLon(lon);
	}

	short getWho() const {
		return who;
	}

	void setWho(short who = -1) {
		this->who = who;
	}

	void addDeviceCommand(string dev_name, string dev_command);

protected:
	string printPointXY() const;

public:

protected:
	double start_la = -1.0;
	double start_lon = -1.0;
	short course = -1;
	double depth = -1.0;
	int point_x = -1;
	int point_y = -1;
	short who = -1;
	short joy_rudder = -1;
	short joy_thrust = -1;
	short joy_elevator = -1;
	string devices;
};

#endif /* MSGREMOTE_H_ */
