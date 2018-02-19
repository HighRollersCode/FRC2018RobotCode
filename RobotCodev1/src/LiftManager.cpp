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

	ScaleLevel1BackState_Cur = false;
	ScaleLevel1BackState_Prev = false;

	ScaleNeutralLevelState_Cur = false;
	ScaleNeutralLevelState_Prev = false;

	SetUpState_Cur = false;
	SetUpState_Prev = false;

	transitioning = false;

	isreceivinginput = false;

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
void LiftManagerClass::Send_Data()
{
	SmartDashboard::PutNumber("Current Lift Mode", CurrentLiftMode);
	SmartDashboard::PutNumber("Current State", CurrentState);
	SmartDashboard::PutBoolean("Transitioning", transitioning);
	SmartDashboard::PutBoolean("Is Receiving Input", isreceivinginput);
}
void LiftManagerClass::DisablePID()
{
	//_arm->WristPIDController->Disable();
	//_arm->ArmPIDController->Disable();
	//_elevator->ElevatorPIDController->Disable();
}
void LiftManagerClass::UpdateLift(
		bool IntakeState, bool SwitchState,bool ScaleLevel1State,bool ScaleLevel1BackState,bool ScaleNeutralLevelState,bool SetUpState)
 {
	IntakeState_Prev = IntakeState_Cur;
	IntakeState_Cur = IntakeState;

	SwitchState_Prev = SwitchState_Cur;
	SwitchState_Cur = SwitchState;

	ScaleLevel1State_Prev = ScaleLevel1State_Cur;
	ScaleLevel1State_Cur = ScaleLevel1State;

	ScaleLevel1BackState_Prev = ScaleLevel1State_Cur;
	ScaleLevel1BackState_Cur = ScaleLevel1BackState;

	ScaleNeutralLevelState_Prev = ScaleNeutralLevelState_Cur;
	ScaleNeutralLevelState_Cur = ScaleNeutralLevelState;

	SetUpState_Prev = SetUpState_Cur;
	SetUpState_Cur = SetUpState;

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
		changeMode(LiftMode::Scale_Losing_Level_Front);
	}

	if(!ScaleLevel1BackState_Prev && ScaleLevel1BackState_Cur)
	{
		changeMode(LiftMode::Scale_Winning_Back_Level1);
	}

	if(!ScaleNeutralLevelState_Prev && ScaleNeutralLevelState_Cur)
	{
		changeMode(LiftMode::Scale_Neutral_Front);
	}

	if(!SetUpState_Prev && SetUpState_Cur)
	{
		changeMode(LiftMode::Set_Up);
	}

	if(_elevator->isreceivingelevatorinput || _arm->isreceivingarminput || _arm->isreceivingwristinput)
	{
		isreceivinginput = true;
	}
	else
	{
		isreceivinginput = false;
	}

	if(isreceivinginput)
	{
		CurrentState = 0;
		transitioning = false;
		changeMode(LiftMode::Free);
	}

	if(transitioning && !isreceivinginput)
	{
		if(CurrentLiftMode == LiftMode::Intake)
		{
			switch(CurrentState)
			{
			case 0 :
					{
						_arm->SetWristTarg(Wrist_Folded);
						CurrentState = 1;
						break;
					}
			case 1 :
					{
						_elevator->SetElevatorTarg(Elevator_Intake);
						if(_arm->WristOnTarg(Wrist_tolerance))
						{
							CurrentState = 2;
						}
						break;
					}
					case 2 :
					{
						_arm->SetArmTarg(Arm_Intake);

						if(_arm->ArmOnTarg(Arm_tolerance * 2))
						{
							CurrentState = 3;
						}
						break;
					}
				case 3:
					{
						_arm->SetWristTarg(Wrist_Intake);
						if(_arm->WristOnTarg(Wrist_tolerance) && _arm->ArmOnTarg(Arm_tolerance))
						{
							CurrentState = 4;
						}
						break;
					}
				case 4:
					{
						CurrentState++;
						transitioning = false;
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
						if(_arm->ArmOnTarg(Arm_tolerance))
						{
							CurrentState = 3;
						}
						break;
					}
				case 3:
					{
						CurrentState++;
						transitioning = false;
						break;
					}
			}
		}
		else if(CurrentLiftMode == LiftMode::Scale_Losing_Level_Front)
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
						if(_arm->WristOnTarg(Wrist_tolerance))
						{
							CurrentState = 2;
						}
						break;
					}
				case 2:
					{
						_arm->SetArmTarg(Arm_Scale_Level_1);
						if(_arm->ArmOnTarg(Arm_tolerance))
						{
							CurrentState = 3;
						}
						break;
					}
				case 3:
					{
						_arm->SetWristTarg(Wrist_Scale_Level_1);
						if(_arm->WristOnTarg(150))
						{
							CurrentState++;
							transitioning = false;
						}
						break;
					}
			}
		}
		else if(CurrentLiftMode == LiftMode::Scale_Neutral_Front)
		{
			switch(CurrentState)
			{
				case 0 :
					{
						_arm->SetWristTarg(Wrist_Folded);
						if(_arm->WristOnTarg(Wrist_tolerance))
						{
							CurrentState = 1;
						}
						break;
					}
				case 1 :
					{
						_arm->SetArmTarg(Arm_Scale_Neutral);
						if(_arm->ArmOnTarg(Arm_tolerance))
						{
							CurrentState = 2;
						}
						break;
					}
				case 2 :
					{
						_arm->SetWristTarg(Wrist_Scale_Neutral);
						if(_arm->WristOnTarg(Wrist_tolerance))
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
						if(_arm->WristOnTarg(Wrist_tolerance))
						{
							CurrentState = 2;
						}
						break;
					}
				case 2:
					{
						_arm->SetArmTarg(Arm_Scale_Level_1);
						if(_arm->ArmOnTarg(Arm_tolerance * 2) && _elevator->ElevatorOnTarg(Elevator_tolerance))
						{
							CurrentState = 3;
						}
						break;
					}
				case 3:
					{
						_arm->SetWristTarg(Wrist_Scale_Level_1_Back);
						if(_arm->WristOnTarg(Wrist_tolerance) && _arm->ArmOnTarg(Arm_tolerance))
						{
							CurrentState++;
							transitioning = false;
						}

						break;
					}
			}
		}
		else if(CurrentLiftMode == LiftMode::Set_Up)
		{
			switch(CurrentState)
			{
				case 0 :
					{
						_elevator->SetElevatorTarg(2000);
						CurrentState = 1;
						break;
					}
				case 1:
					{
						_arm->SetWristTarg(0);
						if(_arm->WristOnTarg(Wrist_tolerance))
						{
							CurrentState = 2;
						}
						break;
					}
				case 2:
					{
						_arm->SetArmTarg(0);
						if(_arm->ArmOnTarg(Arm_tolerance))
						{
							CurrentState = 3;
						}

						break;
					}
				case 3:
					{
						_elevator->SetElevatorTarg(0);
						if(_elevator->ElevatorOnTarg(Elevator_tolerance))
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
