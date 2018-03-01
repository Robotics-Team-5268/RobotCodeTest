/*
 * AutonomousChooser.h
 *
 *  Created on: Jan 27, 2018
 *      Author: osfalcons
 */

#ifndef SRC_AUTONOMOUSCHOOSER_H_
#define SRC_AUTONOMOUSCHOOSER_H_

#include "SuspendableThread.h"
#include <atomic>   // For atomic variables.
#include <chrono>   // For millisecond duration type.
#include <cstdint>  // For standard type definitions.
#include <iomanip>  // For hex, setw, and setfill.
#include <thread>   // For thread and sleep_for.

class AutonomousChooser: public SuspendableThread {
public:
    AutonomousChooser();
    virtual ~AutonomousChooser();
    int AutonomousSelection();
private:
    int currentSelection;
protected:
    void OnRun();
};

#endif /* SRC_AUTONOMOUSCHOOSER_H_ */
