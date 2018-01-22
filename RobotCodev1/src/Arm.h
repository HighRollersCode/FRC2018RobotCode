/*
 * Arm.h
 *
 *  Created on: Jan 13, 2018
 *      Author: 987
 */

#ifndef SRC_ARM_H_
#define SRC_ARM_H_

#include "WPILib.h"
#include "Defines.h"
#include "ctre/Phoenix.h"

class ArmClass
{
public:

	float P;
	float I;
	float D;

	float hold_Arm;

	WPI_VictorSPX *Arm;

	Encoder *ArmEncoder;

	PIDController *ArmPIDController;

	float GetArmEncoder();

	ArmClass();
	virtual ~ArmClass();
	void Update(float command);
	void Send_Data();
};
#endif /* SRC_ARM_H_ */
