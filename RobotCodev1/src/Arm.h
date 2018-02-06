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
#include "CurrentLimitedJoint.h"

class ArmClass
{
public:

	float Arm_P;
	float Arm_I;
	float Arm_D;

	float Wrist_P;
	float Wrist_I;
	float Wrist_D;

	float hold_Arm;
	float hold_Wrist;

	//WPI_VictorSPX *Arm;
	WPI_TalonSRX *ArmTalon;
	//CurrentLimitedJoint *ArmJoint;

	//WPI_VictorSPX *Wrist;
	TalonSRX *WristTalon;

	//CurrentLimitedJoint *WristJoint;

	//Encoder *ArmEncoder;
	//Encoder *WristEncoder;

	//PIDController *WristPIDController;
	//PIDController *ArmPIDController;

	float armCurrent;
	float wristCurrent;

	float GetArmEncoder();
	float GetWristEncoder();

	ArmClass();
	virtual ~ArmClass();
	void Update(float arm_Command,float wrist_Up,float wrist_Down);
	void Arm_Update(float command);
	void Wrist_Update(float upcommand, float downcommand);
	void Send_Data();
	void SetWristTarg(float targ);
	void SetArmTarg(float targ);
	bool WristOnTarg(float tolerance = 5);
	bool ArmOnTarg(float tolerance = 5);
};
#endif /* SRC_ARM_H_ */
