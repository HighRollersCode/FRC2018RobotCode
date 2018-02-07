/*
 * CurrentLimitedJoint.h
 *
 *  Created on: Jan 24, 2018
 *      Author: 987
 */

#ifndef SRC_CURRENTLIMITEDJOINT_H_
#define SRC_CURRENTLIMITEDJOINT_H_

#include "WPILib.h"

class CurrentLimitedJoint : public PIDOutput
{
public:

	frc::SpeedController *speedController;

	int pdpChannel;
	float maxCurrent;

	int disabledDirection;
	float disabledTime;

	float averagedCurrent;

	Timer *motorDisableTimer;

	CurrentLimitedJoint(frc::SpeedController *sc, int pdpchannel, float maxcurrent,float disabledtime);
	virtual ~CurrentLimitedJoint();

	void Update();
	void SetPower(double power);

	virtual void PIDWrite(double output);

};

#endif /* SRC_CURRENTLIMITEDJOINT_H_ */
