/*
 * Claw.h
 *
 *  Created on: Jan 14, 2018
 *      Author: 987
 */

#ifndef SRC_CLAW_H_
#define SRC_CLAW_H_

#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "Defines.h"

class ClawClass {
public:
	VictorSPX *Claw1;
	VictorSPX *Claw2;
	//Solenoid *ClawIntake;

	ClawClass();
	virtual ~ClawClass();
	void Update (bool intake, bool outtake);
};

#endif /* SRC_CLAW_H_ */
