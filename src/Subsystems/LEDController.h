#ifndef LEDController_H
#define LEDController_H

#include "RobotMap.h"
#include "Commands/Subsystem.h"
#include "WPILib.h"

class LEDController: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities

	frc::Relay ledRelay{LED_CAMERA};
	bool led_state = false;
public:
	LEDController();
	void InitDefaultCommand();
	void TurnOn(int num);
	void TurnOff(int num);
	bool IsOn(int num);
};

#endif
