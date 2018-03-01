#pragma once
#ifndef OI_H
#define OI_H

#include "WPILib.h"

// this class defines the stuff we use to control the robot. OI stands for operator input.
class OI {
private:
	std::shared_ptr<Joystick> driverJoystick;
	//std::shared_ptr<Joystick> driverJoystick2;
	std::vector<JoystickButton*> driverBtns;
	//std::vector<JoystickButton*> driverBtns2;
public:
	OI();

	std::shared_ptr<Joystick> getDriverJoystick();
	//std::shared_ptr<Joystick> getDriverJoystick2();
	bool getDriverButtonPressed(int btnNum);
	//bool getDriverButtonPressed2(int btnNum);
};
#endif
