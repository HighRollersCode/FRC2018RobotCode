/*
 * Drivetrain.h
 *
 *  Created on: Jan 9, 2018
 *      Author: 987
 */

#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_

#include "WPILib.h"
#include "Defines.h"
#include "Preferences.h"
#include "ctre/Phoenix.h"

#define USINGGYRO 0

class Drivetrainclass
{
public:

	float mult;

	float forwardramp;
	float straferamp;

	float lastdistance;
	float leftcommand;
	float rightcommand;

	float Gyro_P;
	float Sonar_P;
	float Strafe_P;

	eBrakeMode prevbrake;
	eBrakeMode curbrake;

	float currentForwardSpeed;
	float targetForwardEncoderSpeed;

	float targetForwardManualSpeed;

	bool ramp;

	float currentStrafeSpeed;
	float targetStrafeEncoderSpeed;

	float targetStrafeManualSpeed;

	float headingTarget;

	int currentForwardTarget;
	int currentStrafeTarget;

	float currentRightSonarTarget;
	bool reachedRightSonarTarget;

	bool reachedForwardEncoderTarget;

	bool reachedStrafeEncoderTarget;

	Victor *leftMotor;
	Victor *leftMotor1;
	Victor *rightMotor;
	Victor *rightMotor1;
	Victor *strafeMotor;
	Victor *strafeMotor1;

	Encoder *leftEncoder;
	Encoder *rightEncoder;
	Encoder *middleEncoder;

#if USINGGYRO == 0
	AnalogGyro *gyro;
#else
	PigeonIMU *imu;
#endif

	//Ultrasonic *leftsonar;
	//Ultrasonic *rightsonar;
	Ultrasonic *rightsonar;

	Drivetrainclass();
	virtual ~Drivetrainclass();
	void Zero_Yaw();
	void ResetEncoders_Timers();

	float GyroCorrection(float desheading);

	float LockLeft(float desticks);
	float LockRight(float desticks);

	float ComputeAngleDelta(float t);
	float GetHeading();
	//float GetLeftSonar();
	//float GetRightSonar();
	float GetFrontSonar();

	void StandardArcade(float Forward, float Turn, float Strafe, eGyroMode gyroMode, eBrakeMode brakeMode);
	void AutoUpdate();
	float AutoUpdate_Forward();
	float AutoUpdate_Strafe();
	void SetRawForwardSpeed(float speed);
	void SetRawStrafeSpeed(float speed);
	void SetForwardTarget(int target, float speed);
	void SetStrafeTarget(int target, float speed);
	void SetRightSonarTarg(float distance);
	void ResetTargets();
	void StandardArcade_forwardOnly(float left, float right);
	void StandardArcade_strafeOnly(float Strafe);

	void Send_Data();

	int GetLeftEncoder();
	int GetRightEncoder();
	int GetMiddleEncoder();
};

#endif /* SRC_DRIVETRAIN_H_ */
