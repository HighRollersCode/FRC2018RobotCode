/*
 * Arm.cpp
 *
 *  Created on: Jan 13, 2018
 *      Author: 987
 */

#include <Arm.h>
#include "MyRobot.h"

ArmClass::ArmClass()
{
	Arm_P = 0;
	Arm_I = 0;
	Arm_D = 0;

	Wrist_P = 0;
	Wrist_I = 0;
	Wrist_D = 0;

	hold_Arm = 0;

	hold_Wrist = 0;

	armCurrent = 0;
	wristCurrent = 0;

	ArmTalon = new WPI_TalonSRX(Arm_Motor);

	ArmTalon->ConfigContinuousCurrentLimit(20.0, 1.0f);

	ArmTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,1.0f);
	ArmTalon->SetSelectedSensorPosition(0,0,1.0f);
	//ArmTalon->SetSensorPhase(true);

	ArmTalon->EnableCurrentLimit(true);
	ArmTalon->ConfigForwardSoftLimitThreshold(ArmMaxLimEncoder,1.0f);
	ArmTalon->ConfigReverseSoftLimitThreshold(ArmMinLimEncoder,1.0f);

	Preferences *prefs = Preferences::GetInstance();

	ArmTalon->Config_kP(0,prefs->GetDouble("Arm_P",0.005f), 1.0);
	ArmTalon->Config_kI(0,prefs->GetDouble("Arm_I",0.0f), 1.0);
	ArmTalon->Config_kD(0,prefs->GetDouble("Arm_D",0.0f), 1.0);

	WristTalon = new TalonSRX(Wrist_Motor);
	WristTalon->SetInverted(true);

	WristTalon->ConfigContinuousCurrentLimit(20.0, 1.0);

	WristTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,1.0f);
	WristTalon->SetSelectedSensorPosition(0,0,1.0f);

	WristTalon->SetSensorPhase(true);

	WristTalon->ConfigNominalOutputForward(0,1.0);
	WristTalon->ConfigNominalOutputReverse(0,1.0);
	WristTalon->EnableCurrentLimit(false);
	WristTalon->ConfigForwardSoftLimitThreshold(WristMaxLimEncoder,1.0);
	WristTalon->ConfigReverseSoftLimitThreshold(WristMinLimEncoder,1.0);

	WristTalon->Config_kP(0,prefs->GetDouble("Wrist_P",0.0001f),1.0);
	WristTalon->Config_kI(0,prefs->GetDouble("Wrist_I",0.0f),1.0);
	WristTalon->Config_kD(0,prefs->GetDouble("Wrist_D",0.0f),1.0);
}

ArmClass::~ArmClass(){}

float ArmClass::GetArmEncoder()
{
	return ArmTalon->GetSelectedSensorPosition(0);
}
float ArmClass::GetWristEncoder()
{
	return WristTalon->GetSelectedSensorPosition(0);
}
void ArmClass::SetArmTarg(float targ)
{
	ArmTalon->Set(ControlMode::Position, targ);
}
void ArmClass::SetWristTarg(float targ)
{
	WristTalon->Set(ControlMode::Position, targ);
}
bool ArmClass::WristOnTarg(float tolerance)
{
	if(fabs(GetWristEncoder() - WristTalon->GetClosedLoopTarget(0)) < tolerance)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool ArmClass::ArmOnTarg(float tolerance)
{
	if(fabs(GetArmEncoder() - ArmTalon->GetClosedLoopTarget(0)) < tolerance)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void ArmClass::Update(float arm_Command,float wrist_Up, float wrist_Down)
{
	Arm_Update(arm_Command);
	Wrist_Update(wrist_Up,wrist_Down);
}
void ArmClass::Arm_Update(float command)
{
	if(fabs(command) > .1f)
	{
		ArmTalon->Set(ControlMode::PercentOutput,command);

		hold_Arm = GetArmEncoder();
	}
	else
	{
		ArmTalon->Set(ControlMode::Position,GetArmEncoder());
	}
}
void ArmClass::Wrist_Update(float upcommand, float downcommand)
{

	if(upcommand)
	{
		WristTalon->Set(ControlMode::PercentOutput,.5);
		hold_Wrist = GetWristEncoder();
	}
	else if(downcommand)
	{
		WristTalon->Set(ControlMode::PercentOutput,-.5);
		hold_Wrist = GetWristEncoder();
	}
	else
	{
		WristTalon->Set(ControlMode::Position, hold_Wrist);
	}
}
void ArmClass::Send_Data()
{
	SmartDashboard::PutNumber("Arm Encoder", GetArmEncoder());
	SmartDashboard::PutNumber("Hold Arm", hold_Arm);
	SmartDashboard::PutNumber("Wrist Encoder", GetWristEncoder());
	SmartDashboard::PutNumber("Hold Wrist", hold_Wrist);
	SmartDashboard::PutNumber("Arm Setpoint",ArmTalon->GetClosedLoopTarget(0));
	SmartDashboard::PutNumber("Arm Current", armCurrent);
	SmartDashboard::PutNumber("Wrist Current", wristCurrent);
}
