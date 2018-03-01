#include "Commands/Move.h"

Move::Move(float tm, float spd): CommandBase() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Command::Requires(drive.get());
	pid = nullptr;
	seconds = tm;
	speed = spd;
	timer.reset(new Timer());
}

// Called just before this Command runs the first time
void Move::Initialize() {
}

// Called repeatedly when this Command is scheduled to run
void Move::Execute() {
	if(pid){
		SmartDashboard::PutNumber("Pmove", pid->GetP());
		SmartDashboard::PutNumber("Imove", pid->GetI());
		SmartDashboard::PutNumber("Dmove", pid->GetD());
		SmartDashboard::PutNumber("Fmove", pid->GetF());
	}else{
		pid = new PIDController(
							SmartDashboard::GetNumber("Pmove", .03),
							SmartDashboard::GetNumber("Imove", .005),
							SmartDashboard::GetNumber("Dmove", .01),
							SmartDashboard::GetNumber("Fmove", 0),
							drive->getGyro(),
							new MovePIDOutput(speed));
		pid->SetInputRange(-180, 180);
		pid->SetOutputRange(-.1, .1);
		pid->SetAbsoluteTolerance(3);
		pid->Enable();
		pid->SetSetpoint(drive->getGyroAngle());
		timer->Reset();
		timer->Start();
	}
}

// Make this return true when this Command no longer needs to run execute()
bool Move::IsFinished() {
	return timer->HasPeriodPassed(seconds);
}

// Called once after isFinished returns true
void Move::End() {
	drive->setMotors(0, 0); //Left speed is first, right speed second
	timer->Stop();
	pid->Disable();
	delete pid;
	pid = nullptr;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Move::Interrupted() {
	End();
}

MovePIDOutput::MovePIDOutput(double sp) {
	speed = sp;
}
void MovePIDOutput::PIDWrite(double a) {
	CommandBase::drive->setMotors(speed + a, speed - a);
}
MovePIDOutput::~MovePIDOutput(){}
