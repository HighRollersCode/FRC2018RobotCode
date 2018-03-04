/*
 * Endgame.h
 *
 *  Created on: Feb 20, 2018
 *      Author: 987
 */

#ifndef SRC_ENDGAME_H_
#define SRC_ENDGAME_H_

#include "WPILib.h"
#include "Defines.h"

using namespace frc;

class EndgameClass {
public:

	DoubleSolenoid *lock;

	DoubleSolenoid *clawDeploy;

	bool lockEnable_prev;
	bool lockEnable_cur;

	bool clawDeploy_prev;
	bool clawDeploy_cur;

	float lockToggleState;

	float clawToggleState;

	EndgameClass();
	virtual ~EndgameClass();
	void Update(bool LockEnable, bool ClawEnable);
	void Send_Data();

	void LockOn();
	void LockOff();

	void ClawDeployOn();
	void ClawDeployOff();
};

#endif /* SRC_ENDGAME_H_ */
