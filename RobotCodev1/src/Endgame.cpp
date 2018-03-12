/*
 * Endgame.cpp
 *
 *  Created on: Feb 20, 2018
 *      Author: 987
 */

#include "Endgame.h"
#include "MyRobot.h"

EndgameClass::EndgameClass()
{
	lock = new DoubleSolenoid(16,Lock_On,Lock_Off);

	clawDeploy = new Solenoid(16,Claw_Deploy);

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

void EndgameClass::Update(bool LockEnable, bool ClawEnable,float timer)
{
	lockEnable_prev = lockEnable_cur;
	lockEnable_cur = LockEnable;

	clawDeploy_prev = clawDeploy_cur;
	clawDeploy_cur = ClawEnable;
	if(timer < shutOffTime || !DriverStation::GetInstance().IsFMSAttached())
	{
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
	}
	else
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
	SmartDashboard::PutBoolean("Claw Deployer", clawDeploy->Get());
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
	clawDeploy->Set(true);
}
void EndgameClass::ClawDeployOff()
{
	clawDeploy->Set(false);
}
