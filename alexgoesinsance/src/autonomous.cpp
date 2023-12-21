#include "main.h"
#include <iostream>

int autonNormalSpeed = 100;

void forward(double units);
void forward(double units, int volts);

void sleft(double units);
void sleft(double units, int volts);

void sright(double units);
void sright(double units, int volts);

void offensiveAuton() {
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
}

void defensiveAuton() {
    
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