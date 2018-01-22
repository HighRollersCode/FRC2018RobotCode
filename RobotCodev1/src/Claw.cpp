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

	//ClawIntake = new Solenoid(0);
}
ClawClass::~ClawClass() {
}
void ClawClass::Update(bool intake, bool outtake)
{
	if(intake)
	{
		//ClawIntake->Set(true);

		Claw1->Set(ControlMode::PercentOutput, -1.0);
	}
	else if(outtake)
	{
		//ClawIntake->Set(true);
		Claw1->Set(ControlMode::PercentOutput, 1.0);
	}
	else
	{
		//ClawIntake->Set(false);
		Claw1->Set(ControlMode::PercentOutput, 0);
	}

}
