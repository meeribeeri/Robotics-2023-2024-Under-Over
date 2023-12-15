#include "main.h"
#include <iostream>
#include <stdio.h>
/*
Important information
use pros terminal
commands:
	pros c n [project name] (for a new project)
	pros make clean (clears make or something, its important if make doesn't do anything)
	pros build-compile-commands (project setup, use if headers fail)
	Add "compileCommands": "${workspaceFolder}/compile_commands.json" in c_cpp_properties.json if headers still fail
	pros mut (to upload to robot)
*/
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
// function override
//Test
//Test2.0

//variables
bool reverse = false;
int spin = 1150;
int launchSpin = 200;
int driveVoltageReduction = 0;
bool intakeReady = false;
bool manualCataControl = true;
bool currentPistonState = false;
int autonNormalSpeed = 100;

void forward(double units);
void forward(double units, int volts);

void sleft(double units);
void sleft(double units, int volts);

void sright(double units);
void sright(double units, int volts);

void updateScreen();

//controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

//For motors, first num is port, second is gear(rgb), third is reverse, 1 = reversed 0 = normal
	//higher port num is forward for the drive motors, reverse the lower ones
pros::Motor left_mtr_1(2,MOTOR_GEAR_BLUE,1);
pros::Motor left_mtr_2(3,MOTOR_GEAR_BLUE,0);
pros::Motor_Group left_motors({left_mtr_1,left_mtr_2});

pros::Motor right_mtr_1(4,MOTOR_GEAR_BLUE,0);
pros::Motor right_mtr_2(5,MOTOR_GEAR_BLUE,1);
pros::Motor_Group right_motors({right_mtr_1,right_mtr_2});

pros::Motor intake(7,MOTOR_GEAR_GREEN,false);
pros::Motor catapult(11,MOTOR_GEAR_RED,false);
//pneumatics
pros::ADIDigitalOut leftWing('A', currentPistonState);
pros::ADIDigitalOut rightWing('B',currentPistonState);



void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

	//port is from A-H, the bool is init state
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	left_motors.move(90);
	right_motors.move(90);
	intake.move(60);
	pros::delay(2100);
	left_motors.brake();
	right_motors.brake();
	
	
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	
	//For motors, first num is port, second is gear(rgb), third is reverse, 1 = reversed 0 = normal
	//higher port num is forward for the drive motors, reverse the lower ones
	/*pros::Motor left_mtr_1(1,MOTOR_GEAR_BLUE,0);
	pros::Motor left_mtr_2(2,MOTOR_GEAR_BLUE,1);
	pros::Motor_Group left_motors({left_mtr_1,left_mtr_2});

	pros::Motor right_mtr_1(3,MOTOR_GEAR_BLUE,0);
	pros::Motor right_mtr_2(4,MOTOR_GEAR_BLUE,1);
	pros::Motor_Group right_motors({right_mtr_1,right_mtr_2});

	pros::Motor intake(5,MOTOR_GEAR_GREEN,false);
	pros::Motor catapult(6,MOTOR_GEAR_RED,false);*/


	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		if (reverse) {
			left_motors.move(master.get_analog(ANALOG_LEFT_Y) + driveVoltageReduction);
			right_motors.move(-1*master.get_analog(ANALOG_RIGHT_Y) - driveVoltageReduction);
		} else {
			left_motors.move(-1*master.get_analog(ANALOG_LEFT_Y) - driveVoltageReduction);
			right_motors.move(master.get_analog(ANALOG_RIGHT_Y) + driveVoltageReduction);
		}

		if (master.get_digital_new_press(DIGITAL_Y)) {
			switch (reverse) {
				case false:
					reverse = true;
					break;
				case true:
					reverse = false;
					break;
			}
		}
		

		if (master.get_digital(DIGITAL_R1)) {
			intake.move(127);
			//std::cout << master.get_digital(DIGITAL_R1) << " " << master.get_digital(DIGITAL_L1) << "IN" << std::endl;
		} else if (master.get_digital(DIGITAL_L1)) {
			intake.move(-127);
			//std::cout << master.get_digital(DIGITAL_R1) << " " << master.get_digital(DIGITAL_L1) << "OUT" << std::endl;
		} else {
			//intake.move(0);
			intake.brake();
			//std::cout << "Stopping motors" << intake.is_stopped() << std::endl;
		}

		if (master.get_digital(DIGITAL_R2)) {
			catapult.move(127);
			intakeReady = false;
			manualCataControl = true;
		} else if (manualCataControl) {
			catapult.brake();
		}
		//pneumatics
		if (master.get_digital_new_press(DIGITAL_L2)) {
			switch (currentPistonState) {
				case true:
					currentPistonState = false;
					leftWing.set_value(false);
					rightWing.set_value(false);
					break;
				case false:
					currentPistonState = true;
					leftWing.set_value(true);
					rightWing.set_value(true);
					break;
			}
		}

		//launch Buttons
		if (master.get_digital_new_press(DIGITAL_A)) {
			if (!intakeReady) {
				manualCataControl = false;
				intakeReady = true;
				catapult.tare_position();
				catapult.move_absolute(spin,200);
			} else {
				catapult.tare_position();
				catapult.move_relative(launchSpin,100);
				intakeReady = false;
			}
		}

		if (master.get_digital_new_press(DIGITAL_UP) && driveVoltageReduction < 127) {
			driveVoltageReduction++;
		}

		if (master.get_digital_new_press(DIGITAL_DOWN) && driveVoltageReduction > 0) {
			driveVoltageReduction--;
		}
		//neko miko reimu aishiteru
		pros::delay(10);
	}
}

void forward(double units) {
	left_motors.move_relative(units, autonNormalSpeed);
	right_motors.move_relative(units,autonNormalSpeed);
}

void forward(double units, int vel) {
	left_motors.move_relative(units, vel);
	right_motors.move_relative(units,vel);
}

void sleft(double units) {
	left_motors.move_relative(units, autonNormalSpeed);
	right_motors.move_relative(-units,autonNormalSpeed);
}

void sleft(double units, int vel) {
	left_motors.move_relative(units, vel);
	right_motors.move_relative(-units,vel);
}

void sright(double units) {
	left_motors.move_relative(-units, autonNormalSpeed);
	right_motors.move_relative(units,autonNormalSpeed);
}

void sright(double units, int vel) {
	left_motors.move_relative(-units, vel);
	right_motors.move_relative(units,vel);
}