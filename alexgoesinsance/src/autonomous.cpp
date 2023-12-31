#include "robot.h"

int autonNormalSpeed = 100;

//components
pros::Controller master(pros::E_CONTROLLER_MASTER);

//For motors, first num is port, second is gear(rgb), third is reverse, 1 = reversed 0 = normal
	//higher port num is forward for the drive motors, reverse the lower ones
pros::Motor left_mtr_1(2,MOTOR_GEAR_BLUE,1,MOTOR_ENCODER_DEGREES);
pros::Motor left_mtr_2(3,MOTOR_GEAR_BLUE,0,MOTOR_ENCODER_DEGREES);
pros::Motor left_mtr_3(11,MOTOR_GEAR_BLUE,1,MOTOR_ENCODER_DEGREES);
pros::Motor_Group left_motors({left_mtr_1,left_mtr_2,left_mtr_3});

pros::Motor right_mtr_1(4,MOTOR_GEAR_BLUE,0,MOTOR_ENCODER_DEGREES);
pros::Motor right_mtr_2(5,MOTOR_GEAR_BLUE,1,MOTOR_ENCODER_DEGREES);
pros::Motor right_mtr_3(12,MOTOR_GEAR_BLUE,1,MOTOR_ENCODER_DEGREES);
pros::Motor_Group right_motors({right_mtr_1,right_mtr_2,right_mtr_3});

pros::Motor intake(7,MOTOR_GEAR_GREEN,false,MOTOR_ENCODER_DEGREES);
pros::Motor catapult(11,MOTOR_GEAR_RED,false,MOTOR_ENCODER_DEGREES);
//pneumatics
pros::ADIDigitalOut leftWing('A', false);
pros::ADIDigitalOut rightWing('B',false);
//vision sensor for autons
pros::Vision vision(20,pros::E_VISION_ZERO_CENTER);
//auton functions used to not repeat code
void forward(double units);
void forward(double units, int volts);

void sleft(double units);
void sleft(double units, int volts);

void sright(double units);
void sright(double units, int volts);

//auton functions for the actual auton stuff
void offensiveAuton() { //shoot side
	left_motors.move(90);
	right_motors.move(-90);
	intake.move(90);
	pros::delay(2100);
	left_motors.brake();
	right_motors.brake();
	pros::delay(1000);
	intake.brake();
	left_motors.move(90);
	right_motors.move(90);
	pros::delay(1000);
	left_motors.move(90);
	right_motors.move(-90);
	pros::delay(2100);
	left_motors.brake();
	right_motors.brake();
    setWings(true);
}

void defensiveAuton() { //net side
    //drive forward, push preload into net, grab another ball, push another into net
    //drive forward
    forward(1200);
    forward(-360);
    sleft(MOTOR_RIGHT_TURN / 2);
    intake.move(75);
    forward(3000);
    intake.brake();
    sright(MOTOR_RIGHT_TURN / 2);
    forward(270);
    sright(MOTOR_RIGHT_TURN);
    forward(3000);
    intake.move(-75);
    pros::delay(1000);
    intake.brake();
    forward(-500);


}

void skillAuton() {
    setWings(true);
	for (int i = 0; i < 45; i++) {
		catapult.move_relative(165*3,100);
		pros::delay(700);
		catapult.move_relative(15*3,100);
		pros::delay(300);
	}
    setWings(false);
    forward(3000);
    sright(MOTOR_RIGHT_TURN / 2);
    forward(750);
    setWings(true);
    forward(3000);
	//drive to the middle barrier & determine where to best go for the most triballs pushed in
}


void forward(double units) {
	left_motors.move_relative(units, autonNormalSpeed);
	right_motors.move_relative(-units,autonNormalSpeed);
}

void forward(double units, int vel) {
	left_motors.move_relative(units, vel);
	right_motors.move_relative(-units,vel);
}

void sleft(double units) {
	left_motors.move_relative(units, autonNormalSpeed);
	right_motors.move_relative(units,autonNormalSpeed);
}

void sleft(double units, int vel) {
	left_motors.move_relative(units, vel);
	right_motors.move_relative(units,vel);
}

void sright(double units) {
	left_motors.move_relative(-units, autonNormalSpeed);
	right_motors.move_relative(-units,autonNormalSpeed);
}

void sright(double units, int vel) {
	left_motors.move_relative(-units, vel);
	right_motors.move_relative(-units,vel);
}

void setWings(bool state) {
    leftWing.set_value(state);
    rightWing.set_value(state);
}