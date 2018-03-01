#include "Sighting.h"
#include "../RobotMap.h"
#include "../CommandBase.h"
#include <math.h>

const double OFFSET_TO_FRONT = 9.0; // Theortically distance from center of robot to camera
const double DISTANCE_CONSTANT = 5561.0; // Magic number
const double MAGIC_NUMBER = 1.37; // Makes distance accurate, because theoretical method is not perfect. Based on tape measure values.
const double IN_BETWEEN_CONTOURS = 8.5; // Distance between middle of contours in inches

// Camera's resolution
//const int yres = 480; // y resolution of Microsoft Life Camera - commented out because currently unused
const int xres = 640; // x resolution of Microsoft Life Camera

// Camera's field of view in radians
//const double vertFOV = 0.36397895; //20.85 degrees
const double horiFOV = 0.58800142; // 33.69 degrees
const double tanHoriFOV = 0.666665;


Sighting::Sighting() : Subsystem("Sighting") {
}

void Sighting::InitDefaultCommand() {
	// Set the default command for a subsystem here.
}

/*
 * Handles split contours and contours that
 * are unreasonably different in size (in theory, not in practice)
 */
void Sighting::cleanContours() {
	//if (area.size() < 1) return -1; // No contours

	// This for loop checks
	// if there is a set of broken contours from the peg
	// by seeing if their X values are close to each other
	// Commented out because it doesn't do anything because
	// output is never used. It should merge the contours.

	/*
	int topCutContour = 0;
	int bottomCutContour = 0;

	for (uint i = 0; i < area.size(); i++) { // area.size() gives the number of contours
		for (uint j = i + 1; j < area.size(); j++) {
			// Check if within margin of error (meaning the contour are too close together)
			// This should mean the spring is splitting a contour into two on top of each other
			if (fabs(centerX[j] - centerX[i]) < X_ERROR_MARGIN) {
				if (centerY[j] > centerY[i]) { // Find the top contour
					topCutContour = j;
					bottomCutContour = i;
				} else {
					bottomCutContour = j;
					topCutContour = i;
				}
				i = area.size(); // jump to end of loop
				j = area.size();
			}
		}
	}*/


	// This for loop runs through all of the contours and ensures that they
	// are at similar heights and that they are similar in area.

	pixelWidth1 = 0;
	pixelWidth2 = 0;

	for (uint i = 0; i < area.size(); i++) { // area.size() gives the number of contours
		for (uint j = i + 1; j < area.size(); j++) {
			if (fabs(centerY[j] - centerY[i]) < Y_ERROR_MARGIN && //Check if within margin of error
					(area[j] - area[i]) / area[j] < AREA_ERROR_MARGIN) {
				if (centerX[j] > centerX[i]) { // Find the left contour
					pixelWidth1 = width[i];
					pixelWidth2 = width[j];
				} else {
					pixelWidth2 = width[i];
					pixelWidth1 = width[j];
				}
				frc::SmartDashboard::PutNumber("Countours Fit", 1);
				i = area.size(); // jump to end of loop
				j = area.size();
			}
		}
	}

	frc::SmartDashboard::PutNumber("Pixel Width 1", pixelWidth1);
	frc::SmartDashboard::PutNumber("Pixel Width 2", pixelWidth2);
}

double Sighting::findFacingAngle() {
	cleanContours();

	if (centerX.size() >= 1)
		frc::SmartDashboard::PutNumber("First centerX", centerX[0]);

	if (TwoContoursAvailable()) {
		// KHyp is 8.5in (the distance from center to center of contours)
		// Divide by the length between centers in pixels to get inches per pixel
		const double inPerPx = IN_BETWEEN_CONTOURS / abs(centerX[0] - centerX[1]);
		// this calculates the distance from the center of goal to center of webcam
		double distanceFromCenterPixels = ((centerX[0] + centerX[1]) / 2) - (xres / 2);
		// Converts pixels to inches using the constant from above.
		double distanceFromCenterInch = distanceFromCenterPixels * inPerPx;
		// math brought to you buy Chris and Jones
		double angleToGoal = atan(distanceFromCenterInch / distanceFromTarget());
		angleToGoal = (angleToGoal / 3.14159265) * 180;
		frc::SmartDashboard::PutNumber("Angle To Goal", angleToGoal);
		return angleToGoal;
	}
	frc::SmartDashboard::PutNumber("Angle To Goal", -1);
	return -1;
}

double Sighting::distanceFromTarget(){
	/*// distance costant divided by length between centers of contours
	double lengthBetweenContours = (centerX[0] - centerX[1]) / 2;
	return (DISTANCE_CONSTANT / lengthBetweenContours) - OFFSET_TO_FRONT;*/

	// Written to assume it's only getting two valid contours
	if (TwoContoursAvailable()) {
		double pxBetweenContours = abs(centerX[0] - centerX[1]);
		double distanceToTarget = IN_BETWEEN_CONTOURS * xres / (2 * pxBetweenContours * tanHoriFOV) * MAGIC_NUMBER;
		frc::SmartDashboard::PutNumber("Distance to target", distanceToTarget);

		/*double xDistance = distanceToTarget * sin((CommandBase::drive->getGyroAngle()*3.14159/180.0));
		frc::SmartDashboard::PutNumber("X Distance", xDistance);*/
		if (distanceToTarget > 200) {
			frc::SmartDashboard::PutNumber("Distance to target", -1);
			return -1;
		}
		return distanceToTarget;
	}
	frc::SmartDashboard::PutNumber("Distance to target", -1);
	return -1;
}

//Update contour vectors
void Sighting::readTable() {
	CommandBase::udp->GetContours(centerX, centerY, width, height, area);

	if (TwoContoursAvailable()) {
		frc::SmartDashboard::PutNumber("distance between contours", centerX[0] - centerX[1]);
	}
}

bool Sighting::TwoContoursAvailable() {
	return area.size() == 2; // returns true if there are 2 contours
}
// Get the center of the whole target by averaging the contours together
double Sighting::getCenterX() {
	if (TwoContoursAvailable())
		return (centerX[0] + centerX[1]) / 2.0;
	else return -1.0;
}
