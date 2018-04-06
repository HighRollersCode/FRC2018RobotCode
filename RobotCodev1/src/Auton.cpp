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
	DriveTrain->currentRightSonarTarget = DriveTrain->GetActiveSonar();
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
		printf("ABORTING: ABORT SET %f \r\n", AutonTimer->Get());
		return false;
	}
	else if (ds->IsOperatorControl() == true)
	{
		//printf("ABORTING: OPERATOR CONTROL %f \r\n", AutonTimer->Get());
		return false;
	}
	else if (ds->IsEnabled() == false)
	{
		printf("ABORTING: DS DISABLED %f \r\n", AutonTimer->Get());
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
	while((Running()) && (LiftManager->transitioning == true))
	{
		Auto_System_Update();
	}
}
void Auton::Auto_DriveTimer(float Forward, float Turn, float Strafe, float seconds)
{
	DriveTrain->SetRawForwardSpeed(Forward);
	DriveTrain->SetRawStrafeSpeed(Strafe);
	DriveTrain->EnableGyroCorrection(false);

	AutonWait(seconds);

	DriveTrain->SetRawForwardSpeed(0);
	DriveTrain->SetRawStrafeSpeed(0);
	DriveTrain->EnableGyroCorrection(true);
}
void Auton::EBrake(float Seconds,int enctarg)
{
	DriveTrain->EnabledEBrake(true,enctarg);

	AutonWait(Seconds);

	DriveTrain->EnabledEBrake(false,enctarg);
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
	DriveTrain->SetSonarTarg(desdistance);

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
	DriveTrain->SetSonarTarg(desdistance);
	DriveTrain->headingTarget = desheading;

	float sonarerror = desdistance - DriveTrain->GetActiveSonar();

	while((Running()) && (fabs(sonarerror) > 2))
	{
		Auto_System_Update();
		sonarerror = desdistance - DriveTrain->GetActiveSonar();
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
void Auton::Auto_STRAFEUNTIL(float strafe, float desheading, float desdistance)
{
	DriveTrain->ResetEncoders_Timers();
	DriveTrain->headingTarget = desheading;

	float currentdistance = DriveTrain->GetActiveSonar();
	float initialerror = desdistance - currentdistance;
	float error = desdistance - DriveTrain->GetActiveSonar();
	bool goingLeft = false;

	if((initialerror < 0 && DriveTrain->currentActiveSonar == eSonar::LEFT_SONAR)
			||(initialerror > 0 && DriveTrain->currentActiveSonar == eSonar::RIGHT_SONAR))
	{
		goingLeft = true;
	}
	else
	{
		goingLeft = false;
	}

	if(goingLeft)
	{
		strafe = -fabs(strafe);
	}
	else
	{
		strafe = fabs(strafe);
	}

	DriveTrain->SetRawStrafeSpeed(strafe);

	if(initialerror > 0)
	{
		while (Running() && (DriveTrain->GetActiveSonar() < desdistance) && fabs(error) > 3)
		{
			Auto_System_Update();
			 error = desdistance - DriveTrain->GetActiveSonar();
		}
	}
	else if(initialerror < 0)
	{
		while (Running() && (DriveTrain->GetActiveSonar() > desdistance) && fabs(error) > 3)
		{
			Auto_System_Update();
			 error = desdistance - DriveTrain->GetActiveSonar();
		}
	}

	DriveTrain->SetRawStrafeSpeed(0);
}
void Auton::Auto_SEARCHFORCUBETURN(float turn, float time)
{
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");
	printf("Turn: %f and Time: %f",turn,time);

	// strafe until we see the cube
	while(fabs(table->GetNumber("tv",0) == 0) && (Running()))
	{
		DriveTrain->SetRawTurnSpeed(turn);
		DriveTrain->EnableGyroCorrection(false);

		Auto_System_Update();
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

		float distance_error = ty + 5;
		float cube_error = tx;

		auto_drive = 0.15f;//distance_error * DriveTrain->Drive_P;
		auto_turn = cube_error * .025f;
		auto_strafe = 0; //cube_error * DriveTrain->Strafe_P;

		DriveTrain->SetRawForwardSpeed(auto_drive);
		DriveTrain->SetRawTurnSpeed(auto_turn);
		DriveTrain->SetRawStrafeSpeed(auto_strafe);

		if(IntakeTimer->Get() > time)
		{
			gotcube = true;
		}

		if(Claw->GotCube())
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

		if (counter > 20)
		{
			printf("Got Cube !!!!");
			gotcube = true;
		}
	}
	DriveTrain->SetRawForwardSpeed(0);
	DriveTrain->SetRawTurnSpeed(0);
	DriveTrain->SetRawStrafeSpeed(0);
	DriveTrain->EnableGyroCorrection(true);

}
void Auton::Auto_SEARCHFORCUBESTRAFE(float strafe, float heading,float time)
{
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	DriveTrain->headingTarget = heading;

	DriveTrain->SetRawStrafeSpeed(strafe);

	// strafe until we see the cube
	bool saw_cube = false;
	while(!saw_cube && (Running()))
	{
		float tx = table->GetNumber("tx",0);
		float ty = 0;
		float tv = table->GetNumber("tv",0);

		if(strafe < 0)
		{
			tx += 9;
		}
		auto_tx = tx;
		auto_ty = ty;
		saw_cube = ((tv != 0) && (fabs(tx) < 2.0));
		Auto_System_Update();
	}

	// Now grab the cube
	bool gotcube = false;
	int counter = 0;

	IntakeTimer->Reset();
	IntakeTimer->Start();

	while(gotcube == false && (Running()))
	{
		Auto_System_Update();
		float tv  = table->GetNumber("tv", 0);

		float tx = 0;
		float ty = 0;

		if(tv != 0)
		{
			auto_tx = tx;
			auto_ty = ty;

			tx = table->GetNumber("tx", 0);
			ty = table->GetNumber("ty", 0);
		}

		float distance_error = ty + 5;
		float cube_error = tx;

		if(tv == 0)
		{
			distance_error = 0;
			auto_strafe = strafe;
			auto_drive = 0;
			auto_turn = 0;
		}
		else
		{
			auto_drive = distance_error * DriveTrain->Drive_P;
			auto_turn = 0; //cube_error * DriveTrain->Gyro_P;
			auto_strafe = cube_error * DriveTrain->Strafe_P;
		}

		DriveTrain->SetRawForwardSpeed(auto_drive);
		DriveTrain->SetRawTurnSpeed(auto_turn);
		DriveTrain->SetRawStrafeSpeed(auto_strafe);

		if(IntakeTimer->Get() > time)
		{
			gotcube = true;
		}

		if(Claw->GotCube())
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

		if (counter > 50)
		{
			printf("Got Cube !!!!");
			gotcube = true;
		}
	}
	DriveTrain->SetRawForwardSpeed(0);
	DriveTrain->SetRawTurnSpeed(0);
	DriveTrain->SetRawStrafeSpeed(0);
}
void Auton::Auto_SEARCHFORCUBEFORWARD(float forward, float heading,float time)
{
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	DriveTrain->headingTarget = heading;

	DriveTrain->SetRawForwardSpeed(forward);
	// Now grab the cube
	bool gotcube = false;
	int counter = 0;

	IntakeTimer->Reset();
	IntakeTimer->Start();

	while(gotcube == false && (Running()))
	{
		Auto_System_Update();
		/*float tv  = table->GetNumber("tv", 0);

		float tx = 0;
		float ty = 0;

		if(tv != 0)
		{
			auto_tx = tx;
			auto_ty = ty;

			tx = table->GetNumber("tx", 0);
			ty = table->GetNumber("ty", 0);
		}

		float distance_error = ty + 5;
		float cube_error = tx;

		if(tv == 0)
		{
			distance_error = 0;
			auto_strafe = strafe;
			auto_drive = 0;
			auto_turn = 0;
		}
		else
		{
			auto_drive = distance_error * DriveTrain->Drive_P;
			auto_turn = 0; //cube_error * DriveTrain->Gyro_P;
			auto_strafe = cube_error * DriveTrain->Strafe_P;
		}

		DriveTrain->SetRawForwardSpeed(auto_drive);
		DriveTrain->SetRawTurnSpeed(auto_turn);
		DriveTrain->SetRawStrafeSpeed(auto_strafe);
*/
		if(IntakeTimer->Get() > time)
		{
			gotcube = true;
		}

		if(Claw->GotCube())
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

		if (counter > 50)
		{
			printf("Got Cube !!!!");
			gotcube = true;
		}
	}
	DriveTrain->SetRawForwardSpeed(0);
	DriveTrain->SetRawTurnSpeed(0);
	DriveTrain->SetRawStrafeSpeed(0);
}

void Auton::Auto_TRACKSWITCH(float strafe,float heading,float time)
{
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	DriveTrain->headingTarget = heading;
/*
	DriveTrain->SetRawStrafeSpeed(strafe);

	// strafe until we see the cube
	bool saw_switch = false;
	while(!saw_switch && (Running()))
	{
		float tx = table->GetNumber("tx",0);
		float ty = 0;
		float tv = table->GetNumber("tv",0);

		auto_tx = tx;
		auto_ty = ty;
		saw_switch = ((tv != 0) && (fabs(tx) < 3.0));
		Auto_System_Update();
	}
*/
	// Now go to Switch
	bool done = false;

	IntakeTimer->Reset();
	IntakeTimer->Start();

	while(done == false && (Running()))
	{
		Auto_System_Update();
		float tv  = table->GetNumber("tv", 0);

		float tx = 0;
		float ty = 0;

		if(tv != 0)
		{
			auto_tx = tx;
			auto_ty = ty;

			tx = table->GetNumber("tx", 0);
			ty = table->GetNumber("ty", 0);
		}

		float distance_error = ty;
		float cube_error = tx;

		if(tv == 0)
		{
			distance_error = 0;
			auto_strafe = strafe;
			auto_drive = 0;
			auto_turn = 0;
		}
		else
		{
			auto_drive = 0.4f; //distance_error * DriveTrain->Drive_P;
			auto_turn = 0; //cube_error * DriveTrain->Gyro_P;
			auto_strafe = (cube_error * DriveTrain->Strafe_P)/2;
		}

		DriveTrain->SetRawForwardSpeed(auto_drive);
		DriveTrain->SetRawTurnSpeed(auto_turn);
		DriveTrain->SetRawStrafeSpeed(auto_strafe);

		if(IntakeTimer->Get() > time)
		{
			done = true;
		}

		if((fabs(distance_error) < 1.5) && (tv == 1))
		{
			done = true;
		}
	}
	DriveTrain->SetRawForwardSpeed(0);
	DriveTrain->SetRawTurnSpeed(0);
	DriveTrain->SetRawStrafeSpeed(0);
}
void Auton::Auto_SETPIPELINE(float pipeline)
{
	int pipe = (int) pipeline;

	if(pipe >= 0 && pipe < 10)
	{
		std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

		table->PutNumber("pipeline", pipe);
	}
}
void Auton::Auto_SETLIGHTS(float mode)
{
	int light = (int) mode;

	if(light >= 0 && light < 3)
	{
		std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

		table->PutNumber("ledMode", light);
	}
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
	float Sonar_error = SonarMaintain - DriveTrain->GetActiveSonar();
	float forward = Sonar_error * Strafe_P;

	if(ticks > 0)
	{
		while((-DriveTrain->GetLeftEncoder() < ticks)&&(Running()))
		{
			Sonar_error = SonarMaintain - DriveTrain->GetActiveSonar();
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

			Sonar_error = SonarMaintain - DriveTrain->GetActiveSonar();
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
	float Sonar_error = SonarMaintain - DriveTrain->GetActiveSonar();
	float strafe = Sonar_error * Strafe_P;

	while((fabs(Sonar_error) < 50) && (Running()))
	{
		Sonar_error = SonarMaintain - DriveTrain->GetActiveSonar();
		strafe = Sonar_error * Strafe_P;

		DriveTrain->SetRawForwardSpeed(Forward);
		DriveTrain->SetRawStrafeSpeed(strafe);
		DriveTrain->SetRawTurnSpeed(0);
		DriveTrain->EnableGyroCorrection(true);

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

		LiftManager->UpdateLift(false,false,false,false,false,false,false,false,false,false);

		DriveTrain->AutoUpdate();

		Claw->Auto_Update();

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

		MyRobotClass::Get()->Send_Data();
		SmartDashboard::PutNumber("AUTOTIMER",AutonTimer->Get());
		SmartDashboard::PutNumber("Auto Tx",auto_tx);
		SmartDashboard::PutNumber("Auto Ty",auto_ty);
		SmartDashboard::PutNumber("Auto Drive",auto_drive);
		SmartDashboard::PutNumber("Auto Turn",auto_turn);
		SmartDashboard::PutNumber("Auto Strafe",auto_strafe);
	}
}
void Auton::Auto_Intake_In(float value)
{
	Claw->Claw_Intake(value);
}
void Auton::Auto_Intake_Out(float value)
{
	Claw->Claw_Outake(value);
}
void Auton::Auto_Intake_Off()
{
	Claw->Claw_Off();
}
void Auton::Auto_DriveEncoder(float Forward, float Ticks)
{
	DriveTrain->ResetEncoders_Timers();
	DriveTrain->SetForwardTarget(Ticks,Forward);

	bool complete = DriveTrain->reachedForwardEncoderTarget;

	while((!complete) && (Running()))
	{
		Auto_System_Update();
		complete = DriveTrain->reachedForwardEncoderTarget;
	}
	DriveTrain->SetRawForwardSpeed(0);
}
