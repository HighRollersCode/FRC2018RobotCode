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

class ElevatorClass
{
public:
	WPI_VictorSPX *Elevator;

	Encoder *ElevatorEncoder;

	PIDController *ElevatorPIDController;

	//Preferences *prefs;

	ElevatorClass();
	virtual ~ElevatorClass();
	float GetElevatorEncoder();

	void SetElevator(float targ);
	void ElevatorIntake();
	void ElevatorScale();

	void Update( float verticalcommand);//bool intake, bool scale);
	void Send_Data();
};

#endif /* SRC_ELEVATOR_H_ */
