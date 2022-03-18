/************************************************************/
/*    NAME: MB                                              */
/*    ORGN: MIT                                             */
/*    FILE: main.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <string>
#include "JoystickTest.h"

using namespace std;

int main(int argc, char *argv[])
{
	cout<<"Start Joystick test."<<endl;
	JoystickTest jtest;
	string joy_device = jtest.listDevices(true);
	if ( joy_device.empty() ){
		cerr<<"Joystick device not found, exiting ..."<<endl;
		return (0);
	}
	cout<<"Devices:"<<endl<<joy_device<<endl;
	cout<<"Connecting to joystick...";
	long joy_id = jtest.connectToJoystick(joy_device, 10);
	if ( joy_id <= 0 ){
		cerr<<"Can't connect to joystick, exiting ..."<<endl;
		return (0);
	}
	cout<<"Connected."<<endl;
	cout<<"Reading joystick ... press any button or move the stick."<<endl<<endl;;

	jtest.exitThread();
	cout<<"Press 4 times button A for exit."<<endl<<endl;
	jtest.readJoystick(joy_id);

	cout<<"Disconneting joystick...";
	jtest.closeJoystick(joy_id);
	cout<<"done."<<endl;
  
  return(0);
}

