/*
 * Claw.cpp
 *
 *  Created on: Jan 14, 2018
 *      Author: 987
 */

#include <Claw.h>

ClawClass::ClawClass() {

	Claw1 = new VictorSPX(Claw_Motor_1);

	Claw2 = new VictorSPX(Claw_Motor_2);
	Claw2->Set(ControlMode::Follower, Claw_Motor_1);
	Claw2->SetInverted(true);
}
ClawClass::~ClawClass() {
}
void ClawClass::Update(bool intake, bool outtake)
{
	if(intake)
	{

		Claw1->Set(ControlMode::PercentOutput, -0.75);
	}
	else if(outtake)
	{
		Claw1->Set(ControlMode::PercentOutput, 1.0);
	}
	else
	{
		Claw1->Set(ControlMode::PercentOutput, 0);
	}

}
