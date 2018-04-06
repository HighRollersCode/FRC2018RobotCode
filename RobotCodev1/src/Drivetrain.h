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

#define USINGGYRO 1

class Drivetrainclass
{
public:

	float Drive_P;

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

	float currentTurnSpeed;

	bool reachedForwardEncoderTarget;

	bool reachedStrafeEncoderTarget;

	bool manualStrafe;
	bool manualForward;

	bool shifterPrev;
	bool shifterCur;
	float toggleState;

	bool isPossibleHighCenter = false;

	Victor *leftMotor;
	Victor *leftMotor1;
	Victor *rightMotor;
	Victor *rightMotor1;
	Victor *strafeMotor;
	Victor *strafeMotor1;

	Encoder *leftEncoder;
	Encoder *rightEncoder;
	Encoder *middleEncoder;

	DoubleSolenoid *shifterPiston;

	eSonar currentActiveSonar = eSonar::RIGHT_SONAR;

#if USINGGYRO == 0
	AnalogGyro *gyro;
#else
	PigeonIMU *imu;
#endif

	//Ultrasonic *frontsonar;
	Ultrasonic *rightsonar;
	Ultrasonic *leftsonar;
	AnalogInput *maxboticsonar;

	float leftCurrent = 0.0f;
	float rightCurrent = 0.0f;
	bool strafeHasTraction = false;
	float prevStrafeCommand = 0.0f;

	float tx = 0;
	float ty = 0;

	int LeftSideTicks = 0;
	int MidSideTicks = 0;
	int RightSideTicks = 0;

	float Ebrakemult = .20f;
	bool PrevBrakeEnabled = 0;

	bool brakeEnabled = false;
	int brakeTarg = 0;

	int MiddleEncoderCount = 0;
	int lastRawMiddleEncCount = 0;
	//bool lastStrafeHadTraction = false;

	bool gyroOn = true;

	Drivetrainclass(WPI_TalonSRX *GyroTalon);
	virtual ~Drivetrainclass();
	void Zero_Yaw();
	void UpdateMiddleEncoder();
	void ResetEncoders_Timers();

	float GyroCorrection(float desheading);
	void EnableGyroCorrection(bool enable){gyroOn = enable;}
	bool IsGyroCorrectionEnabled(){return gyroOn;}

	float LockLeft(float desticks);
	float LockRight(float desticks);


	float ComputeAngleDelta(float t);
	float GetHeading();
	float GetRoll();

	float GetTurnRate();

	float GetLeftSonar();
	float GetRightSonar();
	//float GetFrontSonar();

	float GetActiveSonar();

	void SetActiveSonar(eSonar sonar);

	void Sense_Current();

	void StandardArcade(float Forward, float Turn, float Strafe, eGyroMode gyroMode, eBrakeMode brakeMode);

	void CHOOSE_TARGET(float& outx, float& outy);

	void EnabledEBrake(bool enable, int targ);
	void AutoUpdate();
	float AutoUpdate_Forward();
	float AutoUpdate_Turn();
	float AutoUpdate_Strafe();
	void SetRawForwardSpeed(float speed);
	void SetRawStrafeSpeed(float speed);
	void SetForwardTarget(int target, float speed);
	void SetStrafeTarget(int target, float speed);
	void SetSonarTarg(float distance);
	void SetRawTurnSpeed(float speed);
	void ResetTargets();
	void StandardArcade_forwardOnly(float left, float right);
	void StandardArcade_strafeOnly(float Strafe);

	void Shifter_Update(bool ShifterUpdate);
	void Shifter_High();

	void Send_Data();

	int GetLeftEncoder();
	int GetRightEncoder();
	int GetMiddleEncoder();
};

#endif /* SRC_DRIVETRAIN_H_ */
