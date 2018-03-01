#include "Commands/MoveAndRotate.h"

/*
 * There are many variables you may need to change here:
 * P, I, and D for both move and rotate are used to control the "smoothness" of the movement
 * Do some research on them, such as the Wikipedia article on PID Controllers
 *
 * The other thing you will likely need to tweak is the output range, or speed,
 * of both the move and rotate PIDs.
 *
 * These variables will vary from robot to robot.
 */

const double MOVE_GOAL_DISTANCE = 20; //inches
const double MOVE_TOLERANCE = 5; //inches
const double MOVE_MAX_SPEED = .4; //between 0 and 1
const double ROTATE_TOLERANCE = 5; //degrees
const double ROTATE_MAX_SPEED = .057; //between 0 and 1

// float tm, float spd, float amount
MoveAndRotate::MoveAndRotate(): CommandBase()
	, gyroAngle(0), moveOutput(0), rotateOutput(0) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Command::Requires(drive.get());
	pidMove = nullptr;
	pidRotate = nullptr;
}

// Called just before this Command runs the first time
void MoveAndRotate::Initialize() {
}

// Called repeatedly when this Command is scheduled to run
void MoveAndRotate::Execute() {
	// Move PID
	if (pidMove) {
			SmartDashboard::PutNumber("mP", pidMove->GetP());
			SmartDashboard::PutNumber("mI", pidMove->GetI());
			SmartDashboard::PutNumber("mD", pidMove->GetD());
			SmartDashboard::PutNumber("mF", pidMove->GetF());
	} else {
		pidMove = new PIDController(
							// Possible change: P is not constant, but increases with distance
							SmartDashboard::GetNumber("mP", .01), // was .033
							SmartDashboard::GetNumber("mI", .00),
							SmartDashboard::GetNumber("mD", .008), // was .006
							SmartDashboard::GetNumber("mF", 0),
							new MovePIDInput(),
							new MoveAndRotatePIDOutput(this, true));
		pidMove->SetInputRange(0, 1000);
		pidMove->SetOutputRange(-MOVE_MAX_SPEED, MOVE_MAX_SPEED); // Min/max value PID is allowed to output
		pidMove->SetAbsoluteTolerance(MOVE_TOLERANCE); // How far off the PID is ok with being in inches
		pidMove->Enable();
		pidMove->SetSetpoint(MOVE_GOAL_DISTANCE); // Goal, in this case inches from target (was 3 then 12)
	}

	// Rotate PID
	if (pidRotate) {
			SmartDashboard::PutNumber("rP", pidRotate->GetP());
			SmartDashboard::PutNumber("rI", pidRotate->GetI());
			SmartDashboard::PutNumber("rD", pidRotate->GetD());
			SmartDashboard::PutNumber("rF", pidRotate->GetF());
	} else {
		pidRotate = new PIDController(
							SmartDashboard::GetNumber("rP", .033), // was .03
							SmartDashboard::GetNumber("rI", .00), // was .005
							SmartDashboard::GetNumber("rD", .01), // was .01
							SmartDashboard::GetNumber("rF", 0),
							new RotatePIDInput(), // input
							new MoveAndRotatePIDOutput(this, false));
		pidRotate->SetInputRange(-180, 180);
		pidRotate->SetOutputRange(-ROTATE_MAX_SPEED, ROTATE_MAX_SPEED); // was -.75, .75
		pidRotate->SetAbsoluteTolerance(ROTATE_TOLERANCE);
		pidRotate->Enable();
		pidRotate->SetSetpoint(0);
	}
}

// Receives input from one of the PIDs and adjusts the robot's movement accordingly
void MoveAndRotate::Set(bool isMove, double a) {
	if (isMove) {
		// PID's max output is 10, but speed controller's max input is 1
		SmartDashboard::PutNumber("moveInput", a);
		moveOutput = -a; // Negative because motors need to go forward for distance to decrease
		SmartDashboard::PutNumber("moveOutput", moveOutput);
	}
	else {
		// For testing purposes, ignore rotate PID for now
		rotateOutput = a;
		SmartDashboard::PutNumber("rotateOutput", rotateOutput);
	}
	CommandBase::drive->setMotorsArcade(moveOutput, rotateOutput);
}

// Called once after isFinished returns true
void MoveAndRotate::End() {
	drive->setMotors(0, 0); //Left speed is first, right speed second
	pidMove->Disable();
	pidRotate->Disable();
	delete pidMove;
	delete pidRotate;
	pidMove = nullptr;
	pidRotate = nullptr;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void MoveAndRotate::Interrupted() {
	End();
}

bool MoveAndRotate::IsFinished() {
	return pidMove->OnTarget();
	// Later this also needs to see if rotate is happy probably
}




double MovePIDInput::PIDGet() {
	if (!CommandBase::sighting) {
		SmartDashboard::PutBoolean("Sighting available", false);
		return -1;
	}
	SmartDashboard::PutBoolean("Sighting available", true);
	// Todo: function of this value instead of being simply the distance
	// Needs to be more fine when closer
	double tmp = CommandBase::sighting->distanceFromTarget(); // input
	return tmp;
	/*
	 * if (sighting distance >= glide distance) return 1
	 * else return k*ln(sighting distance)
	 */
}
MovePIDInput::~MovePIDInput(){}





double RotatePIDInput::PIDGet() {
	if (!CommandBase::sighting) return 0;
	return CommandBase::sighting->findFacingAngle(); // input
}
RotatePIDInput::~RotatePIDInput(){}





MoveAndRotatePIDOutput::MoveAndRotatePIDOutput(MoveAndRotate* moveAndRotateIn, bool isMoveIn): PIDOutput() {
	moveAndRotate = moveAndRotateIn;
	isMove = isMoveIn; // True if Move PID, false if Rotate PID
}

void MoveAndRotatePIDOutput::PIDWrite(double a) {
	SmartDashboard::PutNumber("a", a);
	moveAndRotate->Set(isMove, a);
}

MoveAndRotatePIDOutput::~MoveAndRotatePIDOutput(){}
