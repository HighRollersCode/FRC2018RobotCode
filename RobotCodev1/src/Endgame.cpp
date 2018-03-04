/*
 * Endgame.cpp
 *
 *  Created on: Feb 20, 2018
 *      Author: 987
 */

#include "Endgame.h"

EndgameClass::EndgameClass()
{
	lock = new DoubleSolenoid(Lock_On,Lock_Off);

	clawDeploy = new DoubleSolenoid(Claw_Deploy_Off,Claw_Deploy_On);

	lockEnable_cur = false;
	lockEnable_prev = false;

	lockToggleState = 1;

	clawDeploy_cur = false;
	clawDeploy_prev = false;

	clawToggleState = 1;

}

EndgameClass::~EndgameClass()
{
	// TODO Auto-generated destructor stub
}

void EndgameClass::Update(bool LockEnable, bool ClawEnable)
{
	lockEnable_prev = lockEnable_cur;
	lockEnable_cur = LockEnable;

	clawDeploy_prev = clawDeploy_cur;
	clawDeploy_cur = ClawEnable;

	if(!lockEnable_prev && lockEnable_cur)
	{
		lockToggleState = -lockToggleState;
	}

	if(lockToggleState == 1)
	{
		LockOff();
	}
	else if(lockToggleState == -1)
	{
		LockOn();
	}

	if(!clawDeploy_prev && clawDeploy_cur)
	{
		clawToggleState = -clawToggleState;
	}

	if(clawToggleState == 1)
	{
		ClawDeployOff();
	}
	else if(clawToggleState == -1)
	{
		ClawDeployOn();
	}
}
void EndgameClass::Send_Data()
{
	SmartDashboard::PutNumber("Claw Deployer", clawDeploy->Get());
	SmartDashboard::PutNumber("Lock",lock->Get());
}

void EndgameClass::LockOn()
{
	lock->Set(DoubleSolenoid::Value::kForward);
}

void EndgameClass::LockOff()
{
	lock->Set(DoubleSolenoid::Value::kReverse);
}

void EndgameClass::ClawDeployOn()
{
	clawDeploy->Set(DoubleSolenoid::Value::kForward);
}
void EndgameClass::ClawDeployOff()
{
	clawDeploy->Set(DoubleSolenoid::Value::kReverse);
}
