/*
 * Auton.h
 *
 *  Created on: Jan 29, 2016
 *      Author: HighRollers
 */

#ifndef AUTON_H_
#define AUTON_H_

#include "WPILib.h"
#include "Drivetrain.h"
#include "LiftManager.h"
#include "Defines.h"

class Auton
{
	public:
		Drivetrainclass *DriveTrain;
		DriverStation *ds;
		ClawClass *Claw;
		LiftManagerClass *LiftManager;
		Timer *AutonTimer;
		Timer *SendTimer;
		Timer *IntakeTimer;

		bool Abort = false;
		bool intake = false;
		bool outake = false;
		bool uptake = false;
		bool downtake = false;
		bool dotrack = false;

		double auto_tx = 0;
		double auto_ty = 0;
		float auto_drive = 0;
		float auto_turn = 0;
		float auto_strafe = 0;

		bool Running();
		void AutonWait(float Seconds);
		void AutonWait2(float Seconds, int brake);
		void AutonWaitForTransition();
		void EBrake(float Seconds, int enctarg);
		bool Auto_System_Update();

		void Auto_DriveTimer(float Forward, float Turn, float Strafe, float seconds);
		void Auto_Intake_In();
		void Auto_Intake_Out();
		void Auto_Intake_Off();
		void Auto_GYROTURN(float heading);
		void Auto_GYROTURN_TIMED(float heading, float seconds);
		void Auto_GYROSTRAIGHT(float forward, float ticks, float desheading);
		void Auto_GYROSTRAIGHTSONAR(float forward,float ticks,float desheading, float desdistance);
		void Auto_SONAR(float desdistance,float desheading);
		void Auto_GYROSTRAFE(float forward, float ticks, float strafe, float strafeTicks, float desheading);
		void Auto_STRAFE(float strafe, float strafeTicks,float desheading);
		void Auto_STRAFEUNTIL(float strafe, float desheading, float desdistance);
		void Auto_SEARCHFORCUBE(float strafe, float heading,float time);
		void Auto_SETPIPELINE(float pipeline);
		void Auto_GYROSTRAFESONAR(float ticks, float strafe, float desheading, float desdistance);
		void Auto_FOLLOWEDGE(float Forward, float desheading, float desdistance);
		void Auto_DriveGyro_Encoder(float Forward, float Angle, float Ticks);

		void SendData();

		void Auto_DriveEncoder(float Forward, float Ticks);

		Auton
		(
				Drivetrainclass *D,
				DriverStation *Ds,
				ClawClass *C,
				LiftManagerClass *LM
		);
		~Auton();
		void Auto_Start();
		void Auto_End();

};

#endif /* AUTON_H_ */
