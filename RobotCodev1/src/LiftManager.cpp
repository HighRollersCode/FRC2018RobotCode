/*
 * LiftManager.cpp
 *
 *  Created on: Jan 27, 2018
 *      Author: 987
 */

#include <LiftManager.h>

LiftManagerClass::LiftManagerClass(ElevatorClass *_Elevator, ArmClass *_Arm, ClawClass *_Claw,EndgameClass *_Endgame)
{
	_elevator = _Elevator;
	_arm =  _Arm;
	_claw = _Claw;
	_endgame = _Endgame;

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

void LiftManagerClass::WaitForArm(float targ,float tolerance)
{
	_arm->SetArmTarg(targ);
	if(_arm->ArmOnTarg(tolerance))
	{
		CurrentState += 1;
	}
}

void LiftManagerClass::WaitForWrist(float targ,float tolerance)
{
	_arm->SetWristTarg(targ);
	if(_arm->WristOnTarg(tolerance))
	{
		CurrentState += 1;
	}
}

void LiftManagerClass::WaitForElevator(float targ, float tolerance)
{
	_elevator->SetElevatorTarg(targ);
	if(_elevator->ElevatorOnTarg(tolerance))
	{
		CurrentState += 1;
	}
}

void LiftManagerClass::EndState()
{
	CurrentState++;
	transitioning = false;
}
void LiftManagerClass::UpdateLift(
		bool IntakeState, bool SwitchState,bool ScaleLevel1State,bool ScaleLevel1BackState,bool ScaleNeutralLevelState,
		bool SetUpState,bool ScaleBackLobState,bool ClimbState, bool PortalState)
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

	ClimbState_Prev = ClimbState_Cur;
	ClimbState_Cur = ClimbState;

	ScaleBackLobState_Prev = ScaleBackLobState_Cur;
	ScaleBackLobState_Cur = ScaleBackLobState;

	PortalState_Prev = PortalState_Cur;
	PortalState_Cur = PortalState;

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

	if(!ClimbState_Prev && ClimbState_Cur)
	{
		//changeMode(LiftMode::Climb);
	}

	if(!ScaleBackLobState_Prev && ScaleBackLobState_Cur)
	{
		changeMode(LiftMode::Scale_Back_Lob);
	}

	if(!PortalState_Prev && PortalState_Cur)
	{
		//changeMode(LiftMode::Claw_Deploy);
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
						//_elevator->SetElevatorTarg(2000);
						_arm->SetWristTarg(Wrist_Folded);
						CurrentState = 1;
						break;
					}
				case 1 :
					{
						/*if(_elevator->ElevatorOnTarg(Elevator_tolerance))
						{
							EndState();
						}*/
						_elevator->SetElevatorTarg(Elevator_Intake);
						if(_arm->WristOnTarg(Wrist_tolerance))
						{
							CurrentState = 2;
						}
						break;
					}
				case 2 :
					{
						WaitForArm(Arm_Intake,Arm_tolerance * 2);
						/*_arm->SetArmTarg(Arm_Intake);
						if(_arm->ArmOnTarg(Arm_tolerance * 2))
						{
							CurrentState = 3;
						}*/
						break;
					}
				case 3:
					{
						_arm->SetWristTarg(Wrist_Intake);
						if(_arm->WristOnTarg(Wrist_tolerance) && _arm->ArmOnTarg(Arm_tolerance))
						{;
							CurrentState = 4;
						}
						break;
					}
				case 4:
					{
						EndState();
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
						WaitForWrist(Wrist_Switch,Wrist_tolerance);
						/*_arm->SetWristTarg(Wrist_Switch);
						if(_arm->WristOnTarg())
						{
							CurrentState = 2;
						}*/
						break;
					}
				case 2:
					{
						WaitForArm(Arm_Switch,Arm_tolerance);
						/*_arm->SetArmTarg(Arm_Switch);
						if(_arm->ArmOnTarg(Arm_tolerance))
						{
							CurrentState = 3;
						}*/
						break;
					}
				case 3:
					{
						WaitForElevator(Elevator_Switch,Elevator_tolerance);
						break;
					}
				case 4:
					{
						EndState();
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
						/*if(_elevator->ElevatorOnTarg(Elevator_tolerance))
						{
							EndState();
						}*/
						//WaitForWrist(Wrist_Folded,Wrist_tolerance,CurrentState);
						_arm->SetWristTarg(Wrist_Folded);
						if(_arm->WristOnTarg(Wrist_tolerance))
						{
							CurrentState = 2;
						}
						break;
					}
				case 2:
					{
						//WaitForArm(Arm_Scale_Level_1,Arm_tolerance,CurrentState);
						_arm->SetArmTarg(Arm_Scale_Level_1);
						if(_arm->ArmOnTarg(Arm_tolerance))
						{
							CurrentState = 3;
						}
						break;
					}
				case 3:
					{
						//WaitForWrist(Wrist_Scale_Level_1,150,CurrentState);
						_arm->SetWristTarg(Wrist_Scale_Level_1);
						if(_arm->WristOnTarg(150))
						{
							CurrentState = 4;
						}
						break;
					}
				case 4:
					{
						EndState();
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
						//WaitForWrist(Wrist_Folded,Wrist_tolerance,CurrentState);
						_arm->SetWristTarg(Wrist_Folded);
						if(_arm->WristOnTarg(Wrist_tolerance))
						{
							CurrentState = 1;
						}
						break;
					}
				case 1 :
					{
						//WaitForArm(Arm_Scale_Neutral,Arm_tolerance,CurrentState);
						_arm->SetArmTarg(Arm_Scale_Neutral);
						if(_arm->ArmOnTarg(Arm_tolerance))
						{
							CurrentState = 2;
						}
						break;
					}
				case 2 :
					{
						//WaitForWrist(Wrist_Scale_Neutral,Wrist_tolerance,CurrentState);
						_arm->SetWristTarg(Wrist_Scale_Neutral);
						if(_arm->WristOnTarg(Wrist_tolerance))
						{
							CurrentState = 3;
						}
						break;
					}
				case 3 :
					{
						EndState();
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
						_elevator->SetElevatorTarg(Elevator_Scale_Level_1_Back);
						CurrentState = 1;
						break;
					}
				case 1:
					{
						//WaitForWrist(Wrist_Folded,Wrist_tolerance,CurrentState);
						_arm->SetWristTarg(Wrist_Folded);
						if(_arm->WristOnTarg(Wrist_tolerance * 3))
						{
							CurrentState = 2;
						}
						break;
					}
				case 2:
					{
						_arm->SetArmTarg(Arm_Scale_Level_1);
						if(_arm->ArmOnTarg(Arm_tolerance * 2.5))
						{
							CurrentState = 3;
						}
						break;
					}
				case 3:
					{
						_arm->SetWristTarg(Wrist_Scale_Level_1_Back);
						if(_arm->WristOnTarg(Wrist_tolerance) && _arm->ArmOnTarg(Arm_tolerance) && _elevator->ElevatorOnTarg(Elevator_tolerance))
						{
							CurrentState = 4;
						}

						break;
					}
				case 4:
					{
						EndState();
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
						//WaitForWrist(0,Wrist_tolerance,CurrentState);
						_arm->SetWristTarg(0);
						if(_arm->WristOnTarg(Wrist_tolerance))
						{
							CurrentState = 2;
						}
						break;
					}
				case 2:
					{
						//WaitForArm(0,Arm_tolerance,CurrentState);
						_arm->SetArmTarg(0);
						if(_arm->ArmOnTarg(Arm_tolerance))
						{
							CurrentState = 3;
						}

						break;
					}
				case 3:
					{
						//WaitForElevator(0,Elevator_tolerance,CurrentState);
						_elevator->SetElevatorTarg(0);
						if(_elevator->ElevatorOnTarg(Elevator_tolerance))
						{
							CurrentState++;
							transitioning = false;
						}

						break;
					}
				case 4:
					{
						EndState();
						/*CurrentState++;
						transitioning = false;*/

						break;
					}
			}
		}
		else if(CurrentLiftMode == LiftMode::Scale_Back_Lob)
		{
			switch(CurrentState)
			{
				case 0 :
					{
						WaitForWrist(Wrist_Folded,Wrist_tolerance);
						break;
					}
				case 1:
					{
						_elevator->SetElevatorTarg(Elevator_Scale_Back_Lob);
						CurrentState = 2;
						break;
					}
				case 2:
					{
						WaitForArm(Arm_Scale_Back_Lob,Arm_tolerance);
						break;
					}
				case 3:
					{
						WaitForWrist(Wrist_Scale_Back_Lob,Wrist_tolerance);
						break;
					}
				case 4:
					{
						EndState();
						break;
					}
			}
		}
		/*
		else if(CurrentLiftMode == LiftMode::Climb)
		{
			switch(CurrentState)
			{
				case 0 :
					{
						WaitForElevator(400,50);
						break;
					}
				case 1:
					{
						_endgame->Update(true,false);
						CurrentState = 2;
						break;
					}
				case 2:
					{
						_endgame->Update(false, false);
						EndState();
						break;
					}
			}
		}*/
		else if(CurrentLiftMode == LiftMode::Portal)
		{
			switch(CurrentState)
			{
				case 0:
					{
						_elevator->SetElevatorTarg(Elevator_Portal);
						CurrentState = 1;
						break;
					}

				case 1:
					{
						WaitForWrist(Wrist_Portal,Wrist_tolerance);
						/*_arm->SetWristTarg(Wrist_Switch);
						if(_arm->WristOnTarg())
						{
							CurrentState = 2;
						}*/
						break;
					}
				case 2:
					{
						WaitForArm(Arm_Portal,Arm_tolerance);
						/*_arm->SetArmTarg(Arm_Switch);
						if(_arm->ArmOnTarg(Arm_tolerance))
						{
							CurrentState = 3;
						}*/
						break;
					}
				case 3:
					{
						EndState();
						break;
					}
			}
		}

	}

	if(CurrentLiftMode == LiftMode::Intake && transitioning == false)
	{
		if(_claw->isIntaking)
		{
			_arm->SetWristTarg(Wrist_Intake);
		}
		else
		{
			_arm->SetWristTarg(Wrist_Folded);
		}
	}
}
