/*
 * LiftManager.h
 *
 *  Created on: Jan 27, 2018
 *      Author: 987
 */

#ifndef SRC_LIFTMANAGER_H_
#define SRC_LIFTMANAGER_H_

#include "WPILib.h"
#include "Defines.h"

#include "Claw.h"
#include "Elevator.h"
#include "Arm.h"
#include "EndGame.h"

enum LiftMode
{
	Free = 0,
	Intake,
	Intake_Down,
	Scale_Losing_Level_Front,
	Scale_Neutral_Front,
	Scale_Winning_Back_Level1,
	Set_Up,
	Reset,
	Climb,
	Scale_Back_Lob,
	Portal,
	Auto_Switch
};
class LiftManagerClass {
public:
	LiftManagerClass(ElevatorClass *_Elevator, ArmClass *_Arm, ClawClass *_Claw, EndgameClass *_Endgame);
	virtual ~LiftManagerClass();

ElevatorClass *_elevator;
ArmClass *_arm;
ClawClass *_claw;
EndgameClass *_endgame;

LiftMode CurrentLiftMode;

bool IntakeState_Cur = false;
bool IntakeState_Prev = false;

bool IntakeDownState_Cur = false;
bool IntakeDownState_Prev = false;

bool ScaleLevel1State_Cur = false;
bool ScaleLevel1State_Prev = false;

bool ScaleNeutralLevelState_Cur = false;
bool ScaleNeutralLevelState_Prev = false;

bool ScaleLevel1BackState_Cur = false;
bool ScaleLevel1BackState_Prev = false;

bool SetUpState_Cur = false;
bool SetUpState_Prev = false;

bool ResetState_Cur = false;
bool ResetState_Prev = false;

bool ScaleBackLobState_Cur = false;
bool ScaleBackLobState_Prev = false;

bool ClimbState_Cur = false;
bool ClimbState_Prev = false;

bool PortalState_Cur = false;
bool PortalState_Prev = false;

bool AutoSwitchState_Cur = false;
bool AutoSwitchState_Prev = false;

bool transitioning;

bool isreceivinginput;

int CurrentState;

Timer *ReZeroTimer;

void changeMode(LiftMode desiredLiftMode);

void Send_Data();

void UpdateLift(bool IntakeState,bool IntakeDownState, bool ScaleLevel1State,bool ScaleLevel1BackState, bool ScaleNeutralLevelState,
		bool SetUpState, bool ResetState, bool ClawDeployState, bool ClimbState, bool PortalState, bool AutoSwitchState);
void DisablePID();

void WaitForArm(float targ, float tolerance);
void WaitForArm_AboveHorizontal(float targ);

void WaitForWrist(float targ, float tolerance);
void WaitForLimitSwitch();

void WaitForElevator(float targ, float tolerance);

void EndState();

};

#endif /* SRC_LIFTMANAGER_H_ */
