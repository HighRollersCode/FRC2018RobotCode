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

	Preferences *prefs = Preferences::GetInstance();

	Drive_P = .05f;
	Strafe_P = prefs->GetDouble("Sonar_P",.05f);

	forwardramp = .001f;
	straferamp = .003f;


	leftcommand = 0;
	rightcommand = 0;

	lastdistance = 0;

	Gyro_P = .01f;

	Sonar_P = .01f;

	curbrake = BRAKE_MODE_OFF;
	prevbrake = BRAKE_MODE_OFF;

	currentForwardTarget = 0;

	currentStrafeTarget = 0;

	headingTarget = 0;

	currentForwardSpeed = 0;
	targetForwardEncoderSpeed = 0;

	targetForwardManualSpeed = 0;

	ramp = true;

	currentStrafeSpeed = 0;
	targetStrafeEncoderSpeed = 0;
	targetStrafeManualSpeed = 0;

	reachedForwardEncoderTarget = false;
	reachedStrafeEncoderTarget = false;

	manualStrafe = false;

	manualForward = false;

	currentRightSonarTarget = 0;
	reachedRightSonarTarget = true;

	currentTurnSpeed = 0;;

	leftMotor = new Victor(Left_Motor_1);
	leftMotor1 = new Victor(Left_Motor_2);

	rightMotor = new Victor(Right_Motor_1);
	rightMotor1 = new Victor(Right_Motor_2);

	strafeMotor = new Victor(Strafe_Motor_1);
	strafeMotor1 = new Victor(Strafe_Motor_2);

	leftEncoder = new Encoder( Left_Encoder_1, Left_Encoder_2, false, Encoder::EncodingType::k4X);
	rightEncoder = new Encoder( Right_Encoder_1, Right_Encoder_2, false, Encoder::EncodingType::k4X);
	middleEncoder = new Encoder( Middle_Encoder_1, Middle_Encoder_2, false, Encoder::EncodingType::k4X);

#if USINGGYRO == 0
	gyro = new AnalogGyro(Gyro);
#else
	imu = new PigeonIMU(Pigeon_IMU);
#endif


	leftsonar = new Ultrasonic(6,7);
	leftsonar->SetEnabled(true);
	leftsonar->SetAutomaticMode(true);

	rightsonar = new Ultrasonic(8,9);
	rightsonar->SetEnabled(true);
	rightsonar->SetAutomaticMode(true);

	maxboticsonar = new AnalogInput(1);

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
#if USINGGYRO == 0
	gyro->Reset();
#else
	imu->SetFusedHeading(0,1.0);
#endif
}
float Drivetrainclass::GyroCorrection(float desheading)
{
	float MAINTAIN = desheading;

	Preferences *prefs = Preferences::GetInstance();

	Gyro_P = prefs->GetDouble("Gyro_P", 0.03f);

	float GyroRate_P = prefs->GetDouble("GyroRate_P", 0.01f);

	float error = MAINTAIN - GetHeading();
#if USINGGYRO == 0
	float command = error * Gyro_P - (GyroRate_P * gyro->GetRate());
#else
	//float command = error * Gyro_P - (GyroRate_P * imu->Get);
	float command =0;
#endif

	SmartDashboard::PutNumber("Gyro_P", Gyro_P);
	SmartDashboard::PutNumber("GyroRate_P", GyroRate_P);

	return command;
}
float Drivetrainclass::GetHeading()
{
#if USINGGYRO == 0
	return gyro->GetAngle();
#else
	return imu->GetFusedHeading();
#endif
}

float Drivetrainclass::GetLeftSonar()
{
	return leftsonar->GetRangeInches();
}

float Drivetrainclass::GetRightSonar()
{
	return rightsonar->GetRangeInches();
}

float Drivetrainclass::GetActiveSonar()
{
	if(currentActiveSonar == eSonar::LEFT_SONAR)
	{
		return GetLeftSonar();
	}
	else if(currentActiveSonar == eSonar::RIGHT_SONAR)
	{
		return GetRightSonar();
	}
	else
	{
		return 0;
	}
}
void Drivetrainclass::SetActiveSonar(eSonar sonar)
{
	currentActiveSonar = sonar;
	if(currentActiveSonar == eSonar::LEFT_SONAR)
	{
		leftsonar->SetEnabled(true);
		leftsonar->SetAutomaticMode(true);
		rightsonar->SetEnabled(false);
	}
	else if(currentActiveSonar == eSonar::RIGHT_SONAR)
	{
		rightsonar->SetEnabled(true);
		rightsonar->SetAutomaticMode(true);
		leftsonar->SetEnabled(false);
	}
	else
	{
		rightsonar->SetEnabled(false);
		leftsonar->SetEnabled(false);
	}
}
/*float Drivetrainclass::GetFrontSonar()
{
	return frontsonar->GetRangeInches();
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
	float cur = GetHeading();

	if(t < 0 && cur > 0)
	{
		cur -= 360;

	}
	else if (t > 0 && cur < 0)
	{
		cur+=360;

	}


	return GetHeading() - cur;
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
		headingTarget = GetHeading();
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
/*
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
*/
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
	strafeMotor->SetSpeed(Strafe);
	strafeMotor1->SetSpeed(Strafe);
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void Drivetrainclass::AutoUpdate()
{
	currentForwardSpeed = AutoUpdate_Forward();
	currentStrafeSpeed = AutoUpdate_Strafe();
	currentTurnSpeed = AutoUpdate_Turn();

	StandardArcade(currentForwardSpeed, currentTurnSpeed, currentStrafeSpeed, GYRO_CORRECTION_ON, BRAKE_MODE_OFF);

	if (currentForwardTarget > 0) {

		if ((GetRightEncoder() >= currentForwardTarget) && (GetLeftEncoder() >= currentForwardTarget))
		{
			reachedForwardEncoderTarget = true;
		}
	}

	if (currentForwardTarget < 0)
	{
		if ((GetRightEncoder() <= currentForwardTarget) && (GetLeftEncoder() <= currentForwardTarget))
		{
			reachedForwardEncoderTarget = true;
		}
	}

	if (currentStrafeTarget > 0)
	{
		if (GetMiddleEncoder() >= currentStrafeTarget)
		{
			reachedStrafeEncoderTarget = true;
		}
	}

	if (currentStrafeTarget < 0)
	{
		if (GetMiddleEncoder() <= currentStrafeTarget)
		{
			reachedStrafeEncoderTarget = true;
		}
	}

	if(fabs(GetActiveSonar() - currentRightSonarTarget) < 1)
	{
		//reachedRightSonarTarget = true;
	}
}
float Drivetrainclass::AutoUpdate_Forward()
{
	float forward = currentForwardSpeed;

	if(!manualForward)
	{
		if(reachedForwardEncoderTarget == false)
		{
			float forwardError = targetForwardEncoderSpeed - currentForwardSpeed;

			if(forwardError > 0)
			{
				forward += forwardramp;
			}
			else if (forwardError < 0)
			{
				forward -= forwardramp;
			}
		}
		else
		{
			forward = 0;
		}
	}

	if(forward < -1)
	{
		forward = -1;
	}
	else if(forward > 1)
	{
		forward = 1;
	}

	return forward;
}

void Drivetrainclass::SetRawForwardSpeed(float speed)
{
	currentForwardSpeed = speed;
	reachedForwardEncoderTarget = true;
	manualForward = true;
}

void Drivetrainclass::SetForwardTarget(int target, float speed)
{
	reachedForwardEncoderTarget = false;
	manualForward = false;
	leftEncoder->Reset();
	rightEncoder->Reset();
	currentForwardTarget = target;
	targetForwardEncoderSpeed = speed;
}

float Drivetrainclass::AutoUpdate_Strafe()
{
	float strafe = currentStrafeSpeed;

	if(!manualStrafe)
	{
		float strafeError = 0;

		if(reachedStrafeEncoderTarget == false)
		{
			strafeError = targetStrafeEncoderSpeed - currentStrafeSpeed;
		}
		else if(reachedRightSonarTarget == false)
		{
			float gain = Strafe_P;
			if(currentActiveSonar == eSonar::RIGHT_SONAR)
			{
				gain *= -1;
			}
			float sonarError = currentRightSonarTarget - GetActiveSonar();
			float sonarCommand = sonarError * gain;
			strafeError = sonarCommand - strafe;
		}
		else
		{
			strafe = 0;
		}



		if(strafeError > 0)
		{
			strafe += straferamp;
		}
		else if(strafeError < 0)
		{
			strafe -= straferamp;
		}
		SmartDashboard::PutNumber("Strafe Errror", strafeError);

	}
	if(strafe < -1)
	{
		strafe = -1;
	}
	else if(strafe > 1)
	{
		strafe = 1;
	}
	return strafe;
}

void Drivetrainclass::SetRawStrafeSpeed(float speed)
{
	currentStrafeSpeed = speed;
	manualStrafe = true;
	reachedStrafeEncoderTarget = true;
	reachedRightSonarTarget = true;
}

void Drivetrainclass::SetStrafeTarget(int target, float speed)
{
	reachedStrafeEncoderTarget = false;
	reachedRightSonarTarget = true;
	manualStrafe = false;

	middleEncoder->Reset();

	currentStrafeTarget = target;
	targetStrafeEncoderSpeed = speed;
}

void Drivetrainclass::SetSonarTarg(float distance)
{
	reachedStrafeEncoderTarget = true;
	reachedRightSonarTarget = false;
	manualStrafe = false;
	currentRightSonarTarget = distance;
}

float Drivetrainclass::AutoUpdate_Turn()
{
	float turn = currentTurnSpeed;

	return turn;
}

void Drivetrainclass::SetRawTurnSpeed(float speed)
{
	currentTurnSpeed = speed;
}

void Drivetrainclass::ResetTargets()
{
	currentForwardTarget = 0;
	currentStrafeTarget = 0;
	currentRightSonarTarget = 0;

	reachedForwardEncoderTarget = true;
	reachedStrafeEncoderTarget = true;
	reachedRightSonarTarget = true;
}
void Drivetrainclass::Send_Data()
{
	SmartDashboard::PutNumber("Gyro Heading", GetHeading());
	SmartDashboard::PutNumber("HeadingTarget", headingTarget);

	SmartDashboard::PutNumber("Left Sonar Value", GetLeftSonar());
	SmartDashboard::PutNumber("Right Sonar Value", GetRightSonar());
	SmartDashboard::PutNumber("Active Sonar Value", GetActiveSonar());
	SmartDashboard::PutNumber("Active Sonar", currentActiveSonar);
	SmartDashboard::PutNumber("Maxbotics Sonar", maxboticsonar->GetValue());
	SmartDashboard::PutNumber("Maxbotics Sonar Conversion", (maxboticsonar->GetValue()/.0098f));

	SmartDashboard::PutNumber("LastDistance", lastdistance);

	SmartDashboard::PutNumber("Left Encoder", GetLeftEncoder());
	SmartDashboard::PutNumber("Right Encoder", GetRightEncoder());
	SmartDashboard::PutNumber("Middle Encoder", GetMiddleEncoder());

	SmartDashboard::PutBoolean("Reached Forward",reachedForwardEncoderTarget);
	SmartDashboard::PutBoolean("Reached Strafe",reachedStrafeEncoderTarget);

	SmartDashboard::PutNumber("Current Forward Speed", currentForwardSpeed);
	SmartDashboard::PutNumber("Current Strafe Speed", currentStrafeSpeed);
}
