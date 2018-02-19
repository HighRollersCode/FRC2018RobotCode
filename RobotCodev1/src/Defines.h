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

enum eSonar
{
	LEFT_SONAR = 0,
	RIGHT_SONAR,
	FRONT_SONAR
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

#define Conveyor_Motor 6

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

//Gyro
#define Pigeon_IMU 6
#define Gyro 0

//Solenoid
#define Sol_High_Gear 0
#define Sol_Low_Gear 1


//Elevator Values
#define Elevator_Intake 1300
#define Elevator_Switch 12000
//#define Elevator_Scale_Level_1 11000
#define Elevator_Scale_Level_1 7250
#define Elevator_tolerance 500

#define ElevatorMaxLimEncoder 13750
#define ElevatorMinLimEncoder 0

//Arm Values
#define Arm_Intake 100
#define Arm_Switch 100
#define Arm_Scale_Level_1 22000
#define Arm_Scale_Neutral 22000

#define ArmMaxLimEncoder 23000
#define ArmMinLimEncoder 0

#define Arm_tolerance 1000

//Wrist Values
#define Wrist_Intake -16500
#define Wrist_Switch -13500
#define Wrist_Scale_Level_1 -14750
#define Wrist_Folded 0
#define Wrist_Scale_Level_1_Back 12000
//#define Wrist_Scale_Level_1_Back 14000

#define Wrist_Scale_Neutral -14000

#define Wrist_tolerance 100

#define WristMaxLimEncoder 22400
#define WristMinLimEncoder -17000

//PDP Channels
#define Arm_PDPChannel 4
#define Claw1_PDPChannel 6
#define Claw2_PDPChannel 7
#define Wrist_PDPChannel 11
#define Elevator_PDPChannel 12


#endif //DEFINES_H
