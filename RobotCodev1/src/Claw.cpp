/*
 * Claw.cpp
 *
 *  Created on: Jan 14, 2018
 *      Author: 987
 */

#include <Claw.h>
#include "MyRobot.h"


ClawClass::ClawClass() {

	Claw1 = new VictorSPX(Claw_Motor_1);
	Claw1->SetInverted(true);

	Claw2 = new VictorSPX(Claw_Motor_2);
	Claw2->Set(ControlMode::Follower, Claw_Motor_1);
	Claw2->SetInverted(false);
}

ClawClass::~ClawClass() {
}

void ClawClass::Sense_Current()
{
	float alpha = 0.1f;

	float curleft = MyRobotClass::Get()->PDP->GetCurrent(Claw1_PDPChannel);
	float curright = MyRobotClass::Get()->PDP->GetCurrent(Claw2_PDPChannel);

	leftCurrent = alpha * curleft + (1.0f - alpha) * leftCurrent;
	rightCurrent = alpha * curright + (1.0f - alpha) * rightCurrent;

}

void ClawClass::Update(bool intake, bool switchouttake, bool outtake,bool slowout,bool istracking)
{
	Sense_Current();
	if(intake || istracking)
	{
		Claw1->Set(ControlMode::PercentOutput, 1.0);
		isIntaking = true;
	}
	else if(switchouttake)
	{
		Claw1->Set(ControlMode::PercentOutput, -1.0);
		//isIntaking = true;
	}
	else if(outtake)
	{
		Claw1->Set(ControlMode::PercentOutput, -0.5);
	}
	else if(slowout)
	{
		Claw1->Set(ControlMode::PercentOutput, -0.2);
	}
	else
	{
		Claw1->Set(ControlMode::PercentOutput, 0);
		isIntaking = false;
	}
}
void ClawClass::Auto_Update()
{
	Sense_Current();
}

void ClawClass::Claw_Intake(float value)
{
	Claw1->Set(ControlMode::PercentOutput, value);
	isIntaking = true;
}

void ClawClass::Claw_Outake(float value)
{
	Claw1->Set(ControlMode::PercentOutput, value); // Switch: -1.0, Scale: -0.75, Soft: -0.3
}
void ClawClass::Claw_Off()
{
	Claw1->Set(ControlMode::PercentOutput, 0.0);
	isIntaking = false;
}

void ClawClass::Send_Data()
{
	SmartDashboard::PutNumber("Claw1 Filter Current",leftCurrent);
	SmartDashboard::PutNumber("Claw1 Raw Current",MyRobotClass::Get()->PDP->GetCurrent(Claw1_PDPChannel));
	SmartDashboard::PutNumber("Claw2 Filter Current",rightCurrent);
	SmartDashboard::PutNumber("Claw2 Raw Current",MyRobotClass::Get()->PDP->GetCurrent(Claw2_PDPChannel));
	SmartDashboard::PutBoolean("Got Cube",GotCube());
	SmartDashboard::PutBoolean("Is Intaking",isIntaking);

}
bool ClawClass::GotCube()
{
	if((leftCurrent > 40) && (rightCurrent > 40))
	{
		return true;
	}
	else
	{
		return false;
	}
}
