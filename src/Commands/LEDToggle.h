#ifndef LED_H
#define LED_H

#include "../CommandBase.h"
#include "WPILib.h"
#include "Robot.h"

class LEDToggle: public CommandBase
{
public:
	LEDToggle();
	LEDToggle(int x);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
	int num;
};

#endif
