#pragma once
#include "main.h"

#include <stdio.h>
#include "robot.h"

#define MOTOR_RIGHT_TURN 90
//CATAPULT_INITIAL_SPIN and CATAPULT_LAUNCH_SPIN should total to 270 degrees
#define CATAPULT_INITIAL_SPIN 222
#define CATAPULT_LAUNCH_SPIN 48
#define AUTON_COMMAND_DELAY 10
//wings
void setWings(bool state);
//Autons
void offensiveAuton();
void defensiveAuton();
void skillAuton();