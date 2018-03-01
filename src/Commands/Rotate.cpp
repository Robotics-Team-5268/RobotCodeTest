
#include "Commands/Rotate.h"

Rotate::Rotate(): CommandBase(), degrees(), gyroAngle(), pid() {

}
Rotate::Rotate(float amount): CommandBase(), gyroAngle(0) {
	Command::Requires(drive.get());
	pid = nullptr;
	degrees = amount;
}

// Called just before this Command runs the first time
void Rotate::Initialize() {

}

// Called repeatedly when this Command is scheduled to run
void Rotate::Execute() {

	if(pid){
		SmartDashboard::PutNumber("P", pid->GetP());
		SmartDashboard::PutNumber("I", pid->GetI());
		SmartDashboard::PutNumber("D", pid->GetD());
		SmartDashboard::PutNumber("F", pid->GetF());
	}else{
		pid = new PIDController(
							SmartDashboard::GetNumber("P", .03),
							SmartDashboard::GetNumber("I", .005),
							SmartDashboard::GetNumber("D", .01),
							SmartDashboard::GetNumber("F", 0),
							drive->getGyro(),
							new RotatePIDOutput());
		pid->SetInputRange(-180, 180);
		pid->SetOutputRange(-.5, .5);
		pid->SetAbsoluteTolerance(3);
		pid->Enable();
		pid->SetSetpoint(degrees);
	}
}

// Make this return true when this Command no longer needs to run execute()
bool Rotate::IsFinished() {
	return pid->OnTarget();
}

// Called once after isFinished returns true
void Rotate::End() {
	pid->Disable();
	delete pid;
	pid = nullptr;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Rotate::Interrupted() {
	End();
}
void RotatePIDOutput::PIDWrite(double a)
{
	CommandBase::drive->setMotors(a,-a);
}
RotatePIDOutput::~RotatePIDOutput(){}
