#include <Commands/LEDToggle.h>

LEDToggle::LEDToggle(): num(1) {
	Requires(leds.get());
}
LEDToggle::LEDToggle(int x): num(x) {
	Requires(leds.get());
}

// Called just before this Command runs the first time
void LEDToggle::Initialize() {

}

// Called repeatedly when this Command is scheduled to run
void LEDToggle::Execute() {
	leds->IsOn(num) ? leds->TurnOff(num) : leds->TurnOn(num);
}

// Make this return true when this Command no longer needs to run execute()
bool LEDToggle::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void LEDToggle::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void LEDToggle::Interrupted() {

}
