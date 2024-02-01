#pragma once
#include "main.h"

#include <stdio.h>
#include "robot.h"

#define MOTOR_RIGHT_TURN 90
#define CATAPULT_INITIAL_SPIN 82*3
#define CATAPULT_LAUNCH_SPIN 15*3
#define AUTON_COMMAND_DELAY 10
//wings
void setWings(bool state);
//Autons
void offensiveAuton();
void defensiveAuton();
void skillAuton();