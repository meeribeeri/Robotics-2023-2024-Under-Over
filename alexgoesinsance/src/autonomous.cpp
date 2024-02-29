#include "robot.h"

int autonNormalSpeed = 100;

//components
pros::Controller master(pros::E_CONTROLLER_MASTER);

//For motors, first num is port, second is gear(rgb), third is reverse, 1 = reversed 0 = normal
	//higher port num is forward for the drive motors, reverse the lower ones
pros::Motor left_mtr_1(1,MOTOR_GEAR_BLUE,0,MOTOR_ENCODER_DEGREES);
pros::Motor left_mtr_2(14,MOTOR_GEAR_BLUE,1,MOTOR_ENCODER_DEGREES);
pros::Motor left_mtr_3(15,MOTOR_GEAR_BLUE,1,MOTOR_ENCODER_DEGREES);
pros::Motor_Group left_motors({left_mtr_1,left_mtr_2,left_mtr_3});

pros::Motor right_mtr_1(7,MOTOR_GEAR_BLUE,0,MOTOR_ENCODER_DEGREES);
pros::Motor right_mtr_2(8,MOTOR_GEAR_BLUE,1,MOTOR_ENCODER_DEGREES);
pros::Motor right_mtr_3(9,MOTOR_GEAR_BLUE,1,MOTOR_ENCODER_DEGREES);
pros::Motor_Group right_motors({right_mtr_1,right_mtr_2,right_mtr_3});

pros::Motor intake(10,MOTOR_GEAR_GREEN,false,MOTOR_ENCODER_DEGREES);
pros::Motor catapult(11,MOTOR_GEAR_RED,false,MOTOR_ENCODER_DEGREES);
//pneumatics
pros::ADIDigitalOut leftWing('A', false);
pros::ADIDigitalOut rightWing('B',false);
//vision sensor for autons
//pros::Vision vision(20,pros::E_VISION_ZERO_CENTER);
int joke = 0;
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
	sright(MOTOR_RIGHT_TURN*2);
	pros::delay(AUTON_COMMAND_DELAY);
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
    setWings(false);
	intake.move_relative(360,autonNormalSpeed);
	catapult.move(CATAPULT_SPIN_VOLTAGE);
	pros::delay(42*1000);
    setWings(false);
    //forward();
	//drive to the middle beam
	forward(3600);
	pros::delay(1000);
	sleft(MOTOR_RIGHT_TURN/2);
	//push triballs to the side
	setRightWings(true);
	forward(3600);
	pros::delay(750);
	forward(-720);
	pros::delay(500);
	forward(720);
	pros::delay(500);
	forward(-720);
	setRightWings(false);
	pros::delay(500);
	sleft(MOTOR_RIGHT_TURN);
	forward(1000);
	pros::delay(750);
	sright(MOTOR_RIGHT_TURN);
	forward(720);
	pros::delay(750);
	sright(MOTOR_RIGHT_TURN);
	forward(1500);
	sright(MOTOR_RIGHT_TURN);
	setWings(true);
	forward(1500);
	pros::delay(1000);
	setWings(false);
	sright(MOTOR_RIGHT_TURN);
	forward(1200);
	pros::delay(1000);
	sleft(MOTOR_RIGHT_TURN);
	forward(750);
	pros::delay(750);
	sleft(MOTOR_RIGHT_TURN);
	setWings(true);
	forward(2000);
	pros::delay(1000);
	forward(-1000);
	pros::delay(750);
	forward(1000);
	pros::delay(1000);
	forward(-1000);
	pros::delay(750);
	//curved turn
	setRightWings(false);
	left_motors.move(80);
	right_motors.move(120);
	pros::delay(1250);
	left_motors.brake();
	right_motors.brake();
	pros::delay(AUTON_COMMAND_DELAY);
	setRightWings(true);
	//sleft(MOTOR_RIGHT_TURN/3);
	//sright(MOTOR_RIGHT_TURN/3);
	forward(1000);
	pros::delay(750);
	forward(-1000);
	pros::delay(750);
	forward(1000);


	//drive to the side entrance & push as many balls as possible in
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
	pros::delay(300);
}

void sleft(double units, int vel) {
	left_motors.move_relative(units, vel);
	right_motors.move_relative(units,vel);
	pros::delay(300);
}

void sright(double units) {
	left_motors.move_relative(-units, autonNormalSpeed);
	right_motors.move_relative(-units,autonNormalSpeed);
	pros::delay(300);
}

void sright(double units, int vel) {
	left_motors.move_relative(-units, vel);
	right_motors.move_relative(-units,vel);
	pros::delay(300);
}

void setWings(bool state) {
    leftWing.set_value(state);
    rightWing.set_value(state);
}

void setLeftWings(bool state) {
	leftWing.set_value(state);
}

void setRightWings(bool state) {
	rightWing.set_value(state);
}


void inputTimer() {
	while (true) {
		if (joke > 0) {
			pros::delay(250);
			joke = 0;
		}
		pros::delay(20);
	}
}