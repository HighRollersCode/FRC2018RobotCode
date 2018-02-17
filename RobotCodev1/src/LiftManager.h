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

enum LiftMode
{
	Free = 0,
	Intake,
	Switch_Front_Level,
	Scale_Losing_Level_Front,
	Scale_Neutral_Front,
	Scale_Winning_Back_Level1,
	Set_Up
};
class LiftManagerClass {
public:
	LiftManagerClass(ElevatorClass *_Elevator, ArmClass *_Arm, ClawClass *_Claw);
	virtual ~LiftManagerClass();

ElevatorClass *_elevator;
ArmClass *_arm;
ClawClass *_claw;
LiftMode CurrentLiftMode;

bool IntakeState_Cur;
bool IntakeState_Prev;

bool SwitchState_Cur;
bool SwitchState_Prev;

bool ScaleLevel1State_Cur;
bool ScaleLevel1State_Prev;

bool ScaleNeutralLevelState_Cur;
bool ScaleNeutralLevelState_Prev;

bool ScaleLevel1BackState_Cur;
bool ScaleLevel1BackState_Prev;

bool SetUpState_Cur;
bool SetUpState_Prev;

bool transitioning;

bool isreceivinginput;

int CurrentState;

void changeMode(LiftMode desiredLiftMode);

void Send_Data();

void UpdateLift(bool IntakeState,bool SwitchState, bool ScaleLevel1State,bool ScaleLevel1BackState, bool ScaleNeutralLevelState,bool SetUpState);
void DisablePID();
};

#endif /* SRC_LIFTMANAGER_H_ */
