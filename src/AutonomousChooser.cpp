#include <AutonomousChooser.h>
#include "Subsystems/RevDigitBoard.h"
#include "SuspendableThread.h"
#include "RobotMap.h"
#include <string>

AutonomousChooser::AutonomousChooser()
	: currentSelection( 1 ) {
	SetRunDelay(std::chrono::milliseconds(50));
	Run();
}

AutonomousChooser::~AutonomousChooser() {}

void AutonomousChooser::OnRun() {
	static REVDigitBoard RDB;
	static bool OldA = false;
	static const int timeLimit = 60;
	static int timer = timeLimit;

	if(RDB.getButtonA() == true && OldA == false){
		if(timer <= timeLimit){
			currentSelection++;
		}
		timer = 0;
	}
	if( currentSelection == 4 ){
		currentSelection = 1;
	}
	OldA = RDB.getButtonA();
	if (timer <= timeLimit) {
		RDB.display(std::to_string(currentSelection));
	} else {
		RDB.display(RobotController::GetInputVoltage());
	}
	if (timer <= timeLimit) {
		timer++;
	}
}

int AutonomousChooser::AutonomousSelection() {
	return currentSelection;
}
