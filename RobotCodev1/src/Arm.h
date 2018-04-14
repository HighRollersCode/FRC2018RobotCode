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
#include "DigitalInput.h"

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

	float prevArmCommand;
	float curArmCommand;

	TalonSRX *WristTalon;

	DigitalInput *LimitSwitch;

	float prevWristCommand;
	float curWristCommand;

	bool isreceivingarminput;
	bool isreceivingwristinput;

	float GetArmEncoder();
	float GetWristEncoder();
	void ResetEncoder();

	ArmClass();
	virtual ~ArmClass();

	void PIDOff();
	void Update(float arm_Command,float wrist_Up,float wrist_Down,float wrist_Reset);
	void Arm_Update(float command);
	void Wrist_Update(float upcommand, float downcommand,float resetcommand);
	void Send_Data();
	void SetWristTarg(float targ);
	void SetArmTarg(float targ);
	float GetWristTarg();
	bool WristOnTarg(float tolerance = 5);
	bool ArmOnTarg(float tolerance = 5);

	bool GetLimtiSwitch();
};
#endif /* SRC_ARM_H_ */
