/*
 * Joystick.h
 *
 *  Created on: 12 lut 2021
 *      Author: marek
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <string>
#include <fcntl.h>
#include <iostream>

class Remote;
using namespace std;

#define EVDEV_DIR "/dev/input/by-id/"


class Joystick {

private:
	static const short LEFT = 100;
	static const short RIGHT = 200;
	static const short REVERT = 100;
	static const short FORWARD = 200;

	string joystick_dev = EVDEV_DIR;

public:
	/**
	 * Flags for joystick device file.
	 */
	static const int OPEN_FLAGS = O_RDONLY;

	/**
	 * Default constructor for class Joystick.
	 */
	Joystick();

	/**
	 * Defaould destructor.
	 */
	virtual ~Joystick();

	/**
	 * Finds and prints joystick device file.
	 * @param print: if prints device name on standard output (terminal). Default: true.
	 * @return name of joystick device.
	 */
	static string listDevices(bool print = true);

	/**
	 * Searching connected joystick device for defined seconds.
	 * @param repeat how long in sec. check for connected joystick.
	 * @return name of connected joystick device or null if joystick was not found.
	 */
	const string& findJoystick(uint repeat = 60);

	/**
	 * Accessor for joystick device name.
	 * @return name of detected joystick device name or null if no joystick detected.
	 */
	const string& getJoystickDev() const {
		return joystick_dev;
	}


	/**
	 * Reapeadly reads joystick device and generates Remote MOSSDB messages.
	 * It reads joystick till "work" is true.
	 */
	void readJoystickPos();


	/**
	 * Reapeadly reads joystick device and generates Remote MOSSDB messages.
	 * It reads joystick till "work" is true.
	 */
	void readJoystickPosOld();

	/**
	 * Creates and starts thread for joystick reading.
	 */
	void* joystickThread();

	/**
	 * Reads selected AUV for remote commands.
	 * @return selected AUV ID, 0 for swarm, 1: first AUV, 2: second AUV, ...
	 */
	void setWho(const string &who = "swarm") {
		this->who = who;
	}

	const string& getWho() const {
		return who;
	}


	/**
	 * Gets MOOS RemoteApp for communication with MOSSDB
	 * @return MOOS RemoteApp
	 */
	Remote* getRemoteApp() {
		return remote_app;
	}

	/**
	 * Sets MOOS RemoteApp for communication with MOSSDB
	 * @param remoteApp MOSS Application
	 */
	void setRemoteApp(Remote *remoteApp) {
		//cout<<"Remote add set to:"<<remoteApp<<endl;
		remote_app = remoteApp;
	}

	bool isActive() const {
		return active;
	}

	void setActive(bool active = false) {
		this->active = active;
	}


protected:
	/**
	 * Generates REMOTE MOOSDB message.
	 * @param lr - left/right move 0: left, 1: neutral, 2: right
	 * @param fr - forward/reverse 0: forward, 1: neutral, 2: reverse
	 * @param d - up/down 0: down, 1: neutral, 2: up
	 * @param zero_lr - set rudder to zero
	 * @param zero_d - set thrust to zero
	 * @return REMOTE MOSSDB message body
	 */
	string genRemoteMsg(int lr = -1, int fr = -1, int d = -1, int zero_lr = -1,
			int zero_d = -1);

	/**
	 * Reading joystick for thread. First searching joystick device for 30 sec. and when found start reading.
	 * @param  pointer to Joystick object.
	 */
	static void* readJoystickThreadOld(void*);

	static void* readJoystickThread(void*);

	/**
	 * Utility method converting actual joystick position to normalized position and returns REMOTE message codes.
	 * 0-64 is converted to  0, 65-192 is converted to 128, 193-255 is converted to 255
	 * @param rel : pointer to direction parameter to change (left/right or forward/backward)
	 * @param new_pos - REMOTE code: 0, 1, 2
	 */
	short checkPos(short *rel, int new_pos);

protected:
	bool work = true;
	string who = "swarm";
	bool active = true;
	Remote *remote_app;
	short x_stat = 128;
	short y_stat = 128;
	short rx_stat = 128;
	short ry_stat = 128;
	int gamepad = -1;
	long tid = 0;


};

#endif /* JOYSTICK_H_ */
