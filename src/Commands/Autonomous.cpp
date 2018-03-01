#include "AutonomousChooser.h"
#include "Autonomous.h"
#include "Commands/Move.h"
#include "Commands/Rotate.h"
#include "Robot.h"

Autonomous::Autonomous(int selection) : CommandGroup("Autonomous") {
	std::string gameData;
	gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
	switch(selection){
	case 1: //right
		AddSequential(new Move(3, .5));
		if(gameData[0] == 'L') // if left
		{
			AddSequential(new Rotate(-15));
		} else {
			AddSequential(new Rotate(15));
		}
		break;
	case 2: //center
		AddSequential(new Move(1, .5));
		if(gameData[0] == 'L')
		{
			AddSequential(new Rotate(-45));
		} else {
			AddSequential(new Rotate(45));
		}
		break;
	case 3: //left
		AddSequential(new Move(1, .5));
		if(gameData[0] == 'L')
		{
			AddSequential(new Rotate(-90));
		} else {
			AddSequential(new Rotate(90));
		}
		break;
	default:
		break;
	}
}
