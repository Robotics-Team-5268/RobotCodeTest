#ifndef SRC_SUBSYSTEMS_DRIVE_H_
#define SRC_SUBSYSTEMS_DRIVE_H_
#include <Commands/Subsystem.h>
#include <WPILib.h>
#include <Drive/DifferentialDrive.h>
#include <SpeedController.h>
#include <SpeedControllerGroup.h>
#include <Talon.h>
#include "RobotMap.h"
#include "Subsystems/ITG3200Gyro.h"

//this class controls all the systems for driving. it does not take input from the controller.

class Drive: public Subsystem  {
private:
	// It's desirable that everything possible is private except
	// for methods that implement subsystem capabilities

	// initializes speed controllers for Drive to use
	frc::Talon speedControllerFL{DRIVE_SPEED_CONTROLLER_FL_CHANNEL};
	frc::Talon speedControllerBL{DRIVE_SPEED_CONTROLLER_BL_CHANNEL};
	frc::SpeedControllerGroup leftSC{speedControllerFL, speedControllerBL};

	frc::Talon speedControllerFR{DRIVE_SPEED_CONTROLLER_FR_CHANNEL};
	frc::Talon speedControllerBR{DRIVE_SPEED_CONTROLLER_BR_CHANNEL};
	frc::SpeedControllerGroup rightSC{speedControllerFR, speedControllerBR};

	frc::DifferentialDrive diffDrive{leftSC, rightSC};
	float oldX, oldY, oldLeftSpeed, oldRightSpeed;
	const float MAX_CHANGE = .05;

	AnalogGyro gyro{GYRO_ANALOG_PORT};
	ITG3200_Gyro newGyro;

public:
	Drive();
	~Drive();
	void safetyOff(){diffDrive.SetSafetyEnabled(false);}
	//void safetyOff(){robotDrive4->SetSafetyEnabled(false);}
	void InitDefaultCommand();

	AnalogGyro* getGyro();
	float getGyroAngle();
	void resetGyro();

	void setMotorsArcade(float move, float rotate);
	void setMotors(float leftSpeed, float rightSpeed);
	void takeInput();
	//std::shared_ptr<RobotDrive> robotDrive4;
};

#endif /* SRC_SUBSYSTEMS_DRIVE_H_ */
