/*
 * CurrentLimitedJoint.cpp
 *
 *  Created on: Jan 24, 2018
 *      Author: 987
 */

#include <CurrentLimitedJoint.h>
#include "MyRobot.h"

CurrentLimitedJoint::CurrentLimitedJoint(frc::SpeedController *sc, int pdpchannel, float maxcurrent, float disabledtime) :
speedController(sc), pdpChannel(pdpchannel),maxCurrent(maxcurrent),disabledTime(disabledtime)
{
	disabledDirection = 0;

	motorDisableTimer = new Timer;
	motorDisableTimer->Reset();

	averagedCurrent = 0;

}

CurrentLimitedJoint::~CurrentLimitedJoint() {
	// TODO Auto-generated destructor stub
}
void CurrentLimitedJoint::Update()
{
	if(motorDisableTimer->Get() > disabledTime)
	{
		motorDisableTimer->Stop();
		disabledDirection = 0;
	}

	float current = MyRobotClass::Get()->PDP->GetCurrent(pdpChannel);
	float average_percent = 0.01f;
	averagedCurrent = average_percent * current + (1.0 - average_percent) * averagedCurrent;

	if(averagedCurrent > maxCurrent)
	{
		if (disabledDirection == 0)
		{
			double motorcommand = speedController->Get();
			if(motorcommand > 0)
			{
				disabledDirection = 1;
				SetPower(0);
			}
			else if(motorcommand < 0)
			{
				disabledDirection = -1;
				SetPower(0);
			}
		}
		motorDisableTimer->Reset();
		motorDisableTimer->Start();
	}


}
void CurrentLimitedJoint::SetPower(double power)
{
	if((disabledDirection > 0)&&(power > 0))
	{
		power = 0;
	}

	if((disabledDirection < 0)&&(power < 0))
	{
		power = 0;
	}
	speedController->Set(power);
}

void CurrentLimitedJoint::PIDWrite(double output)
{
	SetPower(output);
}
