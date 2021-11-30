/*
 * Astu Tanks
 * 
 * Copyright (c) 2021 Tobias Filler. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include <AstuGraphics.h>
#include <AstuInput.h>

/////////////////////////////////////////////////
/////// General constants
/////////////////////////////////////////////////
#define VIEW_WIDTH                  16.0f
#define VIEW_HEIGHT                 9.0f
#define BOUNDARY_THICKNESS          1.0f
#define BACKGROUND_COLOR            RalColors::TrafficBlack
#define BOUNDARY_COLOR              RalColors::TrafficWhite

/////////////////////////////////////////////////
/////// Physics constants
/////////////////////////////////////////////////
#define GRAVITY                     0.0f
#define GENERAL_RESTITUTION         0.5f
#define GENERAL_FRICTION            0.5f

/////////////////////////////////////////////////
/////// Gamemode constants
/////////////////////////////////////////////////

#define STARTING_LIFES              3
#define TEXT_COLOR                  RalColors::TrafficGreyA
#define FINISH_LEVEL_POINTS         25
/////////////////////////////////////////////////
/////// Constants for game entities
/////////////////////////////////////////////////

// TAnk Constants

#define TANK_LINEAR_DAMPING         5.0f
#define TANK_ANGULAR_DAMPING        15.0f
#define TANK_MAX_TORQUE             0.6f
#define TANK_MAX_THRUST             2.0f
#define TANK_POINTS                 0
#define PLAYER1_COLOR               RalColors::SkyBlue
#define PLAYER2_COLOR               RalColors::RaspberryRed

// Wall Constants

#define WALL_COLOR                  RalColors::TrafficWhite
#define WALL_THICKNESS              0.5f

//Length of the different wall segments
#define WALL_LARGE                  16.0f
#define WALL_MEDIUM_LARGE           8.0f
#define WALL_MEDIUM                 4.0f
#define WALL_SMALL                  2.0f
#define WALL_CORNER                 0.5f

// Bullet Constants

#define BULLET_COLOR                RalColors::TrafficWhite
#define BULLET_RADIUS               0.04f
#define BULLET_MAX_DEFELTIONS       2

// Gun Constans

#define GUN_COOLDOWN                0.1f
#define GUN_MUZZLE_VELOCITY         4.0f
#define GUN_MOUNT_POINT             astu::Vector2f(0.0f, 0.5f)
#define GUN_DIRECTION               astu::Vector2f(0.0f, 1.0f)

// Box Constans

#define BOX_SIZE                    0.5f
#define BOX_COLOR                   RalColors::BrownBeige
#define BOX_POINTS                  1

// Turret Constans

#define TURRET_POINTS               5
#define TURRET_COLOR                RalColors::WhiteAluminium
#define TURRET_RANGE                5.0f
#define TURRET_SMALL_RANGE          3.0f
#define TURRET_IDLE_SPEED           1.5f
#define TURRET_ROTATION_SPEED       2.5f
#define TURRET_OFFSET               0.01f // radiants