/*
 * Auton.cpp
 *
 *  Created on: Jan 29, 2016
 *      Author: HighRollers
 */

#include "Auton.h"
#include "math.h"
#include "stdio.h"
#include "MyRobot.h"


Auton::Auton
(
	Drivetrainclass *D,
	DriverStation *Ds,
	ClawClass *C,
	LiftManagerClass *LM
)
{
	DriveTrain = D;
	ds = Ds;
	Claw = C;
	LiftManager = LM;

	AutonTimer = new Timer();
	SendTimer =new Timer();
	IntakeTimer = new Timer();
}
void Auton::Auto_Start()
{
	DriveTrain->Zero_Yaw();
	AutonTimer->Reset();
	AutonTimer->Start();

	SendTimer->Reset();
	SendTimer->Start();
	DriveTrain->ResetEncoders_Timers();
	DriveTrain->currentRightSonarTarget = DriveTrain->GetFrontSonar();
	DriveTrain->headingTarget = DriveTrain->GetHeading();
	DriveTrain->SetForwardTarget(0,0);
	DriveTrain->SetStrafeTarget(0,0);
}
void Auton::Auto_End()
{
	LiftManager->changeMode(LiftMode::Free);
}
bool Auton::Running()
{
	if(Abort)
	{
		printf("ABORTING: ABORT SET %f /r/n", AutonTimer->Get());
		return false;
	}
	else if (ds->IsOperatorControl() == true)
	{
		printf("ABORTING: OPERATOR CONTROL %f /r/n", AutonTimer->Get());
		return false;
	}
	else if (ds->IsEnabled() == false)
	{
		printf("ABORTING: DS DISABLED %f /r/n", AutonTimer->Get());
		return false;
	}
	return true;
}
void Auton::AutonWait(float Seconds)
{
	float targ = AutonTimer->Get() + Seconds;
	while((AutonTimer->Get() < targ)&&(Running()))
	{
		Auto_System_Update();
	}
}
void Auton::AutonWait2(float Seconds,int brake)
{
	float targ = AutonTimer->Get() + Seconds;

	while((AutonTimer->Get() < targ)&&(Running()))
	{
		Auto_System_Update();
	}

	DriveTrain->StandardArcade(0.0f,0.0f, 0.0f, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
}
void Auton::AutonWaitForTransition()
{
	while(LiftManager->transitioning == true)
	{
		Auto_System_Update();
	}
}
void Auton::Auto_DriveTimer(float Forward, float Turn, float Strafe, float seconds)
{
	DriveTrain->SetRawForwardSpeed(Forward);
	DriveTrain->SetRawStrafeSpeed(Strafe);

	AutonWait(seconds);

	DriveTrain->SetRawForwardSpeed(0);
	DriveTrain->SetRawStrafeSpeed(0);
}
void Auton::Auto_GYROTURN(float heading)
{
	DriveTrain->ResetEncoders_Timers();

	DriveTrain->headingTarget = heading;

	float error = DriveTrain->GetHeading() - heading;

	while(fabs(error) > 5 && Running())
	{
		error = DriveTrain->GetHeading() - heading;
		Auto_System_Update();
	}
	DriveTrain->StandardArcade(0,0,0,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);
}
void Auton::Auto_GYROTURN_TIMED(float heading, float seconds)
{
	printf("Timing");
	DriveTrain->ResetEncoders_Timers();

	DriveTrain->headingTarget = heading;

	float timtarg = AutonTimer->Get()+seconds;

	while(AutonTimer->Get() < timtarg && Running())
	{
		Auto_System_Update();
	}
	DriveTrain->StandardArcade(0,0,0, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
}
void Auton::Auto_GYROSTRAIGHT(float forward, float ticks, float desheading)
{
	DriveTrain->ResetEncoders_Timers();

	DriveTrain->SetForwardTarget(ticks,forward);

	DriveTrain->headingTarget = desheading;

	DriveTrain->reachedRightSonarTarget = true;

	bool complete = DriveTrain->reachedForwardEncoderTarget;

	while((Running()) && !complete)
	{
		Auto_System_Update();
		complete = DriveTrain->reachedForwardEncoderTarget;
	}
	DriveTrain->StandardArcade(0,0,0, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
}
void Auton::Auto_GYROSTRAIGHTSONAR(float forward,float ticks,float desheading,float desdistance)
{
	DriveTrain->ResetEncoders_Timers();

	DriveTrain->headingTarget = desheading;
	DriveTrain->SetForwardTarget(ticks,forward);
	DriveTrain->SetRightSonarTarg(desdistance);

	bool complete = (DriveTrain->reachedForwardEncoderTarget);

	while (Running() && (!complete))
	{
		Auto_System_Update();
		complete = (DriveTrain->reachedForwardEncoderTarget);

	}

	DriveTrain->StandardArcade(0,0,0,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);
}
void Auton::Auto_SONAR(float desdistance,float desheading)
{
	DriveTrain->SetRightSonarTarg(desdistance);
	DriveTrain->headingTarget = desheading;

	float sonarerror = desdistance - DriveTrain->GetFrontSonar();

	while((Running()) && (fabs(sonarerror) > 2))
	{
		Auto_System_Update();
		sonarerror = desdistance - DriveTrain->GetFrontSonar();
	}
}
void Auton::Auto_GYROSTRAFE(float forward, float ticks, float strafe, float strafeTicks, float desheading)
{
	DriveTrain->ResetEncoders_Timers();
	DriveTrain->SetForwardTarget(ticks,forward);
	DriveTrain->SetStrafeTarget(strafeTicks,strafe);
	DriveTrain->headingTarget = desheading;

	bool complete = (DriveTrain->reachedForwardEncoderTarget) && (DriveTrain->reachedStrafeEncoderTarget);

	while (Running() && (!complete))
	{
		Auto_System_Update();
		complete = (DriveTrain->reachedForwardEncoderTarget) && (DriveTrain->reachedStrafeEncoderTarget);

	}
	DriveTrain->StandardArcade(0,0,0,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);
}
void Auton::Auto_STRAFE(float strafe, float strafeTicks, float desheading)
{
	DriveTrain->ResetEncoders_Timers();
	DriveTrain->SetStrafeTarget(strafeTicks,strafe);
	DriveTrain->headingTarget = desheading;

	bool complete = DriveTrain->reachedStrafeEncoderTarget;

	while (Running() && (!complete))
	{
		Auto_System_Update();
		complete = DriveTrain->reachedStrafeEncoderTarget;
	}
	DriveTrain->SetRawStrafeSpeed(0);
	DriveTrain->SetRawForwardSpeed(0);
}
void Auton::Auto_SEARCHFORCUBE(float strafe, float heading,float time)
{
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	DriveTrain->headingTarget = heading;

	// strafe until we see the cube
	while(fabs(table->GetNumber("tv",0) == 0) && (Running()))
	{
		Auto_System_Update();
		DriveTrain->StandardArcade(0,0,strafe,GYRO_CORRECTION_ON,BRAKE_MODE_OFF);
	}

	// Now grab the cube
	bool gotcube = false;
	int counter = 0;

	IntakeTimer->Reset();
	IntakeTimer->Start();

	while(gotcube == false && (Running()))
	{
		Auto_System_Update();

		float tx = table->GetNumber("tx", 0);
		float ty = table->GetNumber("ty", 0);

		float distance_error = ty;
		float cube_error = tx;

		auto_drive = distance_error * DriveTrain->mult;
		auto_turn = cube_error * DriveTrain->Gyro_P;
		auto_strafe = 0; //cube_error * DriveTrain->Strafe_P;

		DriveTrain->StandardArcade(auto_drive,auto_turn,auto_strafe,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);

		//printf("Target: %.2f %.2f   turn: %f", tx,ty,auto_turn);

		if(IntakeTimer->Get() > time)
		{
			gotcube = true;
		}

		if((fabs(cube_error) < 7) && (distance_error <= 1))
		{
			counter++;
		}
		else if((fabs(cube_error) < 2) && (distance_error <= 6))
		{
			counter++;
		}
		else
		{
			counter--;// = 0;
			if(counter < 0)
			{
				counter = 0;
			}
		}

		if (counter > 200)
		{
			printf("Got Cube !!!!");
			gotcube = true;
		}
	}
	DriveTrain->StandardArcade(0,0,0,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);
}

void Auton::Auto_GYROSTRAFESONAR(float ticks, float strafe, float desheading, float desdistance)
{
	/*DriveTrain->ResetEncoders_Timers();
	DriveTrain->headingTarget = desheading;
	DriveTrain->SetStrafeTarget(ticks,strafe);
	DriveTrain->SetRightSonarTarg(desdistance);

	float GYRO_P = DriveTrain->Gyro_P;



	float SonarMaintain = desdistance;
	float Strafe_P = DriveTrain->Sonar_P;
	float Sonar_error = SonarMaintain - DriveTrain->GetFrontSonar();
	float forward = Sonar_error * Strafe_P;

	if(ticks > 0)
	{
		while((-DriveTrain->GetLeftEncoder() < ticks)&&(Running()))
		{
			Sonar_error = SonarMaintain - DriveTrain->GetFrontSonar();
			forward = Sonar_error * Strafe_P;

			DriveTrain->StandardArcade(forward, 0,strafe,GYRO_CORRECTION_ON,BRAKE_MODE_OFF);
			Auto_System_Update();
		}
	}
	else
	{
		while((-DriveTrain->GetLeftEncoder() > ticks)&&(Running()))
		{
			float err = DriveTrain->ComputeAngleDelta(MAINTAIN);

			turn = err * GYRO_P;

			Sonar_error = SonarMaintain - DriveTrain->GetFrontSonar();
			forward = Sonar_error * Strafe_P;

			DriveTrain->StandardArcade(forward, turn,strafe,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);
			Auto_System_Update();
		}
	}
	DriveTrain->StandardArcade(0,0,0,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);*/
}
void Auton::Auto_FOLLOWEDGE(float Forward, float desheading, float desdistance)
{

	float SonarMaintain = desdistance;
	float Strafe_P = DriveTrain->Sonar_P;
	float Sonar_error = SonarMaintain - DriveTrain->GetFrontSonar();
	float strafe = Sonar_error * Strafe_P;

	while((fabs(Sonar_error) < 50) && (Running()))
	{
		Sonar_error = SonarMaintain - DriveTrain->GetFrontSonar();
		strafe = Sonar_error * Strafe_P;

		DriveTrain->StandardArcade(Forward, 0,strafe,GYRO_CORRECTION_ON,BRAKE_MODE_OFF);
		Auto_System_Update();
	}
	DriveTrain->StandardArcade(0,0,0,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);
}
void Auton::Auto_DriveGyro_Encoder(float Forward, float Angle, float Ticks)
{
	DriveTrain->ResetEncoders_Timers();
	Auto_GYROTURN(Angle);
	AutonWait(.7f);
	Auto_GYROSTRAIGHT(Forward, Ticks, Angle);
}

bool Auton::Auto_System_Update()
{
	if(Running())
	{
		SendData();

		LiftManager->UpdateLift(false,false,false,false,false,false);

		DriveTrain->AutoUpdate();

		if(AutonTimer->Get() > 14.95)
		{
		}
		Wait(.001f);
	}
	return true;
}
void Auton::SendData()
{
	if(SendTimer->Get() > .05)
	{
		SendTimer->Reset();
		SendTimer->Start();

		DriveTrain->Send_Data();
		SmartDashboard::PutNumber("AUTOTIMER",AutonTimer->Get());
		SmartDashboard::PutNumber("Auto Tx",auto_tx);
		SmartDashboard::PutNumber("Auto Ty",auto_ty);
		SmartDashboard::PutNumber("Auto Drive",auto_drive);
		SmartDashboard::PutNumber("Auto Turn",auto_turn);
		SmartDashboard::PutNumber("Auto Strafe",auto_strafe);
	}
}
void Auton::Auto_Intake_In()
{
	Claw->Update(true,false,false,false);
}
void Auton::Auto_Intake_Out()
{
	Claw->Update(false,false,true,false);
}
void Auton::Auto_Intake_Off()
{
	Claw->Update(false,false,false,false);
}
void Auton::Auto_DriveEncoder(float Forward, float Turn, float Ticks)
{
	DriveTrain->ResetEncoders_Timers();

	if(Ticks > 0)
	{
		while((DriveTrain->GetLeftEncoder() < Ticks)&& Running())
		{
			DriveTrain->StandardArcade(Forward ,Turn, 0, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
			Auto_System_Update();
		}
	}
	else
	{
		while((DriveTrain->GetLeftEncoder() > Ticks)&& Running())
		{
			DriveTrain->StandardArcade(Forward, Turn, 0.0f, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
			Auto_System_Update();
		}
	}
	printf("Finished Driving");
	DriveTrain->StandardArcade(0, 0, 0, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
}
