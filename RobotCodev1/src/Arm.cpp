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

	//Arm = new WPI_VictorSPX(Arm_Motor);
	//Arm->SetInverted(false);

	ArmTalon = new WPI_TalonSRX(Arm_Motor);

	ArmTalon->ConfigContinuousCurrentLimit(20.0, 1.0f);
	ArmTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,1.0f);
	ArmTalon->SetSelectedSensorPosition(0,0,1.0f);
	ArmTalon->SetSensorPhase(true);
	ArmTalon->EnableCurrentLimit(true);
	ArmTalon->ConfigForwardSoftLimitThreshold(ArmMaxLimEncoder,1.0f);
	ArmTalon->ConfigReverseSoftLimitThreshold(ArmMinLimEncoder,1.0f);

	ArmTalon->Config_kP(0,0,1.0);
	ArmTalon->Config_kI(0,0,1.0);
	ArmTalon->Config_kD(0,0,1.0);

	/*ArmJoint = new CurrentLimitedJoint(Arm, Arm_PDPChannel, 20.0, 1.0);

	ArmEncoder = new Encoder(Arm_Encoder_1,Arm_Encoder_2,false,Encoder::EncodingType::k4X);
	ArmEncoder->Reset();

	ArmPIDController = new PIDController(Arm_P,Arm_I,Arm_D,ArmEncoder,ArmJoint,.01f);
	ArmPIDController->SetInputRange(ArmMinLimEncoder, ArmMaxLimEncoder);
	ArmPIDController->SetOutputRange(-.5,.5);
	ArmPIDController->SetContinuous(false);
	ArmPIDController->Disable();

	Wrist = new WPI_VictorSPX(Wrist_Motor);
*/
	WristTalon = new TalonSRX(Wrist_Motor);
	WristTalon->SetInverted(true);

	WristTalon->ConfigContinuousCurrentLimit(20.0, 1.0);
	WristTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,1.0f);
	WristTalon->SetSelectedSensorPosition(0,0,1.0f);
	WristTalon->SetSensorPhase(true);
	WristTalon->ConfigNominalOutputForward(0,1.0);
	WristTalon->ConfigNominalOutputReverse(0,1.0);
	WristTalon->EnableCurrentLimit(true);
	WristTalon->ConfigForwardSoftLimitThreshold(WristMaxLimEncoder,1.0);
	WristTalon->ConfigReverseSoftLimitThreshold(WristMinLimEncoder,1.0);

	Preferences *prefs = Preferences::GetInstance();

	WristTalon->Config_kP(0,prefs->GetDouble("Wrist_P",-0.001f),1.0);
	WristTalon->Config_kI(0,prefs->GetDouble("Wrist_I",0.0f),1.0);
	WristTalon->Config_kD(0,prefs->GetDouble("Wrist_D",0.0f),1.0);

	/*WristJoint = new CurrentLimitedJoint(Wrist,Wrist_PDPChannel,5.0,1.0);

	WristEncoder = new Encoder(Wrist_Encoder_1,Wrist_Encoder_2,false,Encoder::EncodingType::k4X);
	WristEncoder->Reset();

	WristPIDController = new PIDController(Wrist_P,Wrist_I,Wrist_D,WristEncoder,Wrist,.01f);
	WristPIDController->SetInputRange(WristMinLimEncoder,WristMaxLimEncoder);
	WristPIDController->SetContinuous(false);
	WristPIDController->Disable();*/
}

ArmClass::~ArmClass(){}

float ArmClass::GetArmEncoder()
{
	//return ArmEncoder->Get();
	return ArmTalon->GetSelectedSensorPosition(0);
}
float ArmClass::GetWristEncoder()
{
	return WristTalon->GetSelectedSensorPosition(0);
	//return WristEncoder->Get();
}
void ArmClass::SetArmTarg(float targ)
{
	ArmTalon->Set(ControlMode::Position, targ);

	//ArmPIDController->Enable();
	//ArmPIDController->SetSetpoint(targ);
}
void ArmClass::SetWristTarg(float targ)
{
	WristTalon->Set(ControlMode::Position, targ);

	//WristPIDController->Enable();
	//WristPIDController->SetSetpoint(targ);
}
bool ArmClass::WristOnTarg(float tolerance)
{
	if(fabs(GetWristEncoder() - WristTalon->GetClosedLoopTarget(0)/*WristPIDController->GetSetpoint()*/) < tolerance)
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
	if(fabs(GetArmEncoder() - ArmTalon->GetClosedLoopTarget(0)) /*ArmPIDController->GetSetpoint())*/ < tolerance)
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
	//ArmJoint->Update();
	//WristJoint->Update();

	armCurrent = MyRobotClass::Get()->PDP->GetCurrent(Arm_PDPChannel);
	wristCurrent = MyRobotClass::Get()->PDP->GetCurrent(Wrist_PDPChannel);

	Arm_Update(arm_Command);
	Wrist_Update(wrist_Up,wrist_Down);
}
void ArmClass::Arm_Update(float command)
{
	Preferences *prefs = Preferences::GetInstance();

	ArmTalon->Config_kP(0,prefs->GetDouble("Arm_P",0.005f), 1.0);
	ArmTalon->Config_kI(0,prefs->GetDouble("Arm_I",0.0f), 1.0);
	ArmTalon->Config_kD(0,prefs->GetDouble("Arm_D",0.0f), 1.0);

	//ArmPIDController->SetP(prefs->GetDouble("Arm_P",0.005f));
	//ArmPIDController->SetI(prefs->GetDouble("Arm_I",0.0f));
	//ArmPIDController->SetD(prefs->GetDouble("Arm_D",0.0f));

	if(fabs(command) > .1f)
	{
		//Arm->Set(command);
		ArmTalon->Set(ControlMode::PercentOutput,command);

		hold_Arm = GetArmEncoder();
		/*if(ArmPIDController->IsEnabled())
		{
			ArmPIDController->Disable();
		}*/
	}
	else
	{
		/*Arm->Set(ControlMode::Position,GetArmEncoder());
		if(!ArmPIDController->IsEnabled())
		{
			ArmPIDController->Enable();
		}*/
		ArmTalon->Set(ControlMode::Position,GetArmEncoder());
		//ArmPIDController->SetSetpoint(hold_Arm);
	}
}
void ArmClass::Wrist_Update(float upcommand, float downcommand)
{

	//WristPIDController->SetP(prefs->GetDouble("Wrist_P",-0.001f));
	//WristPIDController->SetI(prefs->GetDouble("Wrist_I",0.0f));
	//WristPIDController->SetD(prefs->GetDouble("Wrist_D",0.0f));

	if(upcommand)
	{
		WristTalon->Set(ControlMode::PercentOutput,.5);
		//Wrist->Set(.5);
		hold_Wrist = GetWristEncoder();
		/*if (WristPIDController->IsEnabled())
		{
			WristPIDController->Disable();
		}*/
	}
	else if(downcommand)
	{
		WristTalon->Set(ControlMode::PercentOutput,-.5);
		//Wrist->Set(-.5);
		hold_Wrist = GetWristEncoder();
		/*if (WristPIDController->IsEnabled())
		{
			WristPIDController->Disable();
		}*/
	}
	else
	{
		float error = GetWristEncoder() - hold_Wrist;
		Preferences *prefs = Preferences::GetInstance();

		float out = error * (prefs->GetDouble("Wrist_P",.001f));

		WristTalon->Set(ControlMode::PercentOutput,out);
		/*if(!WristPIDController->IsEnabled())
		{
			WristPIDController->Enable();
		}
		WristPIDController->SetSetpoint(GetWristEncoder());
		WristPIDController->SetSetpoint(hold_Wrist);*/
	}
}
void ArmClass::Send_Data()
{
	SmartDashboard::PutNumber("Arm Encoder", GetArmEncoder());
	SmartDashboard::PutNumber("Hold Arm", hold_Arm);
	SmartDashboard::PutNumber("Wrist Encoder", GetWristEncoder());
	SmartDashboard::PutNumber("Hold Wrist", hold_Wrist);
	SmartDashboard::PutNumber("Arm Setpoint",ArmTalon->GetClosedLoopTarget(0));
	//SmartDashboard::PutNumber("Arm Setpoint",ArmPIDController->GetSetpoint());
	SmartDashboard::PutNumber("Arm Current", armCurrent);
	SmartDashboard::PutNumber("Wrist Current", wristCurrent);
}
