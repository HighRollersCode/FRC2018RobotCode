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

	Conveyor = new ConveyorClass();
	Drivetrain = new Drivetrainclass(Conveyor->Conveyor);
	Arm = new ArmClass();
	Elevator = new ElevatorClass();
	Claw = new ClawClass();
	Endgame = new EndgameClass();

	LiftManager = new LiftManagerClass(Elevator, Arm, Claw,Endgame);

	AutonomousControl = new Auton(Drivetrain, &DriverStation::GetInstance(),Claw,LiftManager);
	PDP = new PowerDistributionPanel();

	Comp = new Compressor(16);
	Comp->SetClosedLoopControl(true);

	forwardcommand = 0;
	turncommand = 0;
	strafecommand = 0;
	turnbias = 0;

	tracking_cur = 0;
	tracking_prev = 0;

	leftStick = new Joystick(0);
	rightStick = new Joystick(1);
	turretStick = new Joystick(2);

	isTracking = false;

	gyromode = GYRO_CORRECTION_ON;
	brakemode = BRAKE_MODE_OFF;

	turndelay = new Timer();
	turndelay->Reset();
	turndelay->Start();

	matchTimer = new Timer();
	matchTimer->Reset();

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
		matchTimer->Stop();
		matchTimer->Reset();
		table->PutNumber("ledMode", 1);
		//Arm->ArmPIDController->Disable();
		Wait(0.005);
	}
}

void MyRobotClass::Autonomous(void)
{
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	table->PutNumber("ledMode", 0);
	Drivetrain->Shifter_High();

	Load_Scripts();
	printf("loaded\n");
	m_ScriptSystem->Run_Auto_Script(0);
	printf("ransettings\n");
	AutonomousControl->Auto_Start();
	printf("startfunction\n");

	//Set Auto Index based on field
	std::string message = DriverStation::GetInstance().GetGameSpecificMessage();

	while(message.length() < 2)
	{
		message = DriverStation::GetInstance().GetGameSpecificMessage();
	}

	if(message[0] == 'R' && message[1] == 'R')
	{
		Auto_Index = Auto_Index_RR;
	}
	else if(message[0] == 'L' && message[1] == 'L')
	{
		Auto_Index = Auto_Index_LL;
	}
	else if(message[0] == 'L' && message[1] == 'R')
	{
		Auto_Index = Auto_Index_LR;
	}
	else if(message[0] == 'R' && message[1] == 'L')
	{
		Auto_Index = Auto_Index_RL;
	}

	Preferences *prefs = Preferences::GetInstance();
	/*int script = prefs->GetInt("Auto Scripts", 0);
	if(script != 0)
	{
		Auto_Index = script;
	}*/
	m_ScriptSystem->Run_Auto_Script(Auto_Index);
	AutonomousControl->Auto_End();

	table->PutNumber("ledMode", 0);
}
void MyRobotClass::UpdateInputs()
{
	forwardcommand = -Forward_Command; //leftStick->GetY();
	strafecommand = Strafe_Command; //leftStick->GetX();
	turncommand = Turn_Command; //rightStick->GetX();

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

	tracking_prev = tracking_cur;
	tracking_cur = Track_Enable;

	if(Track_Enable)
	{
		table->PutNumber("pipeline", 0);
		float tx = table->GetNumber("tx", 0);
		float ty = table->GetNumber("ty", 0);

		float distance_error = ty + 5;
		float cube_error = tx;

		float tracking_drive = distance_error * Drivetrain->Drive_P;
		float tracking_turn = 0; //cube_error * DriveTrain->Gyro_P;
		float tracking_strafe = cube_error * Drivetrain->Strafe_P;

		forwardcommand += tracking_drive;
		turncommand += tracking_turn;
		strafecommand += tracking_strafe;
	}
	else
	{
		table->PutNumber("pipeline", 4);
		isTracking = false;
	}

	if(Random_Number)
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
	SmartDashboard::PutNumber("Pressure Sensor", Comp->GetPressureSwitchValue());
	SmartDashboard::PutNumber("Match Timer", matchTimer->Get());

	Drivetrain->Send_Data();
	AutonomousControl->SendData();
	Elevator->Send_Data();
	Arm->Send_Data();
	LiftManager->Send_Data();
	Claw->Send_Data();
}
void MyRobotClass::OperatorControl(void)
{
	Load_Scripts();
	m_ScriptSystem->Run_Auto_Script(0);

	Drivetrain->Zero_Yaw();

	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("limelight");

	table->PutNumber("ledMode", 0);
	table->PutNumber("pipeline", 4);

	Arm->PIDOff();
	Elevator->PIDOff();

	LiftManager->changeMode(LiftMode::Free);

	Comp->SetClosedLoopControl(true);

	matchTimer->Start();

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
			if(Gyro_Correction_Enable)
			{
				gyromode = GYRO_CORRECTION_ON;
			}
			else
			{
				gyromode = GYRO_CORRECTION_OFF;
			}
		}
		/*if(Brake_Mode_Enable)
		{
			brakemode = BRAKE_MODE_ON;
		}
		else
		{
			brakemode = BRAKE_MODE_OFF;
		}*/

		Drivetrain->StandardArcade(forwardcommand, turncommand, strafecommand, gyromode, brakemode);
		Drivetrain->Shifter_Update(leftStick->GetTrigger());

		Endgame->Update(Deploy_Lock,Deploy_Claw,matchTimer->Get());

		if(Enable_Elevator)
		{
			Elevator->Update(Elevator_Command);
			Arm->Update(0,Wrist_Up_Command,Wrist_Down_Command);
		}
		else
		{
			Elevator->Update(0);
			Arm->Update(Arm_Command,Wrist_Up_Command,Wrist_Down_Command);
		}

		//Elevator->Update(Elevator_Command);
		//Arm->Update(Arm_Command,Wrist_Up_Command,Wrist_Down_Command);

		Claw->Update(Intake_Command, Outake_Command, Turret_Outake,
				Turret_Slow_Outake,Track_Enable);

		LiftManager->UpdateLift(Intake_Mode_Preset,Switch_Mode_Preset ,Scale_Front_Level_1_Preset,
				Scale_Back_Preset,Scale_Neutral_Preset,Set_Up_Preset,Scale_Back_Lob_Preset,false,Portal_Preset); //Climb_Preset,Portal_Preset);

		Conveyor->Update(Conveyor_Left, Conveyor_Right);

		if(Reset_Encoders)
		{
			Arm->ResetEncoder();
			Elevator->ResetElevatorEncoder();
		}

		Wait(0.005);
	}
}

START_ROBOT_CLASS(MyRobotClass)
