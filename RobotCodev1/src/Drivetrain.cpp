/*
 * Drivetrain.cpp
 *
 *  Created on: Jan 9, 2018
 *      Author: 987
 */

#include "Drivetrain.h"
#include "MyRobot.h"

using namespace frc;


float sign(float value)
{
	if(value > 0.0f)
	{
		value = 1.0f;
	}
	else if(value < 0.0f)
	{
		value = -1.0f;
	}
	else
	{
		value = 0.0f;
	}
	return value;
}

Drivetrainclass::Drivetrainclass(WPI_TalonSRX *GyroTalon)
{

	Preferences *prefs = Preferences::GetInstance();

	Drive_P = .025f;
	Strafe_P = prefs->GetDouble("Sonar_P",.05f);

	forwardramp = .01f; //.001f
	straferamp = .2f; //.01f saturday after build //.003f


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

	shifterPrev = false;
	shifterCur = false;

	toggleState = 1;

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

	shifterPiston = new DoubleSolenoid(Sol_Low_Gear,Sol_High_Gear);

#if USINGGYRO == 0
	gyro = new AnalogGyro(Gyro);
#else
	GyroTalon->SetStatusFramePeriod(StatusFrame::Status_4_AinTempVbat_,10,1.0);
	GyroTalon->SetStatusFramePeriod(StatusFrame::Status_2_Feedback0_,10,1.0);
	GyroTalon->SetStatusFramePeriod(StatusFrame::Status_1_General_,10,1.0);
	imu = NULL;
	imu = new PigeonIMU(GyroTalon);
	imu->SetYaw(0.0,1.0);
	imu->SetFusedHeading(0.0,1.0);
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
	SetActiveSonar(eSonar::LEFT_SONAR);
}

Drivetrainclass::~Drivetrainclass() {
}

void Drivetrainclass::Zero_Yaw()
{
#if USINGGYRO == 0
	gyro->Reset();
#else
	imu->SetYaw(0.0,1.0);
	imu->SetFusedHeading(0.0,1.0);
#endif
}

float Drivetrainclass::GetTurnRate()
{
#if USINGGYRO == 0
	return gyro->GetRate();
#else
	double xyz[3] = {0,0,0};

	imu->GetRawGyro(xyz);

	return -(xyz[2]);
#endif

}
float Drivetrainclass::GyroCorrection(float desheading)
{
	float MAINTAIN = desheading;

	Preferences *prefs = Preferences::GetInstance();

	Gyro_P = prefs->GetDouble("Gyro_P", 0.03f);

	float GyroRate_P = prefs->GetDouble("GyroRate_P", 0.01f);

	float error = MAINTAIN - GetHeading();
	float command = error * Gyro_P - (GyroRate_P * GetTurnRate());

	SmartDashboard::PutNumber("Gyro_P", Gyro_P);
	SmartDashboard::PutNumber("GyroRate_P", GyroRate_P);
	SmartDashboard::PutNumber("Gyro Correction Command", command);

	return command;
}
float Drivetrainclass::GetHeading()
{
#if USINGGYRO == 0
	return gyro->GetAngle();
#else
	return -(imu->GetFusedHeading());
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
	return MiddleEncoderCount;
}
void Drivetrainclass::ResetEncoders_Timers()
{
	leftEncoder->Reset();
	rightEncoder->Reset();
	middleEncoder->Reset();
	MiddleEncoderCount = 0;
	lastRawMiddleEncCount = 0;
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
void Drivetrainclass::Sense_Current()
{
	float alpha = 0.05f;

	float curleft = MyRobotClass::Get()->PDP->GetCurrent(Strafe1_PDPChannel);
	float curright = MyRobotClass::Get()->PDP->GetCurrent(Strafe2_PDPChannel);

	leftCurrent = alpha * curleft + (1.0f - alpha) * leftCurrent;
	rightCurrent = alpha * curright + (1.0f - alpha) * rightCurrent;
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
void Drivetrainclass::UpdateMiddleEncoder()
{
	int rawenc = middleEncoder->Get();
	if(strafeHasTraction)
	{
		MiddleEncoderCount += (rawenc-lastRawMiddleEncCount);
	}
	lastRawMiddleEncCount = rawenc;
}
void Drivetrainclass::StandardArcade( float Forward, float Turn, float Strafe, eGyroMode gyroMode, eBrakeMode brakeMode)
{

	Sense_Current();

	UpdateMiddleEncoder();

	float correction = 0;
	if(gyroMode == GYRO_CORRECTION_OFF)
	{
		leftcommand = Forward + Turn;
		rightcommand = -Forward + Turn;
		headingTarget = GetHeading();
	}
	else if(gyroMode == GYRO_CORRECTION_ON)
	{
		correction = GyroCorrection(headingTarget);
		leftcommand = Forward + Turn + correction;
		rightcommand = -Forward + Turn + correction;
		SmartDashboard::PutNumber("Correction", correction);
	}

	prevbrake = curbrake;
	curbrake = brakeMode;
/*
	if(prevbrake == BRAKE_MODE_OFF && curbrake == BRAKE_MODE_ON)
	{
		ResetEncoders_Timers();
	}

	if(brakeMode == BRAKE_MODE_ON)
	{
		leftcommand = Forward + LockLeft(0) + Turn + correction;
		rightcommand = -Forward - LockRight(0) + Turn + correction;

		SmartDashboard::PutNumber("Lock Left", LockLeft(GetLeftEncoder()));
		SmartDashboard::PutNumber("Lock Right", LockLeft(GetRightEncoder()));
	}
	else if(brakeMode == BRAKE_MODE_OFF)
	{
		leftcommand = Forward + Turn + correction;
		rightcommand = -Forward + Turn + correction;
	}
*/

	if(brakeEnabled)
	{
		LeftSideTicks = GetLeftEncoder();
		RightSideTicks = GetRightEncoder();

		float Lerror = LeftSideTicks+brakeTarg;
		float Rerror = RightSideTicks+brakeTarg;

		float Leftout = -Lerror*Ebrakemult;
		float Rightout = Rerror*Ebrakemult;

		leftcommand = Leftout;
		rightcommand = Rightout;
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
	//strafeHasTraction = true;
	if(MyRobotClass::Get()->AutonomousControl->Running())
	{
		if((leftCurrent > 1.4) || (rightCurrent > 1.4))
		{
			strafeHasTraction = true;
			float strafeError = Strafe - strafeMotor->Get();

			if(/*fabs(strafeError) > 1 ||*/ Strafe == 0)
			{
				Strafe = 0.0f;
			}
			if(strafeError > 0)
			{
				Strafe = strafeMotor->Get() + straferamp;
			}
			else if(strafeError < 0)
			{
				Strafe = strafeMotor->Get() - straferamp;
			}
		}
		if(sign(Strafe) == 1)
		{
			Strafe = fmax(0,Strafe);
		}
		else if(sign(Strafe) == -1)
		{
			Strafe = fmin(0,Strafe);
		}
		else
		{
			Strafe = 0;
		}
		if(sign(Strafe) != sign(prevStrafeCommand))
		{
			strafeHasTraction = false;
			leftCurrent = -fabs(leftCurrent)/2.f;
			rightCurrent = -fabs(rightCurrent)/2.f;
		}
	}
	else
	{
		strafeHasTraction = true;
	}

	prevStrafeCommand = Strafe;
	float finalstrafecommand = Strafe;

	if(!strafeHasTraction)
	{
		if(finalstrafecommand > .2f)
		{
			finalstrafecommand = .2f;
		}
		else if(finalstrafecommand < -.2f)
		{
			finalstrafecommand = -.2f;
		}
	}

	strafeMotor->SetSpeed(finalstrafecommand);
	strafeMotor1->SetSpeed(finalstrafecommand);

}

void Drivetrainclass::Shifter_Update(bool ShifterUpdate)
{

	shifterPrev = shifterCur;
	shifterCur = ShifterUpdate;

	if(!shifterPrev && shifterCur)
	{
		toggleState = -toggleState;
	}

	if(toggleState == 1)
	{
		shifterPiston->Set(DoubleSolenoid::Value::kForward);
	}
	else if(toggleState == -1)
	{
		shifterPiston->Set(DoubleSolenoid::Value::kReverse);
	}
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

/*void Drivetrainclass::CHOOSE_TARGET(float& outx, float& outy)
{
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	float cx0 = table->GetNumber("cx0",0);
	float cy0 = table->GetNumber("cy0",0);

	float tx0 = 50;
	float tx1 = 50;
	float tx2 = 50;

	float ty0 = 50;
	float ty1 = 50;
	float ty2 = 50;

	float ta0 = table->GetNumber("ta0",0);
	float ta1 = table->GetNumber("ta1",0);
	float ta2 = table->GetNumber("ta2",0);

	if(ta0 != 0)
	{
		tx0 = table->GetNumber("tx0",0);
		ty0 = table->GetNumber("ty0",0);
	}

	if(ta1 != 0)
	{
		tx1 = table->GetNumber("tx1",0);
		ty1 = table->GetNumber("ty1",0);
	}

	if(ta2 != 0)
	{
		tx2 = table->GetNumber("tx2",0);
		ty2 = table->GetNumber("ty2",0);
	}

	float valid_tx = 0;
	float valid_ty = 0;

	float tx0error = fabs(tx0-cx0);
	float tx1error = fabs(tx1-cx0);
	float tx2error = fabs(tx2-cx0);

	if((tx0error < tx1error) && (tx0error < tx2error))
	{
		valid_tx = tx0;
		valid_ty = ty0;
	}
	else if((tx1error < tx0error) && (tx1error < tx2error))
	{
		valid_tx = tx1;
		valid_ty = ty1;
	}
	else if((tx2error < tx0error) && (tx2error < tx1error))
	{
		valid_tx = tx2;
		valid_ty = ty2;
	}

	cx0 *= 27.0f;
	cy0 *= 20.5f;

	outx = ((valid_tx * 27.0f) - cx0);
	outy = ((valid_ty * 20.5f) - cy0);
}*/

void Drivetrainclass::EnabledEBrake(bool enable, int targ)
{
	brakeEnabled = enable;
	if(enable)
	{
		brakeTarg = targ;
		ResetEncoders_Timers();
	}
	else
	{
		SetRawForwardSpeed(0);
		SetRawTurnSpeed(0);
		SetRawStrafeSpeed(0);
	}
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
			strafe -= straferamp * .6;
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
	MiddleEncoderCount = 0;
	lastRawMiddleEncCount = 0;

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
	SmartDashboard::PutNumber("Turn Rate",GetTurnRate());

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


	SmartDashboard::PutNumber("Left Current", leftCurrent);
	SmartDashboard::PutNumber("Right Current", rightCurrent);
	SmartDashboard::PutBoolean("TRACTION",strafeHasTraction);

	SmartDashboard::PutNumber("Shifter",shifterPiston->Get());

	//float targetX = 0;
	//float targetY = 0;

	//CHOOSE_TARGET(targetX,targetY);

	//SmartDashboard::PutNumber("Tx", targetX);
	//SmartDashboard::PutNumber("Ty", targetY);
}
