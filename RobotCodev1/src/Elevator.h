/*
 * Elevator.h
 *
 *  Created on: Jan 13, 2018
 *      Author: 987
 */

#ifndef SRC_ELEVATOR_H_
#define SRC_ELEVATOR_H_

#include "WPILib.h"
#include "Defines.h"
#include "ctre/Phoenix.h"

using namespace frc;

class ElevatorClass
{
public:

	WPI_TalonSRX *ElevatorTalon;

	float currentverticalcommand;
	float prevverticalcommand;

	bool isreceivingelevatorinput;

	ElevatorClass();
	virtual ~ElevatorClass();
	float GetElevatorEncoder();
	void ResetElevatorEncoder();

	void SetElevatorTarg(float targ);
	bool ElevatorOnTarg(float tolerance = 5);

	void PIDOff();
	void Update(float verticalcommand);
	void Send_Data();
};

#endif /* SRC_ELEVATOR_H_ */
