#ifndef MOVEANDROTATE_H
#define MOVEANDROTATE_H

#include "../CommandBase.h"
#include "Commands/Subsystem.h"
#include "../Robot.h"

/*
 * This is the class we are going to use for moving based on vision. One of the PIDs is for moving
 * forward and backward, the other is for the amount of turning that we have to do over that period
 * of time to be in front of the target.
 */

class MoveAndRotate: public CommandBase {
private:
	float gyroAngle;
	float moveOutput;
	float rotateOutput;
public:
	MoveAndRotate(); // float tm, float spd, float amount

	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
	void Set(bool isMove, double a);
	PIDController* pidMove;
	PIDController* pidRotate;
};

class MovePIDInput: public PIDSource {
public:
	double PIDGet();
	virtual ~MovePIDInput();
};

class RotatePIDInput: public PIDSource {
public:
	double PIDGet();
	virtual ~RotatePIDInput();
};

class MoveAndRotatePIDOutput: public PIDOutput {
private:
	MoveAndRotate* moveAndRotate;
	bool isMove;
public:
	MoveAndRotatePIDOutput(MoveAndRotate* moveAndRotateIn, bool isMoveIn);
	virtual ~MoveAndRotatePIDOutput();
	void PIDWrite(double a);
};

#endif
