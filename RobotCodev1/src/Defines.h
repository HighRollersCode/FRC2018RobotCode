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
#define Claw_Motor_2 5

#define Wrist_Motor 4

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

#define Wrist_Encoder_1 10
#define Wrist_Encoder_2 11

//Solenoid
#define Sol_High_Gear 0
#define Sol_Low_Gear 1


//Elevator Values
#define Elevator_Intake 5000
#define Elevator_Switch 20
#define Elevator_Scale_Level_1 3000

#define ElevatorMaxLimEncoder 13200
#define ElevatorMinLimEncoder 0

//Arm Values
#define Arm_Intake 10
#define Arm_Switch 100
#define Arm_Scale_Level_1 100

#define ArmMaxLimEncoder 5000
#define ArmMinLimEncoder -5000

//Wrist Values
#define Wrist_Intake -100
#define Wrist_Switch -70
#define Wrist_Scale_Level_1 -70
#define Wrist_Folded -70
#define Wrist_Scale_Level_1_Back 70

#define WristMaxLimEncoder -300
#define WristMinLimEncoder 300

//PDP Channels
#define Arm_PDPChannel 4
#define Wrist_PDPChannel 11
#define Elevator_PDPChannel 12


#endif //DEFINES_H
