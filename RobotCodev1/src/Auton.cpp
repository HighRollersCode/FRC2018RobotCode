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
	DriverStation *Ds
)
{
	DriveTrain = D;
	ds = Ds;
	AutonTimer = new Timer();
	SendTimer =new Timer();
}
void Auton::Auto_Start()
{
	DriveTrain->Zero_Yaw();
	AutonTimer->Reset();
	AutonTimer->Start();

	SendTimer->Reset();
	SendTimer->Start();
	DriveTrain->ResetEncoders_Timers();
}
void Auton::Auto_End()
{

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
	//DriveTrain->ResetEncoders_Timers();
	while((AutonTimer->Get() < targ)&&(Running()))
	{
		Auto_System_Update();
		//DriveTrain->UpdateEBrake(1,brake);
	}
	DriveTrain->StandardArcade(0.0f,0.0f, 0.0f, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
}
void Auton::Auto_DriveTimer(float Forward, float Turn, float Strafe, float seconds)
{
	DriveTrain->StandardArcade(Forward, Turn, Strafe, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
	AutonWait(seconds);
	DriveTrain->StandardArcade(0.0f, 0.0f, 0.0f, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
}
void Auton::Auto_GYROTURN(float heading)
{
	DriveTrain->ResetEncoders_Timers();

	DriveTrain->headingTarget = heading;

	float error = DriveTrain->GetHeading() - heading;

	while(fabs(error) > 7 && Running())
	{
		error = DriveTrain->GetHeading() - heading;
		Auto_System_Update();
		DriveTrain->AutoUpdate();
	}
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
		DriveTrain->AutoUpdate();
	}
	DriveTrain->StandardArcade(0,0,0, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
}
void Auton::Auto_GYROSTRAIGHT(float forward, float ticks, float desheading)
{
	DriveTrain->ResetEncoders_Timers();

	DriveTrain->headingTarget = desheading;

	if(ticks > 0)
	{
		while((DriveTrain->GetLeftEncoder() < ticks)&&(DriveTrain->GetRightEncoder() < ticks)&&(Running()))
		{

			DriveTrain->StandardArcade(forward,0,0.0f, GYRO_CORRECTION_ON, BRAKE_MODE_OFF);
			Auto_System_Update();
		}
	}
	else if (ticks < 0)
	{
		while((DriveTrain->GetLeftEncoder() > ticks)&&(DriveTrain->GetRightEncoder() > ticks)&&(Running()))
		{
			DriveTrain->StandardArcade(forward, 0, 0, GYRO_CORRECTION_ON, BRAKE_MODE_OFF);
			Auto_System_Update();
		}
	}
	DriveTrain->StandardArcade(0,0,0, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
}
void Auton::Auto_GYROSTRAIGHTSONAR(float forward,float ticks,float desheading,float desdistance)
{
	DriveTrain->ResetEncoders_Timers();

	float SonarMaintain = desdistance;
	float Strafe_P = DriveTrain->Strafe_P;
	float Sonar_error = SonarMaintain - DriveTrain->GetFrontSonar();
	float StrafeCom = Sonar_error * Strafe_P;

	if(ticks > 0)
	{
		while((-DriveTrain->GetLeftEncoder() < ticks)&&(Running()))
		{
			Sonar_error = SonarMaintain - DriveTrain->GetFrontSonar();
			StrafeCom = Sonar_error * Strafe_P;

			DriveTrain->StandardArcade(forward, 0,-StrafeCom,GYRO_CORRECTION_ON,BRAKE_MODE_OFF);
			Auto_System_Update();
		}
	}
	else
	{
		while((-DriveTrain->GetLeftEncoder() > ticks)&&(Running()))
		{
			Sonar_error = SonarMaintain - DriveTrain->GetFrontSonar();
			StrafeCom = Sonar_error * Strafe_P;

			DriveTrain->StandardArcade(forward, 0,-StrafeCom,GYRO_CORRECTION_ON,BRAKE_MODE_OFF);
			Auto_System_Update();
		}
	}
	DriveTrain->StandardArcade(0,0,0,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);
}
void Auton::Auto_GYROSTRAFE(float forward, float ticks, float strafe, float strafeTicks, float desheading)
{
	DriveTrain->ResetEncoders_Timers();
	DriveTrain->SetForwardTarget(ticks,forward);
	DriveTrain->SetStrafeTarget(strafeTicks,strafe);
	DriveTrain->headingTarget = desheading;

	bool complete = (DriveTrain->reachedForwardTarget) && (DriveTrain->reachedStrafeTarget);

	while (Running() && (!complete))
	{
		Auto_System_Update();
		complete = (DriveTrain->reachedForwardTarget) && (DriveTrain->reachedStrafeTarget);

		DriveTrain->AutoUpdate();
	}
}
void Auton::Auto_SEARCHFORCUBE(float strafe, float heading)
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

	while(gotcube == false && (Running()))
	{
		Auto_System_Update();

		float tx = table->GetNumber("tx", 0);
		float ty = table->GetNumber("ty", 0);

		float distance_error = -ty;
		float cube_error = tx;

		float drive = distance_error * DriveTrain->mult;
		float strafe = cube_error * DriveTrain->Strafe_P;

		DriveTrain->StandardArcade(drive,0,strafe,GYRO_CORRECTION_ON,BRAKE_MODE_OFF);

		if(fabs(distance_error) < 1 && fabs(cube_error) < 1)
		{
			printf("Target X : %.2f", tx);
			printf("Target Y : %.2f", ty);
			printf("Got Cube !!!!");
			gotcube = true;
		}
	}
	DriveTrain->StandardArcade(0,0,0,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);
}

void Auton::Auto_GYROSTRAFESONAR(float ticks, float strafe, float desheading, float desdistance)
{
	DriveTrain->ResetEncoders_Timers();
	float MAINTAIN = desheading;
	float GYRO_P = DriveTrain->Gyro_P;

	float angle_error = MAINTAIN - DriveTrain->GetHeading();
	float turn = GYRO_P * angle_error;

	float SonarMaintain = desdistance;
	float Strafe_P = DriveTrain->Sonar_P;
	float Sonar_error = SonarMaintain - DriveTrain->GetFrontSonar();
	float forward = Sonar_error * Strafe_P;

	if(ticks > 0)
	{
		while((-DriveTrain->GetLeftEncoder() < ticks)&&(Running()))
		{
			float err = DriveTrain->ComputeAngleDelta(MAINTAIN);

			turn = err * GYRO_P;

			Sonar_error = SonarMaintain - DriveTrain->GetFrontSonar();
			forward = Sonar_error * Strafe_P;

			DriveTrain->StandardArcade(forward, turn,strafe,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);
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
	DriveTrain->StandardArcade(0,0,0,GYRO_CORRECTION_OFF,BRAKE_MODE_OFF);
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
	}
}
void Auton::Auto_Intake_Off()
{
	intake = false;
	outake = false;
}
void Auton::Auto_Intake_On()
{
	intake = true;
	outake = false;
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
			//DriveTrain->Failsafe_Update();
		}
	}
	else
	{
		while((DriveTrain->GetLeftEncoder() > Ticks)&& Running())
		{
			DriveTrain->StandardArcade(Forward, Turn, 0.0f, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
			Auto_System_Update();
			//DriveTrain->FailSafe_Update();
		}
	}
	printf("Finished Driving");
	DriveTrain->StandardArcade(0, 0, 0, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
}
