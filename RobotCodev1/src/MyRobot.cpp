/*
 * MyRobot.cpp
 *
 *  Created on: Jan 9, 2018
 *      Author: 987
 */

#include "MyRobot.h"
#include "networktables/NetworkTable.h"

using namespace frc;

MyRobotClass * MyRobotClass::TheRobot = NULL;

MyRobotClass::MyRobotClass()
{
	TheRobot = this;

	Drivetrain = new Drivetrainclass();
	Arm = new ArmClass();
	Elevator = new ElevatorClass();
	Claw = new ClawClass();
	LiftManager = new LiftManagerClass(Elevator, Arm, Claw);
	AutonomousControl = new Auton(Drivetrain, &DriverStation::GetInstance(),Claw,LiftManager);
	Conveyor = new ConveyorClass();
	PDP = new PowerDistributionPanel();

	forwardcommand = 0;
	turncommand = 0;
	strafecommand = 0;
	turnbias = 0;

	leftStick = new Joystick(0);
	rightStick = new Joystick(1);
	turretStick = new Joystick(2);

	isTracking = false;

	gyromode = GYRO_CORRECTION_ON;
	brakemode = BRAKE_MODE_OFF;

	turndelay = new Timer();
	turndelay->Reset();
	turndelay->Start();

	m_ScriptSystem = 0;
	Init_Scripts_System();
}

MyRobotClass::~MyRobotClass() {
	TheRobot = NULL;
}
void MyRobotClass::Disabled(void)
{
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	while(IsDisabled())
	{
		table->PutNumber("ledMode", 1);
		//Arm->ArmPIDController->Disable();
	}
}

void MyRobotClass::Autonomous(void)
{
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	table->PutNumber("ledMode", 1);

	Load_Scripts();
	printf("loaded\n");
	m_ScriptSystem->Run_Auto_Script(0);
	printf("ransettings\n");
	AutonomousControl->Auto_Start();
	printf("startfunction\n");
	m_ScriptSystem->Run_Auto_Script(Auto_Index);
	AutonomousControl->Auto_End();
}
void MyRobotClass::UpdateInputs()
{
	forwardcommand = -leftStick->GetY();
	strafecommand = leftStick->GetX();
	turncommand = rightStick->GetX();

	if (leftStick->GetZ() < 0)
	{
		gyromode = GYRO_CORRECTION_OFF;
	}
	else
	{
		gyromode = GYRO_CORRECTION_ON;
	}


	if(fabs(forwardcommand) < .1)
	{
		forwardcommand = 0;
	}
	if(fabs(turncommand) < .1)
	{
		turncommand = 0;
	}
	if(fabs(strafecommand) < .1)
	{
		strafecommand = 0;
	}

	//limelight aiming
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	float targetX = table->GetNumber("tx", 0);
	float targetA = table->GetNumber("ta", 0);
	float targetV = table->GetNumber("tv", 0);


	/*if(fabs(targetX) > 1)
	{
		steering_adjust = -0.015f * targetX;

		//min
		if(targetX > 0.0f)
		{
			steering_adjust -= 0.1f;
		}
		else
		{
			steering_adjust += 0.1f;
		}
	}*/

	if(leftStick->GetRawButton(5))
	{
		Preferences *prefs = Preferences::GetInstance();

		float track_area = prefs->GetDouble("Track_Area",50);
		float track_kforward = prefs->GetDouble("Track_kForward",.025f);
		float track_kturn = prefs->GetDouble("Track_kTurn",-.025f);
		float track_kstrafe = prefs->GetDouble("Track_kStrafe",-.030f);

		float steering_adjust = track_kturn * targetX;
		float strafe_adjust = track_kstrafe * targetX;

		isTracking = true;
		if(targetV)
		{
			forwardcommand = (track_area-targetA)*track_kforward;

			float transition_range = prefs->GetDouble("Transition_Range", 20);

			if(fabs(targetX) > transition_range)
			{
				turnbias = .7;
			}
			else
			{
				turnbias = .3;
			}
			turncommand = turnbias * -steering_adjust;
			strafecommand = (1 - turnbias) * strafe_adjust;
		}
	}
	else
	{
		isTracking = false;
	}

	if(leftStick->GetRawButton(11))
		{
		int newNum = rand() % 4 + 1; // number 1 through 4
		SmartDashboard::PutNumber("Random Number", newNum);
		}

}
void MyRobotClass::Send_Data()
{
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	SmartDashboard::PutNumber("Target X",table->GetNumber("tx",0));
	SmartDashboard::PutNumber("Target y",table->GetNumber("ty",0));

	SmartDashboard::PutBoolean("Is Tracking", isTracking);
	SmartDashboard::PutString("Game Data", DriverStation::GetInstance().GetGameSpecificMessage());

	Drivetrain->Send_Data();
	AutonomousControl->SendData();
	Elevator->Send_Data();
	Arm->Send_Data();
	LiftManager->Send_Data();
}
void MyRobotClass::OperatorControl(void)
{
	Load_Scripts();
	m_ScriptSystem->Run_Auto_Script(0);

	Drivetrain->Zero_Yaw();

	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	table->PutNumber("ledMode", 1);

	LiftManager->changeMode(LiftMode::Free);

	while(IsOperatorControl() && IsEnabled())
	{
		Preferences *prefs = Preferences::GetInstance();

		UpdateInputs();

		Send_Data();

		if(fabs(turncommand) > 0)
		{
			turndelay->Reset();
			turndelay->Start();
		}

		if((turncommand != 0 || isTracking || turndelay->Get() < prefs->GetDouble("GyroTurn_Delay", 1)))
		{
			gyromode = GYRO_CORRECTION_OFF;
		}
		else
		{
			if(leftStick->GetRawButton(3))
			{
				gyromode = GYRO_CORRECTION_ON;
			}
			else
			{
				gyromode = GYRO_CORRECTION_OFF;
			}
		}
		if(leftStick->GetRawButton(2))
		{
			brakemode = BRAKE_MODE_ON;
		}
		else
		{
			brakemode = BRAKE_MODE_OFF;
		}

		Drivetrain->StandardArcade(forwardcommand, turncommand, strafecommand, gyromode, brakemode);

		if(turretStick->GetRawButton(11))
		{
			Arm->WristTalon->SetSelectedSensorPosition(Wrist_Intake,0,1.0);
		}

		Elevator->Update(turretStick->GetX());
		Arm->Update(turretStick->GetY(),turretStick->GetRawButton(4),turretStick->GetRawButton(5));

		Claw->Update((turretStick->GetTrigger() || rightStick->GetTrigger()),rightStick->GetRawButton(2), turretStick->GetRawButton(2), turretStick->GetRawButton(3));

		LiftManager->UpdateLift((rightStick->GetRawButton(4) || turretStick->GetRawButton(6)),rightStick->GetRawButton(3)
				,turretStick->GetRawButton(7),turretStick->GetRawButton(8),turretStick->GetRawButton(9),leftStick->GetRawButton(6));

		Conveyor->Update(rightStick->GetRawButton(11), rightStick->GetRawButton(10));

		Wait(0.005);
	}
}

START_ROBOT_CLASS(MyRobotClass)
