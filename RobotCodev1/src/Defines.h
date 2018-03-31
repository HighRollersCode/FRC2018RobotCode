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
	LEFT_AND_RIGHT_SONAR,
	FRONT_SONAR
};

//Left Joystick Controls

#define Forward_Command leftStick->GetY()

#define Strafe_Command leftStick->GetX()

//#define Shifter_Command leftStick->GetTrigger()

//#define Brake_Mode_Enable leftStick->GetRawButton(2)
#define Portal_Preset leftStick->GetRawButton(2)

#define Gyro_Correction_Enable leftStick->GetRawButton(3)

#define Set_Up_Preset leftStick->GetRawButton(9)

#define Random_Number leftStick->GetRawButton(11)


//Right Joystick Controls
#define Turn_Command rightStick->GetX()

#define Intake_Command rightStick->GetTrigger()

#define Outake_Command rightStick->GetRawButton(2)

#define Track_Enable rightStick->GetRawButton(3)

#define Intake_Down_Mode_Preset rightStick->GetRawButton(4)

#define Intake_Mode_Preset rightStick->GetRawButton(5)

#define Conveyor_Left rightStick->GetRawButton(6)

#define Conveyor_Right rightStick->GetRawButton(7)

#define Deploy_Lock rightStick->GetRawButton(10)
//#define Climb_Preset rightStick->GetRawButton(10)

#define Deploy_Claw rightStick->GetRawButton(11)
//#define Claw_Deploy_Preset rightStick->GetRawButton(11)


//Turret Joystick Controls
#define Elevator_Command turretStick->GetY()

#define Arm_Command turretStick->GetY()

#define Enable_Elevator turretStick->GetTrigger()

#define Turret_Outake turretStick->GetRawButton(2)

#define Turret_Slow_Outake turretStick->GetRawButton(3)

#define Wrist_Up_Command turretStick->GetRawButton(4)

#define Wrist_Down_Command turretStick->GetRawButton(5)

#define Scale_Neutral_Preset turretStick->GetRawButton(6)

#define Scale_Front_Level_1_Preset turretStick->GetRawButton(7)

#define Scale_Back_Preset turretStick->GetRawButton(8)

#define Scale_Back_Lob_Preset turretStick->GetRawButton(9)

#define Reset_Encoders turretStick->GetRawButton(11)



//Motors
#define Left_Motor_1 0
#define Left_Motor_2 1
#define Right_Motor_1 2
#define Right_Motor_2 3
#define Strafe_Motor_1 4
#define Strafe_Motor_2 5

#define Elevator_Motor 1
#define Elevator_Motor_2 7

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

//Gyro
#define Pigeon_IMU 6
#define Gyro 0

//Solenoid
#define Sol_High_Gear 0
#define Sol_Low_Gear 1

#define Lock_On 2
#define Lock_Off 3

#define Claw_Deploy 4


//Elevator Values
#define Elevator_Intake 200 //100
#define Elevator_Switch 15150
//#define Elevator_Scale_Level_1 11000
#define Elevator_Scale_Level_1 7250
#define Elevator_Scale_Level_1_Back 12000
#define Elevator_Claw_Deploy 15150
#define Elevator_Portal 10600
#define Elevator_Scale_Back_Lob 6850

#define Elevator_tolerance 500

#define ElevatorMaxLimEncoder 15200
#define ElevatorMinLimEncoder 0

//Arm Values
#define Arm_Intake 3500 //2300
#define Arm_Switch 100
#define Arm_Scale_Level_1 22000
#define Arm_Scale_Neutral 22000
#define Arm_Portal 2300
#define Arm_Scale_Back_Lob 21950

#define ArmMaxLimEncoder 23000
#define ArmMinLimEncoder 0

#define Arm_tolerance 1000

//Wrist Values
#define Wrist_Intake -16500
#define Wrist_Switch -14000
#define Wrist_Scale_Level_1 -14750
#define Wrist_Folded 0
#define Wrist_Scale_Level_1_Back 10000
#define Wrist_Scale_Back_Lob 6900
//#define Wrist_Scale_Level_1_Back 14000
#define Wrist_Portal -14100

#define Wrist_Scale_Neutral -14000

#define Wrist_tolerance 150

#define WristMaxLimEncoder 22400
#define WristMinLimEncoder -17000

//PDP Channels
#define Arm_PDPChannel 4

#define Claw1_PDPChannel 10
#define Claw2_PDPChannel 11

#define Wrist_PDPChannel 5

#define Elevator_PDPChannel 2//Comp Bot:2
#define Elevator2_PDPChannel 3

#define Strafe1_PDPChannel 15
#define Strafe2_PDPChannel 14


#endif //DEFINES_H
