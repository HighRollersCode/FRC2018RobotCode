/*
 * Conveyor.h
 *
 *  Created on: Feb 11, 2018
 *      Author: 987
 */

#ifndef SRC_CONVEYOR_H_
#define SRC_CONVEYOR_H_


#include "WPILib.h"
#include "Defines.h"
#include "ctre/Phoenix.h"

class ConveyorClass {
public:

	WPI_TalonSRX *Conveyor;

	ConveyorClass();
	virtual ~ConveyorClass();
	void Update(bool forward, bool backward);

	void Conveyor_For();
	void Conveyor_Rev();
	void Conveyor_Off();
};

#endif /* SRC_CONVEYOR_H_ */
