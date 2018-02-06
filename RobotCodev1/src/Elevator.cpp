/*
 * Elevator.cpp
 *
 *  Created on: Jan 13, 2018
 *      Author: 987
 */

#include <Elevator.h>

ElevatorClass::ElevatorClass()
{
	//Elevator = new WPI_VictorSPX(Elevator_Motor);
	//Elevator->SetInverted(false);

	ElevatorTalon = new WPI_TalonSRX(Elevator_Motor);
	ElevatorTalon->SetInverted(true);

	ElevatorTalon->Set(ControlMode::PercentOutput, 0.0);

	ElevatorTalon->ConfigContinuousCurrentLimit(20.0, 1.0);
	ElevatorTalon->EnableCurrentLimit(true);
	ElevatorTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,1.0f);
	ElevatorTalon->SetSensorPhase(true);

	//ElevatorTalon->ConfigPeakOutputForward(1.0,1.0);
	//ElevatorTalon->ConfigPeakOutputReverse(-1.0,1.0);

	ElevatorTalon->ConfigNominalOutputForward(0.0,1.0);
	ElevatorTalon->ConfigNominalOutputReverse(0.0,1.0);

	ElevatorTalon->SetSelectedSensorPosition(0,0,1.0f);
	ElevatorTalon->ConfigForwardSoftLimitThreshold(ElevatorMaxLimEncoder,1.0);
	ElevatorTalon->ConfigReverseSoftLimitThreshold(ElevatorMinLimEncoder,1.0);
	//ElevatorTalon->ConfigForwardSoftLimitEnable(true,1.0);
	//ElevatorTalon->ConfigReverseSoftLimitEnable(true,1.0);

	Preferences *prefs = Preferences::GetInstance();

	ElevatorTalon->Config_kP(0,prefs->GetDouble("Elevator_P", 0.4f),1.0);
	ElevatorTalon->Config_kI(0,prefs->GetDouble("Elevator_I", 0.0f),1.0);
	ElevatorTalon->Config_kD(0,prefs->GetDouble("Elevator_D", 0.0f),1.0);
	ElevatorTalon->Config_kF(0,0,1.0);

	currentverticalcommand = 0;
	prevverticalcommand = 0;
/*
	ElevatorJoint = new CurrentLimitedJoint(Elevator,Elevator_PDPChannel, 10.0, 1.0);

	ElevatorEncoder = new Encoder(Elevator_Encoder_1,Elevator_Encoder_2,false,Encoder::EncodingType::k4X);
	ElevatorEncoder->Reset();

	ElevatorPIDController = new PIDController(0,0,0,ElevatorEncoder,Elevator,.01f);
	ElevatorPIDController->SetInputRange(ElevatorMinLimEncoder,ElevatorMaxLimEncoder);
	ElevatorPIDController->SetContinuous(false);
	ElevatorPIDController->Disable();*/
}

ElevatorClass::~ElevatorClass() {
}

float ElevatorClass::GetElevatorEncoder()
{
	return ElevatorTalon->GetSelectedSensorPosition(0);
	//return ElevatorEncoder->Get();
}

void ElevatorClass::SetElevatorTarg(float targ)
{
	ElevatorTalon->Set(ControlMode::Position,targ);

	//ElevatorPIDController->SetEnabled(true);
	//ElevatorPIDController->SetSetpoint(targ);
}

bool ElevatorClass::ElevatorOnTarg(float tolerance)
{
	if(fabs(GetElevatorEncoder() - ElevatorTalon->GetClosedLoopTarget(0)) < tolerance) //ElevatorPIDController->GetSetpoint()
	{
		return true;
	}
	else
	{
		return false;
	}
}
void ElevatorClass::ElevatorIntake()
{
	SetElevatorTarg(Elevator_Intake);
}
void ElevatorClass::ElevatorScale()
{
	//SetElevatorTarg(Elevator_Scale);
}

void ElevatorClass::Update(float verticalcommand, bool intake)//, bool scale)
{
	prevverticalcommand = currentverticalcommand;
	currentverticalcommand = verticalcommand;

	//ElevatorJoint->SetPower(verticalcommand);
	if(fabs(verticalcommand) > .2f)
	{
		ElevatorTalon->Set(ControlMode::PercentOutput, verticalcommand);
	}

	if((fabs(prevverticalcommand) > .2) && (fabs(currentverticalcommand) <= .2))
	{
		//SetElevatorTarg(GetElevatorEncoder());
	}

	if(intake)
	{
		SetElevatorTarg(Elevator_Intake);
	}


	//ElevatorPIDController->SetP(prefs->GetDouble("Elevator_P",0.003f));
	//ElevatorPIDController->SetI(prefs->GetDouble("Elevator_I",0.0f));
	//ElevatorPIDController->SetD(prefs->GetDouble("Elevator_D",0.004f));
}

void ElevatorClass::Send_Data()
{
	SmartDashboard::PutNumber("Elevator Encoder", GetElevatorEncoder());
	SmartDashboard::PutNumber("Elevator Command", ElevatorTalon->Get());
}
