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

	ArmTalon = new WPI_TalonSRX(Arm_Motor);

	ArmTalon->ConfigContinuousCurrentLimit(20.0, 1.0f);

	ArmTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,1.0f);
	ArmTalon->SetSelectedSensorPosition(0,0,1.0f);
	ArmTalon->SetSensorPhase(true);

	ArmTalon->EnableCurrentLimit(false);
	ArmTalon->ConfigForwardSoftLimitThreshold(ArmMaxLimEncoder,1.0f);
	ArmTalon->ConfigReverseSoftLimitThreshold(ArmMinLimEncoder,1.0f);
	ArmTalon->ConfigForwardSoftLimitEnable(true,1.0);
	ArmTalon->ConfigReverseSoftLimitEnable(true,1.0);
	ArmTalon->ConfigPeakOutputForward(1.0,1.0);
	ArmTalon->ConfigPeakOutputReverse(-1.0,1.0);

	Preferences *prefs = Preferences::GetInstance();

	ArmTalon->Config_kP(0,prefs->GetDouble("Arm_P",0.8f), 1.0);
	ArmTalon->Config_kI(0,prefs->GetDouble("Arm_I",0.0f), 1.0);
	ArmTalon->Config_kD(0,prefs->GetDouble("Arm_D",0.0f), 1.0);

	ArmTalon->Config_kP(1,0.05f, 1.0);
	ArmTalon->Config_kI(1,0.0f, 1.0);
	ArmTalon->Config_kD(1,0.0f, 1.0);

	prevArmCommand = 0;
	curArmCommand = 0;

	WristTalon = new TalonSRX(Wrist_Motor);
	WristTalon->SetInverted(true);

	WristTalon->ConfigContinuousCurrentLimit(20.0, 1.0);

	WristTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,1.0f);
	WristTalon->SetSelectedSensorPosition(0,0,1.0f);

	WristTalon->SetSensorPhase(true);

	WristTalon->ConfigNominalOutputForward(0,1.0);
	WristTalon->ConfigNominalOutputReverse(0,1.0);
	WristTalon->ConfigPeakOutputForward(1.0,1.0);
	WristTalon->ConfigPeakOutputReverse(-1.0,1.0);
	WristTalon->EnableCurrentLimit(false);

	WristTalon->ConfigForwardSoftLimitThreshold(WristMaxLimEncoder,1.0);
	WristTalon->ConfigReverseSoftLimitThreshold(WristMinLimEncoder,1.0);

	WristTalon->ConfigForwardSoftLimitEnable(false,1.0);
	WristTalon->ConfigReverseSoftLimitEnable(false,1.0);

	WristTalon->ConfigForwardLimitSwitchSource(LimitSwitchSource::LimitSwitchSource_Deactivated,LimitSwitchNormal::LimitSwitchNormal_Disabled,1.0);
	WristTalon->ConfigReverseLimitSwitchSource(LimitSwitchSource::LimitSwitchSource_Deactivated,LimitSwitchNormal::LimitSwitchNormal_Disabled,1.0);

	WristTalon->Config_kP(0,prefs->GetDouble("Wrist_P",0.3f),1.0); //0.9
	WristTalon->Config_kI(0,prefs->GetDouble("Wrist_I",0.0f),1.0);
	WristTalon->Config_kD(0,prefs->GetDouble("Wrist_D",0.0f),1.0);
	WristTalon->Config_kF(0,0,1.0);

	LimitSwitch = new DigitalInput(10);

	prevWristCommand = 0;
	curWristCommand = 0;

	isreceivingarminput = false;
	isreceivingwristinput = false;
}

ArmClass::~ArmClass(){}

void ArmClass::PIDOff()
{
	ArmTalon->Set(ControlMode::PercentOutput,0);
	WristTalon->Set(ControlMode::PercentOutput,0);
}

bool ArmClass::GetLimtiSwitch()
{
	return !LimitSwitch->Get();
}
float ArmClass::GetArmEncoder()
{
	return ArmTalon->GetSelectedSensorPosition(0);
}

float ArmClass::GetWristEncoder()
{
	return WristTalon->GetSelectedSensorPosition(0);
}

void ArmClass::ResetEncoder()
{
	hold_Wrist = 0;
	hold_Arm = 0;

	ArmTalon->Set(ControlMode::PercentOutput,0);
	WristTalon->Set(ControlMode::PercentOutput,0);

	ArmTalon->SetSelectedSensorPosition(0,0,1.0f);
	WristTalon->SetSelectedSensorPosition(0,0,1.0f);
}

void ArmClass::SetArmTarg(float targ)
{
	if(targ < GetArmEncoder())
	{
		ArmTalon->SelectProfileSlot(1,0);
	}
	else
	{
		ArmTalon->SelectProfileSlot(0,0);
	}
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

float ArmClass::GetWristTarg()
{
	return WristTalon->GetClosedLoopTarget(0);
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

void ArmClass::Update(float arm_Command,float wrist_Up, float wrist_Down,float wrist_Reset)
{
	Arm_Update(arm_Command);
	Wrist_Update(wrist_Up,wrist_Down,wrist_Reset);
}

void ArmClass::Arm_Update(float command)
{
	prevArmCommand = curArmCommand;
	curArmCommand = command;

	if(fabs(command) > .3f)
	{
		ArmTalon->Set(ControlMode::PercentOutput,command);
		isreceivingarminput = true;
		hold_Arm = GetArmEncoder();
	}
	if(fabs(curArmCommand) < .3f && fabs(prevArmCommand) >= .3f)
	{
		ArmTalon->Set(ControlMode::Position,hold_Arm);
		isreceivingarminput = false;
	}
}

void ArmClass::Wrist_Update(float upcommand, float downcommand,float resetcommand)
{
	float wristCommand = 0;

	if(upcommand)
	{
		wristCommand = .75;
	}
	else if(downcommand)
	{
		wristCommand = -.75;
	}
	else if(resetcommand)
	{
		wristCommand = .25;
	}
	else
	{
		wristCommand = 0;
	}

	prevWristCommand = curWristCommand;
	curWristCommand = wristCommand;


	if(fabs(curWristCommand) > .1f)
	{
		if(GetLimtiSwitch())
		{
			ResetEncoder();
		}
		WristTalon->Set(ControlMode::PercentOutput,wristCommand);
		hold_Wrist = GetWristEncoder();
		isreceivingwristinput = true;
	}

	if(fabs(curWristCommand) < .1f && fabs(prevWristCommand) >= .1f)
	{
		WristTalon->Set(ControlMode::Position, hold_Wrist);
		isreceivingwristinput = false;
	}
}

void ArmClass::Send_Data()
{
	SmartDashboard::PutNumber("Arm Encoder", GetArmEncoder());
	SmartDashboard::PutNumber("Hold Arm", hold_Arm);
	SmartDashboard::PutNumber("Wrist Encoder", GetWristEncoder());
	SmartDashboard::PutNumber("Hold Wrist", hold_Wrist);
	SmartDashboard::PutNumber("Wrist Control Mode", (int)WristTalon->GetControlMode());
	SmartDashboard::PutNumber("Wrist Error", WristTalon->GetClosedLoopError(0));
	SmartDashboard::PutNumber("Arm Setpoint", ArmTalon->GetClosedLoopTarget(0));
	SmartDashboard::PutBoolean("Wrist Proximity Switch", GetLimtiSwitch());
}
