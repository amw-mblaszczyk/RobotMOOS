/**
 * MsgRemote.cpp
 *
 *  Created on: 25 lut 2021
 *      Author: marek
 */
#define Tests_On_The_Surface


#include "MsgRemote.h"
#include "MBUtils.h"
#include <iterator>
#include "ACTable.h"
#include <iostream>

/**
 * Default constructor.
 */
MsgRemote::MsgRemote() {
	clear();
}

/**
 * Constructor: object created from message string.
 */
MsgRemote::MsgRemote(string msg) {
	clear();
	parsing_error = parseRemote(msg);
}

MsgRemote::~MsgRemote() {
	// TODO Auto-generated destructor stub
}

void MsgRemote::clear() {
	
	is_camera_set = false;
	is_magnetometer_set = false;
	is_course_set = false;
	is_depth_set = false;
	is_speed_set = false;
	is_point_x_set = false;
	is_point_y_set = false;
	is_who_set = false;
	is_joy_rudder_set = false;
	is_joy_thrust_set = false;
	is_joy_elevator_set = false;
}

/**
 * Generates message from object.
 * @return string with MOSS DB message format.
 */
string MsgRemote::get_spec() const {
	string spec;

	if (is_camera_set){
		spec = R_CAMERA+"=";
		if(camera)
			spec += R_ON;
		else
			spec += R_OFF;
	}
	if (is_magnetometer_set){
		if (spec.length()>0) spec += ",";
		spec += R_MAG+"=";
		if(magnetometer)
			spec += R_ON;
		else
			spec += R_OFF;
	}
	if (is_course_set){
		if (spec.length()>0) spec += ",";
		spec += R_COURSE+"="+doubleToStringX(course);
	}
	if (is_depth_set){
		if (spec.length()>0) spec += ",";
		spec += R_DEPTH+"="+doubleToStringX(depth, 3);
	}
	if (is_speed_set){
		if (spec.length()>0) spec += ",";
		spec += R_SPEED+"="+doubleToStringX(speed, 3);
	}
	if (is_point_x_set && is_point_y_set){
		if (spec.length()>0) spec += ",";
		spec += printPointXY();
	}
	if (is_who_set){
		if (spec.length()>0) spec += ",";
		spec += R_WHO+"="+who;
	}
	if (is_joy_rudder_set){
		if (spec.length()>0) spec += ",";
		if (joy_rudder == 0) /*LEFT*/ spec += R_JOY_RUDDER+"="+R_LEFT;
		else if (joy_rudder == 1) /*CENTER*/ spec += R_JOY_RUDDER + "="+R_ZERO;
		else if (joy_rudder == 2) /*CENTER*/ spec += R_JOY_RUDDER +  "="+R_RIGHT;
	}
	if (is_joy_thrust_set){
		if (spec.length()>0) spec += ",";
		if (joy_thrust == 0) /*accelerate*/ spec += R_JOY_THRUST +  "="+R_FORWARD;
		else if (joy_thrust == 1) /*CENTER*/ spec += R_JOY_THRUST + "="+R_ZERO;
		else if (joy_thrust == 2) /*CENTER*/ spec += R_JOY_THRUST +  "="+R_BACKWARD;
	}
	if (is_joy_elevator_set){
		if (spec.length()>0) spec += ",";
		if (joy_elevator == 0) /*down*/ spec += R_JOY_ELEVATOR + "="+R_DOWN;
		else if (joy_elevator == 1) /*CENTER*/ spec += R_JOY_ELEVATOR + "="+R_ZERO;
		else if (joy_elevator == 2) /*up*/ spec += R_JOY_ELEVATOR + "="+R_UP;
	}
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

/**
 * Tests whether the message is for receiver".
 * @return if true the message is either for the whole swarm or for the vehicle whose community is called my_name 
 */
bool MsgRemote::isForMe(string msg, string my_name) {
	if(strContains(msg,R_WHO)){
		string who = tokStringParse(msg, R_WHO, ',', '=');
		if(who == R_SWARM || who == my_name)
			return true;
	}
	return false;
}

/**
 * Tests whether the message is for joy handler.
 * @return if true the message is for joy handler 
 */
bool MsgRemote::isForJoy(string msg) {
	return (strContains(msg,R_JOY_RUDDER) || strContains(msg,R_JOY_THRUST) || strContains(msg,R_JOY_ELEVATOR)
			|| strContains(msg,R_JOY_WHO) || strContains(msg,R_JOY_ACTIVE));
}


/**
 * Parses message string and fills parameters of MsgRemote object ".
 * @return integer which determines correctness of parsing, 0 - means no error, value greater than 0 means error
 */
int MsgRemote::parseRemote(string msg) {
	msg = removeBlanks(msg);
	string resultParse;
	
	vector<string> vectorOfKeyValues = parseStringQ(msg, ',');
	for(int i=0;i<vectorOfKeyValues.size();i++){
		vector<string> keyValue = parseString(vectorOfKeyValues[i], '=');
		
		if(keyValue[0] == R_MAG){
			resultParse = keyValue[1];
			if(resultParse == R_ON)
				setMagnetometer(true);
			else if(resultParse == R_OFF)
				setMagnetometer(false);
			else{
				clear();
				return ERROR_WRONG_VALUE;
			}
		}
		else if(keyValue[0] == R_CAMERA){
			resultParse = keyValue[1];
			if(resultParse == R_ON)
				setCamera(true);
			else if(resultParse == R_OFF)
				setCamera(false);
			else{
				clear();
				return ERROR_WRONG_VALUE;
			}
		}
		else if(keyValue[0] == R_COURSE){
			resultParse = keyValue[1];
			if(isNumber(resultParse,false))
				setCourse(atof(resultParse.c_str()));
			else{
				clear();
				return ERROR_IS_NOT_A_NUMBER;
			}
		}
#ifdef Tests_On_The_Surface
		else if(keyValue[0] == R_DEPTH){
			resultParse = keyValue[1];
			if(isNumber(resultParse,false))
				setDepth(atof(resultParse.c_str()));
			else{
				clear();
				return ERROR_IS_NOT_A_NUMBER;
			}
		}
#endif
		else if(keyValue[0] == R_SPEED){
			resultParse = keyValue[1];
			if(isNumber(resultParse,false))
				setSpeed(atof(resultParse.c_str()));
			else{
				clear();
				return ERROR_IS_NOT_A_NUMBER;
			}
		}
		else if(keyValue[0] == R_POINT && isBraced(keyValue[1])){
			string withoutBraces = stripBraces(keyValue[1]);
			vector<string> point = parseString(withoutBraces, ',');
			if(isNumber(point[0],false) && isNumber(point[1],false))
				setPointXY(atof(point[0].c_str()),atof(point[1].c_str()));
			else{
				clear();
				return ERROR_IS_NOT_A_NUMBER;
			}
		}
		else if(keyValue[0] == R_JOY_RUDDER){
			resultParse = keyValue[1];
			if(resultParse == R_LEFT)
				setJoyRudder(0);
			else if(resultParse == R_ZERO)
				setJoyRudder(1);
			else if(resultParse == R_RIGHT)
				setJoyRudder(2);
			else{
				clear();
				return ERROR_WRONG_VALUE;
			}
		}
		else if(keyValue[0] == R_JOY_THRUST){
			resultParse = keyValue[1];
			if(resultParse == R_FORWARD)
				setJoyThrust(0);
			else if(resultParse == R_ZERO)
				setJoyThrust(1);
			else if(resultParse == R_BACKWARD)
				setJoyThrust(2);
			else{
				clear();
				return ERROR_WRONG_VALUE;
			}
		}
#ifdef Tests_On_The_Surface
		else if(keyValue[0] == R_JOY_ELEVATOR){
			resultParse = keyValue[1];
			if(resultParse == R_DOWN)
				setJoyThrust(0);
			else if(resultParse == R_ZERO)
				setJoyThrust(1);
			else if(resultParse == R_UP)
				setJoyThrust(2);
			else{
				clear();
				return ERROR_WRONG_VALUE;
			}
		}
#endif
	}
	
	return ERROR_NO_ERROR;
}

string MsgRemote::removeBlanks(string str) {
   string result;
   for (size_t i = 0; i < str.size(); i++) {
          char currentChar = str[i];
          if (currentChar != ' ')
              result += currentChar;
   }
       return result;
}


