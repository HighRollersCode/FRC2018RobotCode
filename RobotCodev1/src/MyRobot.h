/*
 * MyRobot.h
 *
 *  Created on: Jan 9, 2018
 *      Author: 987
 */

#ifndef SRC_MYROBOT_H_
#define SRC_MYROBOT_H_

#include "WPILib.h"

#include "Drivetrain.h"
#include "Elevator.h"
#include "Arm.h"
#include "Claw.h"
#include "Defines.h"
#include "Auton.h"
#include "LiftManager.h"
#include "Conveyor.h"
#include "Endgame.h"

#include "HRscript.h"

class MyRobotClass: public SampleRobot {
public:

	float forwardcommand;
	float turncommand;
	float strafecommand;
	float turnbias;

	float tracking_cur;
	float tracking_prev;

	eGyroMode gyromode;
	eBrakeMode brakemode;

	Joystick *leftStick;
	Joystick *rightStick;
	Joystick *turretStick;

	Drivetrainclass *Drivetrain;
	ArmClass *Arm;
	ElevatorClass *Elevator;
	ClawClass *Claw;
	Auton *AutonomousControl;
	LiftManagerClass *LiftManager;
	ConveyorClass *Conveyor;
	EndgameClass *Endgame;

	PowerDistributionPanel *PDP;

	Compressor *Comp;

	bool isTracking;

	Timer *turndelay;
	Timer *matchTimer;

	MyRobotClass();
	virtual ~MyRobotClass();

	static MyRobotClass * Get() { return TheRobot; }

	void Disabled(void);
	void Autonomous(void);
	void UpdateInputs();
	void OperatorControl(void);
	void Send_Data();
	void Init_Scripts_System();
	void Load_Scripts();

	int Auto_Index = 0;

	int Auto_Index_LL = 0;

	int Auto_Index_RR = 0;

	int Auto_Index_LR = 0;

	int Auto_Index_RL = 0;


protected:

	HrScriptSystemClass * m_ScriptSystem;
	static MyRobotClass * TheRobot;

};

#endif /* SRC_MYROBOT_H_ */
