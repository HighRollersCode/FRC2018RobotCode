#ifndef DEFINES_H
#define DEFINES_H


enum eGyroMode
{
	GYRO_CORRECTION_ON = 0,
	GYRO_CORRECTION_OFF
};

enum eBrakeMode
{
	BRAKE_MODE_ON = 0,
	BRAKE_MODE_OFF
};

//Motors
#define Left_Motor_1 0
#define Left_Motor_2 1
#define Right_Motor_1 2
#define Right_Motor_2 3
#define Strafe_Motor_1 4
#define Strafe_Motor_2 5

#define Elevator_Motor 1
#define Arm_Motor 2

#define Claw_Motor_1 3
#define Claw_Motor_2 4

//Encoders
#define Right_Encoder_1 0
#define Right_Encoder_2 1

#define Left_Encoder_1 2
#define Left_Encoder_2 3

#define Middle_Encoder_1 4
#define Middle_Encoder_2 5


#define Elevator_Encoder_1 6
#define Elevator_Encoder_2 7

#define Arm_Encoder_1 8
#define Arm_Encoder_2 9

//Solenoid
#define Sol_High_Gear 0
#define Sol_Low_Gear 1
#define Sol_Claw_Intake 2


//Elevator Values
#define Elevator_Intake 10
#define Elevator_Scale 3000

#define ElevatorMaxLimEncoder 4140
#define ElevatorMinLimEncoder 0

//Arm Values
#define ArmMaxLimEncoder 400
#define ArmMinLimEncoder 0



#endif //DEFINES_H
