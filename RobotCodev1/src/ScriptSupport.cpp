/*
 * ScriptSupport.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: 987
 */
#include "MyRobot.h"
#include "LiftManager.h"
#include "HRscript.h"
#include "Defines.h"


// Create commands for the robot scripts

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Settings Code//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

class SetDriveTrainConstantsCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTrainSettings"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetDriveTrainConstantsCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Drivetrain->Gyro_P = (float)m_Parameters[0];
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Wait Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class WaitCommand1 : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "Wait"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new WaitCommand1(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->AutonWait(m_Parameters[0]);
	}
};
class WaitForBrakeCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "BrakeWait"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new WaitForBrakeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->EBrake(m_Parameters[0],(int)m_Parameters[1]);
	}
};
class WaitForTransitionCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "WaitForTransition"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new WaitForTransitionCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->AutonWaitForTransition();
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Driving Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class DisableBrakeCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DisableBrake"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new DisableBrakeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->AutonWait2(m_Parameters[0],(int)m_Parameters[1]);
	}
};
class DriveTimeCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTimed"; }
	virtual int Get_Parameter_Count() { return 4; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveTimeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_DriveTimer(m_Parameters[0], m_Parameters[1],m_Parameters[2],m_Parameters[3]);
	}
};
class DriveCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "Drive"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Drivetrain->StandardArcade(m_Parameters[0], m_Parameters[1], 0, GYRO_CORRECTION_OFF, BRAKE_MODE_OFF);
	}
};
class DriveHeadingTicksCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTicksHeading"; }
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveHeadingTicksCommand(); }
	virtual void Execute()
	{

		MyRobotClass::Get()->AutonomousControl->Auto_GYROSTRAIGHT(m_Parameters[0], m_Parameters[1],m_Parameters[2]);
	}
};
class DriveTicksHeadingDistanceCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTicksHeadingDistance"; }
	virtual int Get_Parameter_Count() { return 4; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveTicksHeadingDistanceCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_GYROSTRAIGHTSONAR(m_Parameters[0],m_Parameters[1],m_Parameters[2],m_Parameters[3]);
	}
};
class DistanceHeadingCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DistanceHeading"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new DistanceHeadingCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_SONAR(m_Parameters[0],m_Parameters[1]);
	}
};
class DriveHeadingDistanceCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveHeadingDistance"; }
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveHeadingDistanceCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_FOLLOWEDGE(m_Parameters[0],m_Parameters[1],m_Parameters[2]);
	}
};
class DriveStrafeTicksCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTicksStrafeHeading"; }
	virtual int Get_Parameter_Count() { return 5; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveStrafeTicksCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_GYROSTRAFE(m_Parameters[0],m_Parameters[1],m_Parameters[2],m_Parameters[3],m_Parameters[4]);
	}
};
class StrafeTicksCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "StrafeTicksHeading"; }
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new StrafeTicksCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_STRAFE(m_Parameters[0],m_Parameters[1],m_Parameters[2]);
	}
};
class StrafeUntilCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "StrafeHeadingDistance"; }
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new StrafeUntilCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_STRAFEUNTIL(m_Parameters[0],m_Parameters[1],m_Parameters[2]);
	}
};
class DriveTicksCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTicks"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveTicksCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_DriveEncoder(m_Parameters[0],m_Parameters[2]);
	}
};
class GyroTurnCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "GyroTurn"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new GyroTurnCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_GYROTURN(m_Parameters[0]);
	}
};
class GyroTurnTimeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "GyroTurnTimed"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new GyroTurnTimeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_GYROTURN_TIMED(m_Parameters[0],m_Parameters[1]);
	}
};

class SetActiveSonarCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetActiveSonar"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetActiveSonarCommand(); }
	virtual void Execute()
	{
		if(m_Parameters[0] == 0)
		{
			MyRobotClass::Get()->Drivetrain->SetActiveSonar(eSonar::LEFT_SONAR);
		}
		else if(m_Parameters[0] == 1)
		{
			MyRobotClass::Get()->Drivetrain->SetActiveSonar(eSonar::RIGHT_SONAR);
		}
		else if(m_Parameters[0] == 2)
		{
			MyRobotClass::Get()->Drivetrain->SetActiveSonar(eSonar::FRONT_SONAR);
		}
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Tracking Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class SearchForCubeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "SearchForCubeStrafeTimed"; }
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new SearchForCubeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_SEARCHFORCUBESTRAFE(m_Parameters[0],m_Parameters[1],m_Parameters[2]);
	}
};

class SearchForCubeTurnCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "SearchForCubeTurnTimed"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new SearchForCubeTurnCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_SEARCHFORCUBETURN(m_Parameters[0],m_Parameters[1]);
	}
};

class TrackSwitchCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "StrafeTrackToSwitch"; }
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new TrackSwitchCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_TRACKSWITCH(m_Parameters[0],m_Parameters[1],m_Parameters[2]);
	}
};
class SearchForCubeForwardCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "SearchForCubeForward"; }
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new SearchForCubeForwardCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_SEARCHFORCUBEFORWARD(m_Parameters[0],m_Parameters[1],m_Parameters[2]);
	}
};
class SetPipelineCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "SetPipeline"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetPipelineCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_SETPIPELINE(m_Parameters[0]);
	}
};

class SetLightsCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "SetLights"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetLightsCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_SETLIGHTS(m_Parameters[0]);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Intake Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class SetIntakeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "Intake"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetIntakeCommand(); }
	virtual void Execute()
	{
		if(m_Parameters[0] < 0)
		{
			MyRobotClass::Get()->AutonomousControl->Auto_Intake_Out(m_Parameters[0]);
		}
		else if(m_Parameters[0] > 0)
		{
			MyRobotClass::Get()->AutonomousControl->Auto_Intake_In(m_Parameters[0]);
		}
		else
		{
			MyRobotClass::Get()->AutonomousControl->Auto_Intake_Off();
		}
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Elevator Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class SetElevatorCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "SetElevator"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetElevatorCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Elevator->SetElevatorTarg(m_Parameters[0]);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Elevator Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class SetWristCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "SetWrist"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetWristCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Arm->SetWristTarg(m_Parameters[0]);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Conveyor Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class SetConveyorCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "Conveyor"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetConveyorCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->SetConveyorSpeed(m_Parameters[0]);
	}
};

class SetAutoTriggerConveyorCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "AutoTriggerConveyor"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new SetAutoTriggerConveyorCommand(); }
	virtual void Execute()
	{

		MyRobotClass::Get()->AutonomousControl->auto_Conveyor = m_Parameters[0];
		MyRobotClass::Get()->AutonomousControl->auto_Conveyor_Distance = m_Parameters[1];

	}
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Mode Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class SetIntakeModeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "IntakeMode"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new SetIntakeModeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->LiftManager->changeMode(LiftMode::Intake_Down);
	}
};

class SetScaleBackModeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "ScaleBackMode"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new SetScaleBackModeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->LiftManager->changeMode(LiftMode::Scale_Winning_Back_Level1);
	}
};

class SetScaleBackLobModeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "ScaleBackLobMode"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new SetScaleBackLobModeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->LiftManager->changeMode(LiftMode::Scale_Back_Lob);
	}
};

class SetSwitchModeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "SwitchMode"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new SetSwitchModeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->LiftManager->changeMode(LiftMode::Auto_Switch);
	}
};

class SetTeleopSwitchModeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "TeleopSwitchMode"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new SetTeleopSwitchModeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->LiftManager->changeMode(LiftMode::Intake);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Support Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class SetAutoCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetAuto"; }
	virtual int Get_Parameter_Count() { return 1; }

	virtual HrScriptCommandClass * Create_Command() { return new SetAutoCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Auto_Index = (int)m_Parameters[0];
		printf("Set Up Autos");
	}
};

class SetAutoRRRCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetAutoRR"; }
	virtual int Get_Parameter_Count() { return 1; }

	virtual HrScriptCommandClass * Create_Command() { return new SetAutoRRRCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Auto_Index_RR = (int)m_Parameters[0];
		printf("Set Up Autos");
	}
};
class SetAutoLLLCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetAutoLL"; }
	virtual int Get_Parameter_Count() { return 1; }

	virtual HrScriptCommandClass * Create_Command() { return new SetAutoLLLCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Auto_Index_LL = (int)m_Parameters[0];
		printf("Set Up Autos");
	}
};
class SetAutoLRLCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetAutoLR"; }
	virtual int Get_Parameter_Count() { return 1; }

	virtual HrScriptCommandClass * Create_Command() { return new SetAutoLRLCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Auto_Index_LR = (int)m_Parameters[0];
		printf("Set Up Autos");
	}
};
class SetAutoRLRCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetAutoRL"; }
	virtual int Get_Parameter_Count() { return 1; }

	virtual HrScriptCommandClass * Create_Command() { return new SetAutoRLRCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Auto_Index_RL = (int)m_Parameters[0];
		printf("Set Up Autos");
	}
};

void MyRobotClass::Init_Scripts_System()
{
	m_ScriptSystem = new HrScriptSystemClass();
	printf("Script Initializing \r\n");
	// register commands with the system
	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new WaitCommand1());
	m_ScriptSystem->Add_Command(new WaitForBrakeCommand());
	m_ScriptSystem->Add_Command(new WaitForTransitionCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new DisableBrakeCommand());
	m_ScriptSystem->Add_Command(new DriveTimeCommand());
	m_ScriptSystem->Add_Command(new DriveHeadingTicksCommand());
	m_ScriptSystem->Add_Command(new DriveTicksHeadingDistanceCommand());
	m_ScriptSystem->Add_Command(new DriveHeadingDistanceCommand());
	m_ScriptSystem->Add_Command(new DistanceHeadingCommand());
	m_ScriptSystem->Add_Command(new DriveStrafeTicksCommand());
	m_ScriptSystem->Add_Command(new StrafeUntilCommand());
	m_ScriptSystem->Add_Command(new StrafeTicksCommand());
	m_ScriptSystem->Add_Command(new DriveTicksCommand());
	m_ScriptSystem->Add_Command(new DriveCommand());
	m_ScriptSystem->Add_Command(new GyroTurnCommand());
	m_ScriptSystem->Add_Command(new GyroTurnTimeCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SetActiveSonarCommand());
	m_ScriptSystem->Add_Command(new SearchForCubeCommand());
	m_ScriptSystem->Add_Command(new SearchForCubeTurnCommand());
	m_ScriptSystem->Add_Command(new SearchForCubeForwardCommand());
	m_ScriptSystem->Add_Command(new TrackSwitchCommand());
	m_ScriptSystem->Add_Command(new SetPipelineCommand());
	m_ScriptSystem->Add_Command(new SetLightsCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SetIntakeCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SetElevatorCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SetWristCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SetConveyorCommand());
	m_ScriptSystem->Add_Command(new SetAutoTriggerConveyorCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SetIntakeModeCommand());
	m_ScriptSystem->Add_Command(new SetScaleBackModeCommand());
	m_ScriptSystem->Add_Command(new SetScaleBackLobModeCommand());
	m_ScriptSystem->Add_Command(new SetSwitchModeCommand());
	m_ScriptSystem->Add_Command(new SetTeleopSwitchModeCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SetAutoCommand());
	m_ScriptSystem->Add_Command(new SetAutoRRRCommand());
	m_ScriptSystem->Add_Command(new SetAutoLLLCommand());
	m_ScriptSystem->Add_Command(new SetAutoLRLCommand());
	m_ScriptSystem->Add_Command(new SetAutoRLRCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	Load_Scripts();

}

void MyRobotClass::Load_Scripts()
{
	// Load and run the robot settings script:
	m_ScriptSystem->Load_And_Run_Script("RobotSettings.hrs");

	// Load all of the auto-mode scripts

	//Scale and Switch
	m_ScriptSystem->Set_Auto_Script(11,"MIDDLERIGHTSWITCHANDSCALE.hrs");
	m_ScriptSystem->Set_Auto_Script(12,"MIDDLELEFTSWITCHANDSCALE.hrs");
	m_ScriptSystem->Set_Auto_Script(13,"MIDDLELEFTSWITCHANDRIGHTSCALETURN.hrs");
	m_ScriptSystem->Set_Auto_Script(14,"MIDDLERIGHTSWITCHANDLEFTSCALETURN.hrs");

	//Only Scale Right Start
	m_ScriptSystem->Set_Auto_Script(21,"RIGHTALLSCALERIGHT.hrs");
	m_ScriptSystem->Set_Auto_Script(23,"RIGHTALLSCALELEFT.hrs");

	//Only Scale Left Start
	m_ScriptSystem->Set_Auto_Script(22,"LEFTALLSCALERIGHT.hrs");
	m_ScriptSystem->Set_Auto_Script(24,"LEFTALLSCALELEFT.hrs");
	m_ScriptSystem->Set_Auto_Script(26,"LEFTALLSCALELEFTCOOP.hrs");

	//Only Switch Coop
	m_ScriptSystem->Set_Auto_Script(31,"LEFTLEFTSWITCHCOOP.hrs");
	m_ScriptSystem->Set_Auto_Script(32,"RIGHTRIGHTSWITCHCOOP.hrs");

	//Pyramid
	m_ScriptSystem->Set_Auto_Script(41,"MIDDLEPYRAMIDRIGHT.hrs");
	m_ScriptSystem->Set_Auto_Script(42,"MIDDLEPYRAMIDLEFT.hrs");
	m_ScriptSystem->Set_Auto_Script(43,"MIDDLEPYRAMIDLOBLEFT.hrs");
	m_ScriptSystem->Set_Auto_Script(44,"MIDDLEPYRAMIDLOBRIGHT.hrs");
	m_ScriptSystem->Set_Auto_Script(45,"MIDDLEPYRAMIDLOBLEFTSCALE.hrs");
	m_ScriptSystem->Set_Auto_Script(46,"MIDDLEPYRAMIDLOBRIGHTSCALE.hrs");


	//Switch and Scale No Backpack
	m_ScriptSystem->Set_Auto_Script(51,"MIDDLERIGHTSWITCHANDSCALENOPACK.hrs");
	m_ScriptSystem->Set_Auto_Script(52,"MIDDLELEFTSWITCHANDSCALENOPACK.hrs");
	m_ScriptSystem->Set_Auto_Script(53,"MIDDLELEFTSWITCHANDRIGHTSCALETURNNOPACK.hrs");
	m_ScriptSystem->Set_Auto_Script(54,"MIDDLERIGHTSWITCHANDLEFTSCALETURNNOPACK.hrs");

	//Spare Autos
	m_ScriptSystem->Set_Auto_Script(115,"MIDDLELEFTSWITCHANDRIGHTSCALE.hrs");

	m_ScriptSystem->Set_Auto_Script(113,"MIDDLEALLSCALERIGHT.hrs");

	//EmptyAuto
	m_ScriptSystem->Set_Auto_Script(999,"EMPTY.hrs");

	//Settings
	m_ScriptSystem->Set_Auto_Script(0,"RobotSettings.hrs");
}
