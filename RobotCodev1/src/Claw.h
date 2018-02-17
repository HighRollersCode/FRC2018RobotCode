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

	ClawClass();
	virtual ~ClawClass();
	void Update(bool intake, bool switchouttake, bool outtake,bool slowout);
};

#endif /* SRC_CLAW_H_ */
