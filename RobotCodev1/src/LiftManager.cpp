/*
 * LiftManager.cpp
 *
 *  Created on: Jan 27, 2018
 *      Author: 987
 */

#include <LiftManager.h>

LiftManagerClass::LiftManagerClass(ElevatorClass *_Elevator, ArmClass *_Arm, ClawClass *_Claw)
{
	_elevator = _Elevator;
	_arm =  _Arm;
	_claw = _Claw;

	IntakeState_Cur = false;
	IntakeState_Prev = false;

	SwitchState_Cur = false;
	SwitchState_Prev = false;

	ScaleLevel1State_Cur = false;
	ScaleLevel1State_Prev = false;

	transitioning = false;

	CurrentState = 0;

	CurrentLiftMode = LiftMode::Free;

}

LiftManagerClass::~LiftManagerClass() {
}
void LiftManagerClass::changeMode(LiftMode desiredLiftMode)
{
	CurrentLiftMode = desiredLiftMode;
	CurrentState = 0;
	transitioning = true;

}

void LiftManagerClass::DisablePID()
{
	//_arm->WristPIDController->Disable();
	//_arm->ArmPIDController->Disable();
	//_elevator->ElevatorPIDController->Disable();
}
void LiftManagerClass::UpdateLift(bool IntakeState, bool SwitchState,bool ScaleLevel1State)
 {
	IntakeState_Prev = IntakeState_Cur;
	IntakeState_Cur = IntakeState;

	SwitchState_Prev = SwitchState_Cur;
	SwitchState_Cur = SwitchState;

	ScaleLevel1State_Prev = ScaleLevel1State_Cur;
	ScaleLevel1State_Cur = ScaleLevel1State;

	if(!IntakeState_Prev && IntakeState_Cur)
	{
		changeMode(LiftMode::Intake);
	}

	if(!SwitchState_Prev && SwitchState_Cur)
	{
		changeMode(LiftMode::Switch_Front_Level);
	}

	if(!ScaleLevel1State_Prev && ScaleLevel1State_Cur)
	{
		changeMode(LiftMode::Scale_Winning_Front_Level1);
	}

	if(transitioning)
	{
		if(CurrentLiftMode == LiftMode::Intake)
		{
			switch(CurrentState)
			{
				case 0 :
					{
						_elevator->SetElevatorTarg(Elevator_Intake);
						CurrentState = 1;
						break;
					}
				case 1 :
					{
						_arm->SetWristTarg(Wrist_Intake);
						if(_arm->WristOnTarg())
						{
							CurrentState = 2;
						}
						break;
					}
				case 2 :
					{
						_arm->SetArmTarg(Arm_Intake);

						if(_arm->ArmOnTarg())
						{
							CurrentState = 3;
						}
						break;
					}
				case 3:
					{
						DisablePID();
						transitioning = false;
						CurrentState = 4;
						break;
					}
			}
		}
		else if(CurrentLiftMode == LiftMode::Switch_Front_Level)
		{
			switch(CurrentState)
			{
				case 0:
					{
						_elevator->SetElevatorTarg(Elevator_Switch);
						CurrentState = 1;
						break;
					}

				case 1:
					{
						_arm->SetWristTarg(Wrist_Switch);
						if(_arm->WristOnTarg())
						{
							CurrentState = 2;
						}
						break;
					}
				case 2:
					{
						_arm->SetArmTarg(Arm_Switch);
						if(_arm->ArmOnTarg())
						{
							CurrentState = 3;
						}
						break;
					}
				case 3:
					{
						DisablePID();
						transitioning = false;
						CurrentState = 4;
						break;
					}
			}
		}
		else if(CurrentLiftMode == LiftMode::Scale_Winning_Front_Level1)
		{
			switch(CurrentState)
			{
				case 0 :
					{
						_elevator->SetElevatorTarg(Elevator_Scale_Level_1);
						CurrentState = 1;
						break;
					}
				case 1:
					{
						_arm->SetWristTarg(Wrist_Folded);
						if(_arm->WristOnTarg())
						{
							CurrentState = 2;
						}
						break;
					}
				case 2:
					{
						_arm->SetArmTarg(Arm_Scale_Level_1);
						if(_arm->ArmOnTarg())
						{
							CurrentState = 3;
						}
						break;
					}
				case 3:
					{
						_arm->SetWristTarg(Wrist_Scale_Level_1);
						if(_arm->WristOnTarg())
						{
							CurrentState++;
							transitioning = false;
						}

						break;
					}
			}
		}
		else if(CurrentLiftMode == LiftMode::Scale_Winning_Back_Level1)
		{
			switch(CurrentState)
			{
				case 0 :
					{
						_elevator->SetElevatorTarg(Elevator_Scale_Level_1);
						CurrentState = 1;
						break;
					}
				case 1:
					{
						_arm->SetWristTarg(Wrist_Folded);
						if(_arm->WristOnTarg())
						{
							CurrentState = 2;
						}
						break;
					}
				case 2:
					{
						_arm->SetArmTarg(Arm_Scale_Level_1);
						if(_arm->ArmOnTarg() && _elevator->ElevatorOnTarg())
						{
							CurrentState = 3;
						}
						break;
					}
				case 3:
					{
						_arm->SetWristTarg(Wrist_Scale_Level_1_Back);
						if(_arm->WristOnTarg())
						{
							CurrentState++;
							transitioning = false;
						}

						break;
					}
			}
		}
	}
}
