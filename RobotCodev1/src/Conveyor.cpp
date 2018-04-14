/*
 * Conveyor.cpp
 *
 *  Created on: Feb 11, 2018
 *      Author: 987
 */

#include <Conveyor.h>

ConveyorClass::ConveyorClass() {
	Conveyor = new WPI_TalonSRX(Conveyor_Motor);
}

ConveyorClass::~ConveyorClass() {
	// TODO Auto-generated destructor stub
}
void ConveyorClass::Update(bool forward, bool backward)
{
	if(forward)
	{
		Conveyor_For();
	}
	else if(backward)
	{
		Conveyor_Rev();
	}
	else
	{
		Conveyor_Off();
	}
}

void ConveyorClass::Conveyor_For()
{
	Conveyor->Set(1.0);
}

void ConveyorClass::Conveyor_Rev()
{
	Conveyor->Set(-1.0);
}

void ConveyorClass::Conveyor_Off()
{
	Conveyor->Set(0.0);
}
