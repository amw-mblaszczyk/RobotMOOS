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
/*
 * etykiety z duzej litery, a wartosci z malej litery
 */
	static const string R_MAG = "Mag";
	static const string R_CAMERA = "Camera";
	static const string R_COURSE = "Course";
	static const string R_DEPTH = "Depth";
	static const string R_SPEED = "Speed";
	static const string R_POINT = "Point";
	static const string R_WHO = "Who";
	static const string R_JOY_RUDDER = "JoyRudder";
	static const string R_JOY_THRUST = "JoyThrust";
	static const string R_JOY_ELEVATOR = "JoyElevator";

	static const string R_LEFT = "left";
	static const string R_RIGHT = "right";
	static const string R_ZERO = "zero";
	static const string R_FORWARD = "forward";
	static const string R_BACKWARD = "backward";
	static const string R_DOWN = "down";
	static const string R_UP = "up";
	static const string R_ON = "on";
	static const string R_OFF = "off";
	static const string R_SWARM = "swarm";
	static const string R_VEHICLE = "vehicle";
	
	static const int NUMBER_OF_ERRORS = 3; 
	static const int ERROR_NO_ERROR = 0; 
	static const int ERROR_WRONG_VALUE = 1; 
	static const int ERROR_IS_NOT_A_NUMBER = 2; 
	static const string parsingErrors[NUMBER_OF_ERRORS] = {"No error","Error: wrong value","Error: value is not a number"};


class MsgRemote {
public:
	MsgRemote();
	MsgRemote(string msg);
	virtual ~MsgRemote();

	void clear();
	string get_spec() const;

	bool getCamera() const {
		return camera;
	}

	void setCamera(bool camera) {
		this->camera = camera;
		is_camera_set = true;
	}
	
	bool isCameraSet() const {
		return is_camera_set;
	}
	
	bool getMagnetometer() const {
		return magnetometer;
	}

	void setMagnetometer(bool magnetometer) {
		this->magnetometer = magnetometer;
		is_magnetometer_set = true;
	}
	
	bool isMagnetometerSet() const {
		return is_magnetometer_set;
	}
	
	float getCourse() const {
		return course;
	}
	
	void setCourse(float course) {
		this->course = course;
		is_course_set = true;
	}
	
	bool isCourseSet() const {
		return is_course_set;
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
	
	float getSpeed() const {
		return speed;
	}

	void setSpeed(float speed) {
		this->speed = speed;
		is_speed_set = true;
	}
	
	bool isSpeedSet() const {
		return is_speed_set;
	}

	short getJoyElevator() const {
		return joy_elevator;
	}

	void setJoyElevator(short joyElevator) {
		joy_elevator = joyElevator;
		is_joy_elevator_set = true;
	}
	
	bool isJoyElevatorSet() const {
		return is_joy_elevator_set;
	}

	short getJoyRudder() const {
		return joy_rudder;
	}

	void setJoyRudder(short joyRudder) {
		joy_rudder = joyRudder;
		is_joy_rudder_set = true;
	}
	
	bool isJoyRudderSet() const {
		return is_joy_rudder_set;
	}

	short getJoyThrust() const {
		return joy_thrust;
	}

	void setJoyThrust(short joyThrust) {
		joy_thrust = joyThrust;
		is_joy_thrust_set = true;
	}
	
	bool isJoyThrustSet() const {
		return is_joy_thrust_set;
	}

	float getPointX() const {
		return point_x;
	}

	void setPointX(float pointX) {
		point_x = pointX;
		is_point_x_set = true;
	}

	float getPointY() const {
		return point_y;
	}
	
	void setPointY(float pointY) {
		point_y = pointY;
		is_point_y_set = true;
	}

	void setPointXY(float x, float y){
		setPointX(x);
		setPointY(y);
		is_point_x_set = true;
		is_point_y_set = true;
	}
	
	bool isPointSet() const {
			return (is_point_y_set && is_point_x_set);
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
	
	string getParsingError(){
	return parsingErrors[parsing_error];
	}
	
	static bool isForMe(string msg, string my_name);
	static bool isForJoy(string msg);
	void addDeviceCommand(string dev_name, string dev_command);

protected:
	string printPointXY() const;

public:

protected:
    
	bool camera;
	bool magnetometer;
	float course;
	float depth;
	float speed;
	float point_x;
	float point_y;
	short who;
	short joy_rudder;
	short joy_thrust;
	short joy_elevator;
	string devices;
	
	bool is_camera_set;
	bool is_magnetometer_set;
	bool is_course_set;
	bool is_depth_set;
	bool is_speed_set;
	bool is_point_x_set;
	bool is_point_y_set;
	bool is_who_set;
	bool is_joy_rudder_set;
	bool is_joy_thrust_set;
	bool is_joy_elevator_set;
	
	int parsing_error;
	
private:

	int parseRemote(string msg);
	//static string removeBlanks(string msg);
};

#endif /* MSGREMOTE_H_ */
