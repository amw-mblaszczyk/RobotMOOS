/************************************************************/
/*    NAME: MB                                              */
/*    ORGN: MIT                                             */
/*    FILE: JoystickTest.cpp                                        */
/*    DATE:                                                 */
/************************************************************/
#include <assert.h>
#include <dirent.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <iostream>

#include "JoystickTest.h"

using namespace std;

//---------------------------------------------------------
// Constructor

JoystickTest::JoystickTest()
{
}

//---------------------------------------------------------
// Destructor

JoystickTest::~JoystickTest()
{
}

string JoystickTest::listDevices(bool print) {
	DIR *dirp = opendir(EVDEV_DIR);
	struct dirent *dp;
	string dev;
	const char *pattern = "event-joystick";

	if (dirp == NULL)
		return NULL;

	// Loop over dir entries using readdir
	size_t len = strlen(pattern);
	while ((dp = readdir(dirp)) != NULL) {

		// Only select names that end in 'event-joystick'
		size_t devlen = strlen(dp->d_name);
		if (devlen >= len) {
			const char *const start = dp->d_name + devlen - len;

			if (strncmp(start, pattern, len) == 0) {
				//printf("%s%s\n", EVDEV_DIR, dp->d_name);
				dev = string(EVDEV_DIR).append(+dp->d_name);
			}
		}
	}
	return dev;
}

void JoystickTest::readJoystick(long joystick_id) {

	//Read joystick till active and worked
	while (work && (joystick_id > 0)) {
		int r1 = 0;
		struct input_event events[4]; // 8 events per frame is pretty extreme, more like 1 or 2, sometimes as high as 4
		r1 = read(joystick_id, events, sizeof events);

		if (r1 > 0) {
			int new_event_count = r1 / sizeof(struct input_event);
			//cout << "Events count:" << new_event_count << endl;
			for (int evi = 0; evi < new_event_count; evi++) {
				auto &ev = events[evi];
				//cout << "Result:" << r1 << " EV_TYPE:" << ev.type << endl;
				switch (ev.type) {
				case EV_ABS: {
					//cout << "EV_ABS EV_CODE:" << ev.code << endl;
					read_joy_abs(ev);
				}
					break;
				case EV_KEY: {
					//cout << "EV_KEY EV_CODE:" << ev.code << endl;
					read_joy_key(ev);
				}
				}
			}

		}
		usleep(20 * 1000);
	}
	//close gamepad device if still opened
	if (gamepad > 0)
		close (gamepad);
}

long JoystickTest::connectToJoystick(string device, int sek = 10) {
	long jid = -1;
		if (!device.empty())
			jid = open(device.c_str(), OPEN_FLAGS);

	if (jid >0)
		cout << endl << "Joystick connected. Joystick ID:" << jid << endl;
	else
		cerr << endl << "Joystick not connected." << gamepad << endl;
	return jid;
}

void JoystickTest::closeJoystick(long joystickId) {
	if (joystickId > 0)
		close (joystickId);
}

void JoystickTest::read_joy_abs(struct input_event ev) {
	switch (ev.code) {
	case ABS_X: {
		cout << "ABS_X (" << ABS_X << ") value:" << ev.value << endl;
	}
		break;
	case ABS_Y: {
		cout << "ABS_Y (" << ABS_Y << ") value:" << ev.value << endl;
	}
		break;
	case ABS_RX: {
		cout << "ABS_RX (" << ABS_RX << ") value:" << ev.value << endl;
	}
		break;
	case ABS_RY: {
		cout << "ABS_RY (" << ABS_RY << ") value:" << ev.value << endl;
	}
		break;
	case ABS_Z: {
		cout << "ABS_Z (" << ABS_Z << ") value:" << ev.value << endl;
	}
		break;
	case ABS_RZ: {
		cout << "ABS_RZ (" << ABS_RZ << ") value:" << ev.value << endl;
	}
		break;
	default: {
		cout << "ABS_UNKNOWN EV_CODE:" << ev.code << " value:" << ev.value << endl;
	}
	}
}

void JoystickTest::read_joy_key(struct input_event ev) {
	switch (ev.code) {
	case BTN_A: {
		cout<<"BTN_A ("<<BTN_A<<") value:"<<ev.value<<endl;
	}
		break;
	case BTN_B: {
		cout<<"BTN_B ("<<BTN_B<<") value:"<<ev.value<<endl;
	}
		break;
	case BTN_X: {
		cout<<"BTN_X ("<<BTN_X<<") value:"<<ev.value<<endl;
	}
		break;
	case BTN_Y: {
		cout<<"BTN_Y ("<<BTN_Y<<") value:"<<ev.value<<endl;
	}
		break;
		default: {
			cout << "BTN_UNKNOWN BTN_CODE:"<< ev.code << ") value:" << ev.value << endl;
		}

	}

	if (ev.code == BTN_A) {
		if (ev.value == 1)
			counter++;
		if (counter > 4)
			work = false;
	}else
		counter = 0;
}

void* JoystickTest::readCommand(void* p) {
	JoystickTest *j = (JoystickTest *)p;
	while (j->work) {
		cout << "Type quite<ENTER> for program exit." << endl;
		string cmd;
		cin >> cmd;
		if (cmd == "quite") {
			cout << "User exiting request. Press any joystick button to close application." << endl;
			j->work = false;
		}
	}
	return NULL;
}

void* JoystickTest::exitThread() {
	pthread_create(&tid, NULL, JoystickTest::readCommand, this);
	sleep(1);
	return NULL;
}
