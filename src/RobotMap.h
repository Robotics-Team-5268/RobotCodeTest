/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <SpeedControllerGroup.h>
#include <SpeedController.h>
#include <Drive/DifferentialDrive.h>

#ifndef ROBOTMAP_H
#define ROBOTMAP_H
#include <WPILib.h>
#include <ctre/Phoenix.h> // we get this from the people who make the CAN talons


// this used to be in a file called RobotParameters. We put that information here for simplicity.
#define DRIVE_SPEED_CONTROLLER_FL_CHANNEL 0
#define DRIVE_SPEED_CONTROLLER_FR_CHANNEL 1
#define DRIVE_SPEED_CONTROLLER_BL_CHANNEL 2
#define DRIVE_SPEED_CONTROLLER_BR_CHANNEL 3

#define RAMP_SPEED_CONTROLLER_UPPER_LEFT_CHANNEL 4
#define RAMP_SPEED_CONTROLLER_UPPER_RIGHT_CHANNEL 5
#define RAMP_SPEED_CONTROLLER_MIDDLE_LEFT_CHANNEL 6
#define RAMP_SPEED_CONTROLLER_MIDDLE_RIGHT_CHANNEL 7
#define RAMP_SPEED_CONTROLLER_LOWER_LEFT_CHANNEL 8
#define RAMP_SPEED_CONTROLLER_LOWER_RIGHT_CHANNEL 9
#define RAMP_SPEED_CONTROLLER_INTAKE_LEFT_CHANNEL 10
#define RAMP_SPEED_CONTROLLER_INTAKE_RIGHT_CHANNEL 11

#define CLIMB_SPEED_CONTROLLER_EXTEND_CHANNEL 12
#define CLIMB_SPEED_CONTROLLER_RETRACT_CHANNEL 13


// SPI is the better gyro, ANALOG is worse, but used on test bot.
//#define GYRO_SPI_PORT 							SPI::kOnboardCS0_
#define GYRO_ANALOG_PORT 0

// LED relays
#define LED_CAMERA 0

#define SCFL_INVERTED false
#define SCBL_INVERTED false
#define SCFR_INVERTED false
#define SCBR_INVERTED false

#define RIGHT_TRIGGER 3
#define LEFT_TRIGGER 2

#define Y_ERROR_MARGIN 15 // PIXELS
#define X_ERROR_MARGIN 20
#define AREA_ERROR_MARGIN .15 // PERCENT ERROR

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */

#endif// ROBOTMAP_H
