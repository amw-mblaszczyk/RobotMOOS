/*
 * Joystick.h
 *
 *  Created on: 12 lut 2021
 *      Author: marek
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "Remote.h"

#include <string>
#include <fcntl.h>

using namespace std;

#define EVDEV_DIR "/dev/input/by-id/"

class Joystick {
private:
	static const short LEFT = 100;
	static const short RIGHT = 200;
	static const short REVERT = 100;
	static const short FORWARD = 200;

	string joystick_dev = EVDEV_DIR;
	short x_stat = 128;
	short y_stat = 128;
	short rx_stat = 128;
	short ry_stat = 128;


public:
	static const int OPEN_FLAGS = O_RDONLY;
	string device;
	bool work = true;


	Joystick();
	virtual ~Joystick();

	static string listDevices(bool print=true);

	const string& findJoystick(uint repeat = 60);

	const string& getJoystickDev() const {
		return joystick_dev;
	}

	void readJoystickPosOld();

	void *joystickThread();

	uint getWho() const {
		return who;
	}

	void setWho(uint who = 0) {
		this->who = who;
	}

	Remote* getRemoteApp() {
		return remote_app;
	}

	void setRemoteApp(Remote *remoteApp) {
		remote_app = remoteApp;
	}

private:
	long tid = 0;

	static void* readJoystickThreadOld(void*);
	short checkPos(short *rel, int new_pos);

protected:
	uint who = 0;
	string genRemoteMsg(int lr=-1, int fr=-1, int d=-1, int zero_lr=-1, int zero_d=-1);
	Remote *remote_app;

};



#endif /* JOYSTICK_H_ */
