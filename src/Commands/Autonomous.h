
#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_

#include "Commands/CommandGroup.h"
#include "../CommandBase.h"

class Autonomous: public CommandGroup {
public:
	Autonomous(int selection);
};



#endif /* SRC_AUTONOMOUS_H_ */
