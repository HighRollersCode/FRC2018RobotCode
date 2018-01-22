/*
 * Elevator.cpp
 *
 *  Created on: Jan 13, 2018
 *      Author: 987
 */

#include <Elevator.h>

ElevatorClass::ElevatorClass()
{
	Elevator = new WPI_VictorSPX(Elevator_Motor);
	Elevator->SetInverted(true);

	ElevatorEncoder = new Encoder(Elevator_Encoder_1,Elevator_Encoder_2,false,Encoder::EncodingType::k4X);
	ElevatorEncoder->Reset();

	ElevatorPIDController = new PIDController(0,0,0,ElevatorEncoder,Elevator,.01f);
	ElevatorPIDController->SetInputRange(ElevatorMinLimEncoder,ElevatorMaxLimEncoder);
	ElevatorPIDController->SetContinuous(false);
	ElevatorPIDController->Disable();
}

ElevatorClass::~ElevatorClass() {
}

float ElevatorClass::GetElevatorEncoder()
{
	return ElevatorEncoder->Get();
}
void ElevatorClass::SetElevator(float targ)
{
	ElevatorPIDController->SetEnabled(true);
	ElevatorPIDController->SetSetpoint(targ);

}
void ElevatorClass::ElevatorIntake()
{
	SetElevator(Elevator_Intake);
}
void ElevatorClass::ElevatorScale()
{
	SetElevator(Elevator_Scale);
}

void ElevatorClass::Update(float verticalcommand) //bool intake, bool scale)
{
	Elevator->Set(verticalcommand);
	/*if(intake)
	{
		ElevatorIntake();
	}

	if(scale)
	{
		ElevatorScale();
	}*/

	Preferences *prefs = Preferences::GetInstance();

	ElevatorPIDController->SetP(prefs->GetDouble("Elevator_P",0.003f));
	ElevatorPIDController->SetI(prefs->GetDouble("Elevator_I",0.0f));
	ElevatorPIDController->SetD(prefs->GetDouble("Elevator_D",0.004f));
}

void ElevatorClass::Send_Data()
{
	SmartDashboard::PutNumber("Elevator Encoder", GetElevatorEncoder());
	SmartDashboard::PutData("Elevator PID Controller", ElevatorPIDController);
	SmartDashboard::PutNumber("Elevator Command", Elevator->Get());
}
