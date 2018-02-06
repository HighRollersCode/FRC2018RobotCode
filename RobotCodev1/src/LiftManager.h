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
	Scale_Winning_Front_Level1,
	Scale_Winning_Back_Level1
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

bool transitioning;

int CurrentState;

void changeMode(LiftMode desiredLiftMode);

void UpdateLift(bool IntakeState,bool SwitchState, bool ScaleLevel1State);
void DisablePID();
};

#endif /* SRC_LIFTMANAGER_H_ */
