/*
 * Arm.cpp
 *
 *  Created on: Jan 13, 2018
 *      Author: 987
 */

#include <Arm.h>

ArmClass::ArmClass()
{
	P = 0;
	I = 0;
	D = 0;

	hold_Arm = 0;

	Arm = new WPI_VictorSPX(Arm_Motor);

	ArmEncoder = new Encoder(Arm_Encoder_1,Arm_Encoder_2,false,Encoder::EncodingType::k4X);
	ArmEncoder->Reset();

	ArmPIDController = new PIDController(P,I,D,ArmEncoder,Arm,.01f);
	ArmPIDController->SetInputRange(ArmMinLimEncoder, ArmMaxLimEncoder);
	ArmPIDController->SetOutputRange(0,.75);
	ArmPIDController->SetContinuous(false);
	ArmPIDController->Disable();
}

ArmClass::~ArmClass(){}

float ArmClass::GetArmEncoder()
{
	return ArmEncoder->Get();
}
void ArmClass::Update(float command)
{
	Preferences *prefs = Preferences::GetInstance();

	ArmPIDController->SetP(prefs->GetDouble("Arm_P",0.005f));
	ArmPIDController->SetI(prefs->GetDouble("Arm_I",0.0f));
	ArmPIDController->SetD(prefs->GetDouble("Arm_D",0.0f));

	if(fabs(command) > .1f)
	{
		Arm->Set(command);
		hold_Arm = GetArmEncoder();
	}
	else
	{
		ArmPIDController->SetEnabled(true);
		ArmPIDController->SetSetpoint(hold_Arm);
	}
}
void ArmClass::Send_Data()
{
	SmartDashboard::PutNumber("Arm Encoder", GetArmEncoder());
	SmartDashboard::PutData("Arm PID Controller", ArmPIDController);
	SmartDashboard::PutNumber("Hold Arm", hold_Arm);
}
