#include "LEDController.h"
#include "../RobotMap.h"

LEDController::LEDController() : Subsystem("LEDController") {}

void LEDController::InitDefaultCommand() {}

// Put methods for controlling this subsystem
// here. Call these from Commands.
void LEDController::TurnOff(int num) {
	led_state = false;
	ledRelay.Set(frc::Relay::kOff);
}

void LEDController::TurnOn(int num) {
	led_state = true;
	ledRelay.Set(frc::Relay::kForward);
	// DO NOT EVER SET RELAY TO REVERSE
	// UNLESS THE WIRING GETS CHANGED.
	// This would fry the LED.
}

bool LEDController::IsOn(int num) {
	return led_state;
}
