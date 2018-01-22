/*
 * ScriptSupport.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: 987
 */
#include "MyRobot.h"
#include "HRscript.h"
#include "Defines.h"
//#include "HRLogger.h"


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
		MyRobotClass::Get()->AutonomousControl->AutonWait2(m_Parameters[0],(int)m_Parameters[1]);
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
class DriveTicksCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTicks"; }
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveTicksCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_DriveEncoder(m_Parameters[0], m_Parameters[1],m_Parameters[2]);
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

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Tracking Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class SearchForCubeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "SearchForCubeStrafe"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new SearchForCubeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_SEARCHFORCUBE(m_Parameters[0],m_Parameters[1]);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Intake Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

/*class SetIntakeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "Intake"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetIntakeCommand(); }
	virtual void Execute()
	{
		if(m_Parameters[0] == 1)
		{
			MyRobotClass::Get()->AutonomousControl->Auto_Intake_On();
		}
		else if (m_Parameters[0] == 0)
		{
			MyRobotClass::Get()->AutonomousControl->Auto_Intake_Off();
		}
		else if (m_Parameters[0] == -1)
		{
			MyRobotClass::Get()->Intake->IntakeOut();
		}
	}
};*/

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

void MyRobotClass::Init_Scripts_System()
{
	m_ScriptSystem = new HrScriptSystemClass();
	printf("Script Initializing \r\n");
	// register commands with the system
	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new WaitCommand1());
	m_ScriptSystem->Add_Command(new WaitForBrakeCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new DisableBrakeCommand());
	m_ScriptSystem->Add_Command(new DriveTimeCommand());
	m_ScriptSystem->Add_Command(new DriveHeadingTicksCommand());
	m_ScriptSystem->Add_Command(new DriveTicksHeadingDistanceCommand());
	m_ScriptSystem->Add_Command(new DriveHeadingDistanceCommand());
	m_ScriptSystem->Add_Command(new DriveStrafeTicksCommand());
	m_ScriptSystem->Add_Command(new DriveTicksCommand());
	m_ScriptSystem->Add_Command(new DriveCommand());
	m_ScriptSystem->Add_Command(new GyroTurnCommand());
	m_ScriptSystem->Add_Command(new GyroTurnTimeCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SearchForCubeCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SetAutoCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	Load_Scripts();

}

void MyRobotClass::Load_Scripts()
{
	// Load and run the robot settings script:
	m_ScriptSystem->Load_And_Run_Script("RobotSettings.hrs");

	// Load all of the auto-mode scripts
	m_ScriptSystem->Set_Auto_Script(111,"MIDDLERIGHTSWITCHANDSCALE.hrs");

	//EmptyAuto
	m_ScriptSystem->Set_Auto_Script(999,"EMPTY.hrs");

	//Settings
	m_ScriptSystem->Set_Auto_Script(0,"RobotSettings.hrs");
}
