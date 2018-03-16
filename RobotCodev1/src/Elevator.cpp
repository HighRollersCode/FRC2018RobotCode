/*
 * Elevator.cpp
 *
 *  Created on: Jan 13, 2018
 *      Author: 987
 */

#include "Elevator.h"

ElevatorClass::ElevatorClass()
{
	ElevatorTalon = new WPI_TalonSRX(Elevator_Motor);
	ElevatorTalon->SetInverted(true);

	ElevatorTalon->Set(ControlMode::PercentOutput, 0.0);

	ElevatorTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,1.0f);
	ElevatorTalon->SetSensorPhase(true);

	ElevatorTalon->ConfigNominalOutputForward(0.0,1.0);
	ElevatorTalon->ConfigNominalOutputReverse(0.0,1.0);

	ElevatorTalon->SetSelectedSensorPosition(0,0,1.0f);

	ElevatorTalon->ConfigForwardSoftLimitThreshold(ElevatorMaxLimEncoder,1.0);
	ElevatorTalon->ConfigReverseSoftLimitThreshold(ElevatorMinLimEncoder,1.0);

	ElevatorTalon->ConfigForwardSoftLimitEnable(true,1.0);
	ElevatorTalon->ConfigReverseSoftLimitEnable(true,1.0);

	ElevatorTalon->ConfigForwardLimitSwitchSource(LimitSwitchSource::LimitSwitchSource_Deactivated,LimitSwitchNormal::LimitSwitchNormal_Disabled,1.0);
	ElevatorTalon->ConfigReverseLimitSwitchSource(LimitSwitchSource::LimitSwitchSource_Deactivated,LimitSwitchNormal::LimitSwitchNormal_Disabled,1.0);

	Preferences *prefs = Preferences::GetInstance();

	ElevatorTalon->Config_kP(0,prefs->GetDouble("Elevator_P_0", 1.0f),1.0);
	ElevatorTalon->Config_kI(0,prefs->GetDouble("Elevator_I_0", 0.0f),1.0);
	ElevatorTalon->Config_kD(0,prefs->GetDouble("Elevator_D_0", 0.0005f),1.0);
	ElevatorTalon->Config_kF(0,0,1.0);

	ElevatorTalon->Config_kP(1,prefs->GetDouble("Elevator_P_1", 1.0f),1.0);
	ElevatorTalon->Config_kI(1,prefs->GetDouble("Elevator_I_1", 0.0f),1.0);
	ElevatorTalon->Config_kD(1,prefs->GetDouble("Elevator_D_1", 0.0005f),1.0);
	ElevatorTalon->Config_kF(1,0,1.0);

	currentverticalcommand = 0;
	prevverticalcommand = 0;

	isreceivingelevatorinput = false;
}

ElevatorClass::~ElevatorClass() {
}

float ElevatorClass::GetElevatorEncoder()
{
	return ElevatorTalon->GetSelectedSensorPosition(0);
}
void ElevatorClass::ResetElevatorEncoder()
{
	ElevatorTalon->Set(ControlMode::PercentOutput,0);
	SetElevatorTarg(0);

	ElevatorTalon->SetSelectedSensorPosition(0,0,1.0f);
}
void ElevatorClass::SetElevatorTarg(float targ)
{
	if(targ > GetElevatorEncoder())
	{
		ElevatorTalon->SelectProfileSlot(0,0);
	}
	else if(targ < GetElevatorEncoder())
	{
		ElevatorTalon->SelectProfileSlot(1,0);
	}
	ElevatorTalon->Set(ControlMode::Position,targ);
}

bool ElevatorClass::ElevatorOnTarg(float tolerance)
{
	if(fabs(GetElevatorEncoder() - ElevatorTalon->GetClosedLoopTarget(0)) < tolerance)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

void ElevatorClass::PIDOff()
{
	ElevatorTalon->Set(ControlMode::PercentOutput, 0);
}
void ElevatorClass::Update(float verticalcommand)
{
	prevverticalcommand = currentverticalcommand;
	currentverticalcommand = verticalcommand;

	if(fabs(verticalcommand) > .1f)
	{
		ElevatorTalon->Set(ControlMode::PercentOutput, verticalcommand);
		isreceivingelevatorinput = true;
	}

	if((fabs(prevverticalcommand) > .1) && (fabs(currentverticalcommand) <= .1))//.2
	{
		SetElevatorTarg(GetElevatorEncoder());
		isreceivingelevatorinput = false;
	}
}

void ElevatorClass::Send_Data()
{
	SmartDashboard::PutNumber("Elevator Encoder", GetElevatorEncoder());
}
