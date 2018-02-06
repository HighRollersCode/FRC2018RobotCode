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
#include "CurrentLimitedJoint.h"

class ElevatorClass
{
public:
	//WPI_VictorSPX *Elevator;

	WPI_TalonSRX *ElevatorTalon;

	float currentverticalcommand;
	float prevverticalcommand;

	//Encoder *ElevatorEncoder;

	//PIDController *ElevatorPIDController;

	//CurrentLimitedJoint *ElevatorJoint;

	//Preferences *prefs;

	ElevatorClass();
	virtual ~ElevatorClass();
	float GetElevatorEncoder();

	void SetElevatorTarg(float targ);
	bool ElevatorOnTarg(float tolerance = 5);

	void ElevatorIntake();
	void ElevatorScale();

	void Update( float verticalcommand, bool intake);//, bool scale);
	void Send_Data();
};

#endif /* SRC_ELEVATOR_H_ */
