#include "CommandBase.h"
#include "Commands/Scheduler.h"

// Initialize a single static instance of all of your subsystems to NULL
std::unique_ptr<OI> CommandBase::oi;
std::unique_ptr<Drive> CommandBase::drive;
std::unique_ptr<LEDController> CommandBase::leds;
std::unique_ptr<Sighting> CommandBase::sighting;
std::unique_ptr<UDPReceiver> CommandBase::udp;
std::unique_ptr<Encoders> CommandBase::rightEncoder;
std::unique_ptr<Encoders> CommandBase::leftEncoder;

CommandBase::CommandBase(const std::string &name) : Command(name) {
}

CommandBase::CommandBase() : Command() {
}

void CommandBase::init() {
	drive.reset(new Drive());
	leds.reset(new LEDController());
	udp.reset(new UDPReceiver());
	leftEncoder.reset(new Encoders("left",0, 1, false));
	rightEncoder.reset(new Encoders("right", 3, 2, false));


	// Keep at the end
	oi.reset(new OI());
}





