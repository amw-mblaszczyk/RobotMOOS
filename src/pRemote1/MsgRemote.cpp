/**
 * MsgRemote.cpp
 *
 *  Created on: 25 lut 2021
 *      Author: marek
 */

#include "MsgRemote.h"
#include "MBUtils.h"

/**
 * Default constructor.
 */
MsgRemote::MsgRemote() {
	// TODO Auto-generated constructor stub

}

MsgRemote::~MsgRemote() {
	// TODO Auto-generated destructor stub
}

void MsgRemote::clear() {
	start_la = -1.0;
	start_lon = -1.0;
	course = -1;
	depth = -1.0;
	point_x = -1;
	point_y = -1;
	who = -1;
	joy_rudder = -1;
	joy_thrust = -1;
	joy_elevator = -1;
	devices = "";
}

/**
 * Generates message from object.
 * @return string with MOSS DB message format.
 */
string MsgRemote::get_spec() const {
	string spec;

	if (start_la > 0.0)
		spec += R_START_LA+"="+doubleToStringX(start_la, 6);
	if (start_lon > 0.0){
		if (spec.length()>3) spec += ",";
		spec += R_START_FI+"="+doubleToStringX(start_lon, 6);
	}
	if (course >= 0){
		if (spec.length()>3) spec += ",";
		spec += R_COURSE+"="+uintToString(course);
	}
	if (depth >= 0.0){
		if (spec.length()>3) spec += ",";
		spec += R_DEPTH+"="+doubleToStringX(depth, 3);
	}
	if ((point_x >= 0) || (point_y >= 0)){
		if (spec.length()>3) spec += ",";
		spec += printPointXY();
	}
	if (who >= 0){
		if (spec.length()>3) spec += ",";
		if (who == 0)
			spec += R_WHO+"="+R_SWARM;
	}
	if (joy_rudder >= 0){
		if (spec.length()>3) spec += ",";
		if (joy_rudder == 0) /*LEFT*/ spec += R_JOY_RUDDER+"="+R_LEFT;
		else if (joy_rudder == 1) /*CENTER*/ spec += R_JOY_RUDDER + "=0";
		else if (joy_rudder == 2) /*CENTER*/ spec += R_JOY_RUDDER +  "="+R_RIGHT;
	}
	if (joy_thrust >= 0){
		if (spec.length()>3) spec += ",";
		if (joy_thrust == 0) /*accelerate*/ spec += R_JOY_THRUST +  "="+R_FORWARD;
		else if (joy_thrust == 1) /*CENTER*/ spec += R_JOY_THRUST +  "=0";
		else if (joy_thrust == 2) /*CENTER*/ spec += R_JOY_THRUST +  "="+R_BACKWARD;
	}
	if (joy_elevator >= 0){
		if (spec.length()>3) spec += ",";
		if (joy_elevator == 0) /*down*/ spec += R_JOY_ELEVATOR + "="+R_DOWN;
		else if (joy_elevator == 1) /*CENTER*/ spec += R_JOY_ELEVATOR + "=0";
		else if (joy_elevator == 2) /*up*/ spec += R_JOY_ELEVATOR + "="+R_UP;
	}
	if (devices.length() >0){
		if (spec.length()>3) spec += ",";
		spec += devices;
	}


/*
	joy_rudder = -1;
	joy_thrust = -1;
	joy_elevator = -1;
	devices = "";
	*/

	return spec;
}

/**
 * Adds device with command e.g addDeviceCommand("Camera","Off") adds "Camera=Off" to the output message.
 * @param dev_name device name e.g. "Mag", "Camera", "Lights"
 * @param dev_command device command e.g. "On", "Off".
 */
void MsgRemote::addDeviceCommand(string dev_name, string dev_command) {
	if (devices.length()>3)
		devices.append(",");
	devices.append(dev_name).append("=").append(dev_command);
}

/**
 * Generates mesage text representing XYPoint e.g. "Point={30,20}".
 * @return string representation of point_x,point_y in MOSS DB messages format ("Point={point_x,point_y}")
 */
string MsgRemote::printPointXY() const {
	string str;
	str = R_POINT + "={" + uintToString(point_x) + "," + uintToString(point_y)+ "}";
	return str;
}
