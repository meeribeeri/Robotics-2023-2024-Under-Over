#include "robot.h"

/*
Important information
use pros terminal
	pros c n [project name] (for a new project)
	pros make clean (clears make or something, its important if make doesn't do anything)
	pros make : builds the program
	pros build-compile-commands (project setup, use if headers fail)
	Add "compileCommands": "${workspaceFolder}/compile_commands.json" in c_cpp_properties.json if headers still fail
	pros mut (to upload to robot)
*/
// function override

//components
extern pros::Controller master;
extern pros::Motor_Group left_motors;
extern pros::Motor_Group right_motors;
extern pros::Motor intake;
extern pros::Motor catapult;
extern pros::Vision vision;
//variables
bool reverse = false;
double driveVoltagePercent = 1.00;
bool intakeReady = false;
bool manualCataControl = true;
bool currentPistonState = false;
int autonNumber = 0;

void elevationWarning(void* param);

void competitionsAuton();
void skillAuton();
//controller

void autonChangeSkill() {
	pros::lcd::clear_line(2);
	pros::lcd::set_text(2, "Skills Auton Selected");
	autonNumber = 2;
}

void autonChangeOffence() {
	pros::lcd::clear_line(2);
	pros::lcd::set_text(2, "Offensive(Shoot Side) Auton Selected");
	autonNumber = 0;
}

void autonChangeDefence() {
	pros::lcd::clear_line(2);
	pros::lcd::set_text(2, "Defensive(Net Side) Auton Selected");
	autonNumber = 1;
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();

	pros::lcd::register_btn0_cb(autonChangeOffence);
	pros::lcd::register_btn1_cb(autonChangeDefence);
	pros::lcd::register_btn2_cb(autonChangeSkill);

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
void competition_initialize() {

	//pros::lcd::register_btn0_cb
}

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
	//use pros make, then pros upload --slot 2
	//this is only for skill code, comp code use pros mut
	switch (autonNumber) {
		case 0:
			offensiveAuton();
			break;
		case 1:
			defensiveAuton();
			break;
		case 2:
			skillAuton();
			break;

	}
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
	
	pros::Task task(elevationWarning,(void*)"null");

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		if (reverse) {
			left_motors.move((int)(master.get_analog(ANALOG_LEFT_Y) * driveVoltagePercent + 0.5));
			right_motors.move((int)(-1*master.get_analog(ANALOG_RIGHT_Y) * driveVoltagePercent + 0.5));
		} else {
			left_motors.move((int)(-1*master.get_analog(ANALOG_LEFT_Y) * driveVoltagePercent));
			right_motors.move((int)(master.get_analog(ANALOG_RIGHT_Y) * driveVoltagePercent));
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
					break;
				case false:
					currentPistonState = true;
					break;
			}
			setWings(currentPistonState);
		}

		//launch Buttons
		if (master.get_digital_new_press(DIGITAL_A)) {
			if (!intakeReady) {
				manualCataControl = false;
				intakeReady = true;
				catapult.tare_position();
				catapult.move_relative(CATAPULT_INITIAL_SPIN,200);
			} else {
				catapult.tare_position();
				catapult.move_relative(CATAPULT_LAUNCH_SPIN,100);
				intakeReady = false;
			}
		}

		if (master.get_digital_new_press(DIGITAL_UP) && driveVoltagePercent < 1) {
			driveVoltagePercent = driveVoltagePercent + 0.05;
		}

		if (master.get_digital_new_press(DIGITAL_DOWN) && driveVoltagePercent > 0.05) {
			driveVoltagePercent = driveVoltagePercent - 0.05;
		}
		if (master.get_digital_new_press(DIGITAL_X)) {
			driveVoltagePercent = 1.00;
		}
		//neko miko reimu aishiteru
		pros::delay(10);
	}
}

void elevationWarning(void* param) {
	pros::delay((60+35)*1000);
	master.rumble("- -");
	pros::delay(5*1000);
	master.rumble("--------");
}
