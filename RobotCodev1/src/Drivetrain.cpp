/*
 * Drivetrain.cpp
 *
 *  Created on: Jan 9, 2018
 *      Author: 987
 */

#include "Drivetrain.h"

using namespace frc;
Drivetrainclass::Drivetrainclass()
{
	mult = .03;
	Strafe_P = .05;

	leftcommand = 0;
	rightcommand = 0;

	lastdistance = 0;

	Gyro_P = .01f;
	Sonar_P = .06f;

	curbrake = BRAKE_MODE_OFF;
	prevbrake = BRAKE_MODE_OFF;

	currentForwardTarget = 0;

	currentStrafeTarget = 0;

	headingTarget = 0;

	currentForwardSpeed = 0;
	targetForwardSpeed = 0;

	currentStrafeSpeed = 0;
	targetStrafeSpeed = 0;

	reachedForwardTarget = false;
	reachedStrafeTarget = false;

	leftMotor = new Victor(Left_Motor_1);
	leftMotor1 = new Victor(Left_Motor_2);

	rightMotor = new Victor(Right_Motor_1);
	rightMotor1 = new Victor(Right_Motor_2);

	strafeMotor = new Victor(Strafe_Motor_1);
	strafeMotor1 = new Victor(Strafe_Motor_2);

	leftEncoder = new Encoder( Left_Encoder_1, Left_Encoder_2, false, Encoder::EncodingType::k4X);
	rightEncoder = new Encoder( Right_Encoder_1, Right_Encoder_2, false, Encoder::EncodingType::k4X);
	middleEncoder = new Encoder( Middle_Encoder_1, Middle_Encoder_2, false, Encoder::EncodingType::k4X);

	gyro = new AnalogGyro(0);

	frontsonar = new AnalogInput(2);
	//frontsonar = new Ultrasonic(8,9);
	//frontsonar->SetEnabled(true);
	//frontsonar->SetAutomaticMode(true);

	//rightsonar = new Ultrasonic(6, 7);
	//rightsonar->SetEnabled(true);
	//rightsonar->SetAutomaticMode(true);

	//leftsonar = new Ultrasonic(4,5);
	//leftsonar->SetEnabled(true);
	//leftsonar->SetAutomaticMode(true);

	leftEncoder->Reset();
	rightEncoder->Reset();
}

Drivetrainclass::~Drivetrainclass() {
}

void Drivetrainclass::Zero_Yaw()
{
	gyro->Reset();
}
float Drivetrainclass::GyroCorrection(float desheading)
{
	float MAINTAIN = desheading;

	Preferences *prefs = Preferences::GetInstance();

	Gyro_P = prefs->GetDouble("Gyro_P", 0.03f);

	float GyroRate_P = prefs->GetDouble("GyroRate_P", 0.01f);

	//float Gyro_min = prefs->GetDouble("Gyro_Min",0.1f);
	//float Gyro_max = prefs->GetDouble("Gyro_Max", 0.25f);

	float error = MAINTAIN - gyro->GetAngle();
	float command = error * Gyro_P - (GyroRate_P * gyro->GetRate());

	SmartDashboard::PutNumber("Gyro_P", Gyro_P);
	SmartDashboard::PutNumber("GyroRate_P", GyroRate_P);
	/*if(error > 0)
	{
		command = std::max(Gyro_min, command);
		command = std::min(Gyro_max, command);
	}
	if(error < 0)
	{
		command = std::min(-Gyro_min, command);
		command = std::max(-Gyro_max, command);
	}*/
	return command;
}
float Drivetrainclass::GetHeading()
{
	return gyro->GetAngle();
}
float Drivetrainclass::GetFrontSonar()
{
	return frontsonar->GetValue();
}
/*float Drivetrainclass::GetLeftSonar()
{
	return leftsonar->GetRangeInches();
}
float Drivetrainclass::GetRightSonar()
{
	return rightsonar->GetRangeInches();
}*/

int Drivetrainclass::GetLeftEncoder()
{
	return leftEncoder->Get();
}
int Drivetrainclass::GetRightEncoder()
{
	return rightEncoder->Get();
}
int Drivetrainclass::GetMiddleEncoder()
{
	return middleEncoder->Get();
}
void Drivetrainclass::ResetEncoders_Timers()
{
	leftEncoder->Reset();
	rightEncoder->Reset();
	middleEncoder->Reset();
}
float Drivetrainclass::ComputeAngleDelta(float t)
{
	float cur = gyro->GetAngle();

	if(t < 0 && cur > 0)
	{
		cur -= 360;

	}
	else if (t > 0 && cur < 0)
	{
		cur+=360;

	}


	return gyro->GetAngle() - cur;
}
float Drivetrainclass::LockLeft(float desticks)
{
	Preferences *prefs = Preferences::GetInstance();

	float MAINTAIN = desticks;
	float Drive_P = prefs->GetDouble("Drive_P", .09);

	float error = MAINTAIN - GetLeftEncoder();
	float command = error * Drive_P;

	return command;
}
float Drivetrainclass::LockRight(float desticks)
{
	Preferences *prefs = Preferences::GetInstance();

	float MAINTAIN = desticks;
	float Drive_P = prefs->GetDouble("Drive_P", .09f);

	float error = MAINTAIN - GetRightEncoder();
	float command = error * Drive_P;

	return command;
}
void Drivetrainclass::StandardArcade( float Forward, float Turn, float Strafe, eGyroMode gyroMode, eBrakeMode brakeMode)
{
	if(gyroMode == GYRO_CORRECTION_OFF)
	{
		leftcommand = Forward + Turn;
		rightcommand = -Forward + Turn;
		headingTarget = gyro->GetAngle();
	}
	else if(gyroMode == GYRO_CORRECTION_ON)
	{
		leftcommand = Forward + Turn + GyroCorrection(headingTarget);
		rightcommand = -Forward + Turn + GyroCorrection(headingTarget);
	}

	prevbrake = curbrake;
	curbrake = brakeMode;

	if(prevbrake == BRAKE_MODE_OFF && curbrake == BRAKE_MODE_ON)
	{
		ResetEncoders_Timers();
	}

	if(brakeMode == BRAKE_MODE_ON)
	{
		leftcommand = Forward + LockLeft(0) + Turn;// + GyroCorrection(lastheading);
		rightcommand = -Forward - LockRight(0) + Turn;// + GyroCorrection(lastheading);

		SmartDashboard::PutNumber("Lock Left", LockLeft(GetLeftEncoder()));
		SmartDashboard::PutNumber("Lock Right", LockLeft(GetRightEncoder()));
	}
	else if(brakeMode == BRAKE_MODE_OFF)
	{
		leftcommand = Forward + Turn;
		rightcommand = -Forward + Turn;
	}

	StandardArcade_forwardOnly(leftcommand, rightcommand);
	StandardArcade_strafeOnly(Strafe);
}
void Drivetrainclass::StandardArcade_forwardOnly(float left,float right)
{
	leftMotor->SetSpeed(left);
	leftMotor1->SetSpeed(left);
	rightMotor->SetSpeed(right);
	rightMotor1->SetSpeed(right);
}

void Drivetrainclass::StandardArcade_strafeOnly(float Strafe)
{
	strafeMotor->SetSpeed(-Strafe);
	strafeMotor1->SetSpeed(-Strafe);
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void Drivetrainclass::AutoUpdate()
{
	float forwardError = targetForwardSpeed - currentForwardSpeed;

	if(forwardError > 0)
	{
		currentForwardSpeed += 0.001f;
	}
	else if (forwardError < 0)
	{
		currentForwardSpeed -= 0.001f;
	}

	float strafeError = targetStrafeSpeed - currentStrafeSpeed;

	if(strafeError > 0)
	{
		currentStrafeSpeed += 0.001f;
	}
	else if(strafeError < 0)
	{
		currentStrafeSpeed -= 0.001f;
	}

	float forward = 0;
	float turn = 0;
	float strafe = 0;
	if (reachedForwardTarget == false)
	{
		forward = currentForwardSpeed;
	}

	if (reachedStrafeTarget == false)
	{
		strafe = currentStrafeSpeed;
	}
	//turn = GyroCorrection(headingTarget);

	StandardArcade(forward, 0.0f, strafe, GYRO_CORRECTION_ON, BRAKE_MODE_OFF);

	if (currentForwardTarget > 0) {

		if ((GetRightEncoder() >= currentForwardTarget) && (GetLeftEncoder() >= currentForwardTarget))
		{
			reachedForwardTarget = true;
		}
	}

	if (currentForwardTarget < 0)
	{
		if ((GetRightEncoder() <= currentForwardTarget) && (GetLeftEncoder() <= currentForwardTarget))
		{
			reachedForwardTarget = true;
		}
	}

	if (currentStrafeTarget > 0)
	{
		if (GetMiddleEncoder() >= currentStrafeTarget)
		{
			reachedStrafeTarget = true;
		}
	}

	if (currentStrafeTarget < 0)
	{
		if (GetMiddleEncoder() <= currentStrafeTarget)
		{
			reachedStrafeTarget = true;
		}
	}
}

void Drivetrainclass::SetForwardTarget(int target, float speed)
{
	reachedForwardTarget = false;
	leftEncoder->Reset();
	rightEncoder->Reset();
	currentForwardTarget = target;
	targetForwardSpeed = speed;
}

void Drivetrainclass::SetStrafeTarget(int target, float speed)
{
	reachedStrafeTarget = false;
	middleEncoder->Reset();
	currentStrafeTarget = target;
	targetStrafeSpeed = speed;
}
void Drivetrainclass::ResetTargets()
{
	currentForwardTarget = 0;
	currentStrafeTarget = 0;
}
void Drivetrainclass::Send_Data()
{
	SmartDashboard::PutNumber("Gyro Heading", gyro->GetAngle());
	SmartDashboard::PutNumber("Front Sonar", GetFrontSonar());
	SmartDashboard::PutNumber("HeadingTarget", headingTarget);
	SmartDashboard::PutNumber("LastDistance", lastdistance);
	SmartDashboard::PutNumber("Left Encoder", GetLeftEncoder());
	SmartDashboard::PutNumber("Right Encoder", GetRightEncoder());
	SmartDashboard::PutNumber("Middle Encoder", GetMiddleEncoder());
	SmartDashboard::PutBoolean("Reached Forward",reachedForwardTarget);
	SmartDashboard::PutBoolean("Reached Strafe",reachedStrafeTarget);
	SmartDashboard::PutNumber("Current Forward Speed", currentForwardSpeed);
	SmartDashboard::PutNumber("Current Strafe Speed", currentStrafeSpeed);
}
