/*
 * Joystick.cpp
 *
 *  Created on: 12 lut 2021
 *      Author: marek
 */

#include "Joystick.h"
#include "MsgRemote.h"
#include "Remote.h"

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

Joystick::Joystick() {
	// TODO Auto-generated constructor stub
}

Joystick::~Joystick() {
}

string Joystick::listDevices(bool print) {

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
				printf("%s%s\n", EVDEV_DIR, dp->d_name);
				dev = string(EVDEV_DIR).append(+dp->d_name);
			}
		}
	}
	return dev;
}

const string& Joystick::findJoystick(uint repeat) {
	bool works = true;
	while (works) {
		joystick_dev = listDevices();
		if (joystick_dev.empty()) {
			if (repeat > 0) {
				cout << "Joystick not found. Trying ..." << endl;
				sleep(1);
				repeat--;
			} else
				works = false;
		} else
			works = false;
	}

	return joystick_dev;
}

void Joystick::readJoystickPos() {
	bool init = true;
	while (work && isActive() && (gamepad <= 0)) {
		//open joystick device if not success wait 1 sec.
		joystick_dev = listDevices();
		if (!joystick_dev.empty())
			gamepad = open(getJoystickDev().c_str(), OPEN_FLAGS);
		if (gamepad == -1)
			sleep(1);
		if (init) {
			cout << "Waiting joystick";
			init = false;
		}
		cout << '.';
		cout.flush();
	}
	cout << endl << "Joystick connected. Joystick ID:" << gamepad << endl;
	//Set joystick initial parameters
	int gamepad_x = 128;
	int gamepad_y = 128;
	int gamepad_rx = 128;
	int gamepad_ry = 128;
	int gamepad_button_A_down = 0;
	int gamepad_button_B_down = 0;
	int gamepad_button_X_down = 0;
	int gamepad_button_Y_down = 0;
	short a = 0;
	short b = 0;
	short last_a = 0;
	short last_b = 0;
	//Read joystick till active and worked
	while (work && (gamepad > 0)) {
		if (!isActive()){
			sleep(1);
			continue;
		}
		int r1 = 0;
		struct input_event events[4]; // 8 events per frame is pretty extreme, more like 1 or 2, sometimes as high as 4
		//cout << "00000000000000 s=" << sizeof events<<" GAMEPAD:"<<gamepad <<" EVENTS:"<<events<<" r1:"<<r1<< endl;

		r1 = read(gamepad, events, sizeof events);

		if (r1 > 0) {
			int new_event_count = r1 / sizeof(struct input_event);
			//cout << "Events count:" << new_event_count << endl;
			for (int evi = 0; evi < new_event_count; evi++) {
				auto &ev = events[evi];
				//cout << "Result:" << r1 << " EV_TYPE:" << ev.type << endl;
				switch (ev.type) {
				case EV_ABS: {
					//cout << "EV_ABS EV_CODE:" << ev.code << endl;
					if (ev.code == getRemoteApp()->joy_rudder_code)
						gamepad_x = ev.value;
					else if (ev.code == getRemoteApp()->joy_thrust_code)
						gamepad_y = ev.value;
					else if (ev.code == getRemoteApp()->joy_elevator_code)
						gamepad_ry = ev.value;
				}
					break;
				case EV_KEY: {
					//cout << "EV_KEY EV_CODE:" << ev.code << endl;
					if ((ev.code == getRemoteApp()->joy_btn_elevator_0)
							|| (ev.code
									== getRemoteApp()->joy_btn_elevator_0_alt))
						gamepad_button_A_down = ev.value;
					else if ((ev.code == getRemoteApp()->joy_btn_rudder_0)
							|| (ev.code == getRemoteApp()->joy_btn_rudder_0_alt))
						gamepad_button_B_down = ev.value;
				}
					break;
				}
			}
			short r = checkPos(&x_stat, gamepad_x);
			short c = checkPos(&y_stat, gamepad_y);
			short d = checkPos(&ry_stat, gamepad_ry);
			if ((gamepad_button_A_down > 0) && (last_a == 0))
				a = 1;
			else
				a = 0;
			last_a = gamepad_button_A_down;

			if ((gamepad_button_B_down > 0) && (last_b == 0))
				b = 1;
			else
				b = 0;
			last_b = gamepad_button_B_down;

			if ((r != 1) || (c != 1) || (d != 1) || (a != 0) || (b != 0)) {
				/*cout << "checkPos:" << gamepad_x << "," << gamepad_y << ","
				 << gamepad_ry << " stat:(" << x_stat << "," << y_stat
				 << "," << ry_stat << ") " << r << " " << c << " " << d
				 << " Button A:"<< a<< " Button B:"<< b<<endl;*/

				string msg("NOT");
				msg = genRemoteMsg(r, c, d, b, a);
				cout << "MSG:" << msg << " Active:"<<isActive()<<endl;
				if ((getRemoteApp() != NULL) && isActive())
					getRemoteApp()->sendRemoteMsg(msg);
			}
		}
		usleep(20 * 1000);
	}
	//close gamepad device if still opened
	if (gamepad > 0)
		close(gamepad);
}

/**
 * @deprecated
 */
void Joystick::readJoystickPosOld() {

	gamepad = open(getJoystickDev().c_str(), OPEN_FLAGS);
	if (gamepad == -1)
		return;

	int gamepad_x = 128;
	int gamepad_y = 128;
	int gamepad_rx = 128;
	int gamepad_ry = 128;
	int gamepad_button_A_down = 0;
	int gamepad_button_B_down = 0;
	int gamepad_button_X_down = 0;
	int gamepad_button_Y_down = 0;
	short a = 0;
	short b = 0;
	short last_a = 0;
	short last_b = 0;

	while (work) {
		if (isActive()) {
			int r1 = 0;
			struct input_event events[4]; // 8 events per frame is pretty extreme, more like 1 or 2, sometimes as high as 4
			//cout << "00000000000000 s=" << sizeof events<<" GAMEPAD:"<<gamepad <<" EVENTS:"<<events<<" r1:"<<r1<< endl;
			struct input_event events1[4];

			r1 = read(gamepad, events, sizeof events);

			if (r1 > 0) {
				int new_event_count = r1 / sizeof(struct input_event);
				//cout << "Events count:" << new_event_count << endl;
				for (int evi = 0; evi < new_event_count; evi++) {
					auto &ev = events[evi];
					//cout << "Result:" << r1 << " EV_TYPE:" << ev.type << endl;
					switch (ev.type) {
					case EV_ABS: {
						//cout << "EV_ABS EV_CODE:" << ev.code << endl;
						if (ev.code == getRemoteApp()->joy_rudder_code)
							gamepad_x = ev.value;
						else if (ev.code == getRemoteApp()->joy_thrust_code)
							gamepad_y = ev.value;
						else if (ev.code == getRemoteApp()->joy_elevator_code)
							gamepad_ry = ev.value;
					}
						break;
					case EV_KEY: {
						//cout << "EV_KEY EV_CODE:" << ev.code << endl;
						if ((ev.code == getRemoteApp()->joy_btn_elevator_0)
								|| (ev.code
										== getRemoteApp()->joy_btn_elevator_0_alt))
							gamepad_button_A_down = ev.value;
						else if ((ev.code == getRemoteApp()->joy_btn_rudder_0)
								|| (ev.code
										== getRemoteApp()->joy_btn_rudder_0_alt))
							gamepad_button_B_down = ev.value;
					}
						break;
					}
				}
				short r = checkPos(&x_stat, gamepad_x);
				short c = checkPos(&y_stat, gamepad_y);
				short d = checkPos(&ry_stat, gamepad_ry);
				if ((gamepad_button_A_down > 0) && (last_a == 0))
					a = 1;
				else
					a = 0;
				last_a = gamepad_button_A_down;

				if ((gamepad_button_B_down > 0) && (last_b == 0))
					b = 1;
				else
					b = 0;
				last_b = gamepad_button_B_down;

				if ((r != 1) || (c != 1) || (d != 1) || (a != 0) || (b != 0)) {
					/*cout << "checkPos:" << gamepad_x << "," << gamepad_y << ","
					 << gamepad_ry << " stat:(" << x_stat << "," << y_stat
					 << "," << ry_stat << ") " << r << " " << c << " " << d
					 << " Button A:"<< a<< " Button B:"<< b<<endl;*/

					string msg("NOT");
					msg = genRemoteMsg(r, c, d, b, a);
					//cout << "MSG:" << msg << endl;
					if (getRemoteApp() != NULL)
						getRemoteApp()->sendRemoteMsg(msg);
				}
			}
		}
		cout << "Before sleep." << endl;
		usleep(1000); //wait 1 sec.
		cout << "After sleep." << endl;

	}
	cout << "Reading joystick interrupted." << endl;
	if (gamepad > 0)
		close(gamepad);

}

short Joystick::checkPos(short *rel, int new_pos) {
		if ((new_pos < 64) && (*rel > 0)) {
			*rel = 0;
			return 0;
		} else if ((new_pos > 192) && (*rel < 255)) {
			*rel = 255;
			return 2;
		} else {
			if ((new_pos > 64) && (new_pos < 192))
				*rel = 128;
			return 1;
		}
	}
void* Joystick::readJoystickThread(void *p) {
	Joystick *j = (Joystick*) p;
	cout << "starting joystick thread..." << endl;
	while (j->work) {
		if (j->isActive()){
			j->readJoystickPos();
		}else
			sleep(1);
	}
	return NULL;
}

	void* Joystick::readJoystickThreadOld(void* p) {
		Joystick *j = (Joystick *)p;
		cout<<"starting thread..."<<endl;
		j->findJoystick(30);
		if (j->joystick_dev.empty()) return NULL;
		cout<<"waiting 1 sec. before start reading.."<<endl;
		sleep(1);
		j->readJoystickPosOld();
		return NULL;
	}

	void *Joystick::joystickThread() {
		pthread_t tid;            // thread ID
		pthread_create(&tid, NULL, Joystick::readJoystickThread, this);
		sleep(1);
		return NULL;
	}

string Joystick::genRemoteMsg(int lr, int fr, int d, int zero_lr, int zero_d) {
	MsgRemote msg;

	if (d != 1)
		msg.setJoyElevator(d);
	if (lr != 1)
		msg.setJoyRudder(lr);
	if (fr != 1)
		msg.setJoyThrust(fr);
	if (zero_lr == 1)
		msg.setJoyRudder(1);
	if (zero_d == 1)
		msg.setJoyElevator(1);
	msg.setWho(who);

	return msg.get_spec();

}
