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

using namespace frc;

class ClawClass {
public:

	float leftCurrent = 0.0f;
	float rightCurrent = 0.0f;

	bool isIntaking = false;

	ClawClass();
	virtual ~ClawClass();
	void Update(bool intake, bool switchouttake, bool outtake,bool slowout,bool istracking);
	void Auto_Update();
	void Sense_Current();
	void Send_Data();
	bool GotCube();

	void Claw_Intake(float value = 1.0f);
	void Claw_Outake(float value = -1.0f);
	void Claw_Off();

private:
	VictorSPX *Claw1;
	VictorSPX *Claw2;

};

#endif /* SRC_CLAW_H_ */
