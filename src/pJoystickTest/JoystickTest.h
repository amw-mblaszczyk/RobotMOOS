/************************************************************/
/*    NAME: MB                                              */
/*    ORGN: MIT                                             */
/*    FILE: JoystickTest.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef JoystickTest_HEADER
#define JoystickTest_HEADER

#include <string>
#include <fcntl.h>
#include <iostream>
#include <linux/input.h>
#include <linux/input-event-codes.h>

using namespace std;

#define EVDEV_DIR "/dev/input/by-id/"

class JoystickTest
{
 public:
   JoystickTest();
   ~JoystickTest();

	/**
	 * Finds and prints joystick device file.
	 * @param print: if prints device name on standard output (terminal). Default: true.
	 * @return name of joystick device.
	 */
	static string listDevices(bool print = true);

	/**
	 * Reapeadly reads joystick device and prints result.
	 */
	void readJoystick(long joystick_id);
	void closeJoystick(long joystickId);
	long connectToJoystick(string device, int sek);
	void read_joy_abs(struct input_event ev);
	void read_joy_key(struct input_event ev);
	static void* readCommand(void*);
	void* exitThread();



 protected:

protected:
	bool work = true;
	short x_stat = 128;
	short y_stat = 128;
	short rx_stat = 128;
	short ry_stat = 128;
	int gamepad = -1;
	pthread_t tid = 0;

 private:
	static const short LEFT = 100;
	static const short RIGHT = 200;
	static const short REVERT = 100;
	static const short FORWARD = 200;

	static const int OPEN_FLAGS = O_RDONLY;

	string joystick_dev = EVDEV_DIR;
	uint counter = 0;

 private: // State variables
};

#endif 
