/*
Life counter
June 1, 2017
Copyright (C) 2017 Adam Bielinski
All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
*/

#include "Arduboy.h"


// Make an instance of arduboy used for many functions
Arduboy arduboy;

// Random horribly organized variables here
char text[] = "asdf";
signed int score;
signed int diff;
int frame_counter;
bool pressing;
bool timer_pressed = false;
bool lost_game;
const int num_sub_scores = 6;
int cur_subscore = 0;
int subscores[num_sub_scores] = { 0 }; // all elements 0
byte ALL_DIR_BUTTONS = RIGHT_BUTTON + LEFT_BUTTON + UP_BUTTON + DOWN_BUTTON;
byte ALL_BUTTONS = RIGHT_BUTTON + LEFT_BUTTON + UP_BUTTON + DOWN_BUTTON + A_BUTTON + B_BUTTON;
static uint32_t StartTime; // Start time when timer is running
static uint32_t EndTime; // End time when timer stops
int TimerState = 0;

// not sure how correct this is
const byte start_game_sound [] = {
   0,166, 0x80, 0x81, 0x82, 0x90,0x53, 0x91,0x3F, 0,166, 0x81, 0x91,0x40, 0,166, 0x81, 0x91,0x3F, 0x92,0x42
};

// I don't know where this came from, but was included as an example for aduboy
const byte PROGMEM lost_game_sound [] = {
  // no idea what this means
  7,208, 0x90,0x45, 0x91,0x39, 1,77, 0x80, 0x81, 0x90,0x44, 0,166, 0x80, 0x90,0x45, 0,166, 0x80, 0x90,0x47,
  0x91,0x38, 1,77, 0x80, 0x81, 0x90,0x45, 0,166, 0x80, 0x90,0x44, 0,166, 0x80, 0x90,0x45, 0x91,0x36, 1,77,
  0x81, 1,77, 0x91,0x31, 0,166, 0x80, 0x90,0x47, 0,166, 0x80, 0x81, 0x90,0x44, 0,166, 0x80, 0x90,0x45,
  0,166, 0x80, 0x90,0x47, 0x91,0x32, 0,166, 0x80, 0x90,0x40, 0,166, 0x80, 0x81, 0x90,0x49, 0,166, 0x80,
  0x90,0x40, 0,166, 0x80, 0x90,0x4A, 0x91,0x34, 0,166, 0x80, 0x90,0x40, 0,166, 0x80, 0x81, 0x90,0x4C, 1,77,
  0x80, 0x90,0x49, 0x91,0x2D, 0,166, 0x80, 0x90,0x47, 0,166, 0x80, 0x81, 0x90,0x45, 0x91,0x39, 0,166, 0x80,
  0x90,0x47, 0,166, 0x80, 0x81, 0x90,0x49, 0x91,0x38, 0,166, 0x80, 0x90,0x4B, 0,166, 0x80, 0x81, 0x90,0x4C,
  0x91,0x36, 0,166, 0x80, 0x90,0x4E, 0,166, 0x80, 0x81, 0x90,0x50, 0x91,0x34, 0x92,0x40, 0,166, 0x80, 0x90,0x51,
  0,166, 0x80, 0x81, 0x82, 0x90,0x53, 0x91,0x3F, 0,166, 0x81, 0x91,0x40, 0,166, 0x81, 0x91,0x3F, 0x92,0x42,
  0,166, 0x80, 0x90,0x51, 0,166, 0x80, 0x81, 0x82, 0x90,0x50, 0x91,0x40, 0,166, 0x80, 0x81, 0x90,0x4E, 0x91,0x3F,
  0,166, 0x80, 0x81, 0x90,0x4C, 0x91,0x3D, 0x92,0x40, 0,166, 0x80, 0x90,0x4B, 0,166, 0x80, 0x81, 0x90,0x49,
  0,166, 0x80, 0x90,0x47, 0,166, 0x80, 0x90,0x4C, 0x91,0x38, 0,166, 0x82, 0x92,0x42, 0,166, 0x81, 0x82,
  0x91,0x3F, 0,166, 0x81, 0x91,0x40, 0,166, 0x81, 0x91,0x42, 0x92,0x39, 0,166, 0x81, 0x91,0x3B, 0,166,
  0x81, 0x82, 0x91,0x44, 0,166, 0x81, 0x91,0x3B, 0,166, 0x80, 0x81, 0x90,0x45, 0x91,0x4B, 0x92,0x3B, 0,166,
  0x80, 0x82, 0x90,0x3B, 0,166, 0x80, 0x90,0x47, 1,77, 0x80, 0x81, 0x90,0x4C, 0x91,0x44, 0x92,0x40, 0,166,
  0x81, 0x91,0x45, 0,166, 0x80, 0x81, 0x82, 0x90,0x4A, 0x91,0x47, 0,166, 0x80, 0x81, 0x90,0x49, 0x91,0x45, 0,166,
  0x80, 0x81, 0x90,0x4A, 0x91,0x44, 0,166, 0x80, 0x81, 0x90,0x4C, 0x91,0x42, 0,166, 0x80, 0x81, 0x90,0x4E, 0x91,0x40,
  0,166, 0x80, 0x81, 0x90,0x50, 0x91,0x3E, 0,166, 0x80, 0x81, 0x90,0x51, 0x91,0x3D, 0x92,0x39, 0,166, 0x81,
  0x91,0x49, 0,166, 0x81, 0x82, 0x91,0x4E, 0x92,0x38, 0,166, 0x80, 0x82, 0x90,0x51, 0x92,0x39, 0,166, 0x80,
  0x82, 0x90,0x50, 0x92,0x3B, 0,166, 0x81, 0x91,0x4B, 0,166, 0x81, 0x82, 0x91,0x4C, 0x92,0x39, 0,166, 0x80,
  0x82, 0x90,0x50, 0x92,0x38, 0,166, 0x80, 0x82, 0x90,0x4E, 0x92,0x39, 0,166, 0x81, 0x91,0x49, 0,166, 0x81,
  0x91,0x4A, 0,166, 0x80, 0x90,0x4E, 0,166, 0x80, 0x90,0x4C, 0,166, 0x81, 0x82, 0x91,0x47, 0x92,0x3B, 0,166,
  0x81, 0x82, 0x91,0x49, 0x92,0x38, 0,166, 0x80, 0x82, 0x90,0x4C, 0x92,0x39, 0,166, 0x80, 0x81, 0x82, 0x90,0x3B,
  0x91,0x4A, 0x92,0x44, 0,166, 0x80, 0x90,0x34, 0,166, 0x80, 0x81, 0x82, 0x90,0x3D, 0x91,0x49, 0x92,0x45, 0,166,
  0x80, 0x90,0x34, 0,166, 0x80, 0x81, 0x90,0x3E, 0x91,0x47, 0,166, 0x80, 0x90,0x34, 0,166, 0x80, 0x82, 0x90,0x44,
  0x92,0x40, 1,77, 0x80, 0x81, 0x82, 0x90,0x3D, 0x91,0x45, 0,166, 0x80, 0x90,0x3B, 0,166, 0x80, 0x90,0x39,
  0x92,0x49, 0,166, 0x80, 0x90,0x3B, 0,166, 0x80, 0x82, 0x90,0x3D, 0x92,0x4E, 0,166, 0x80, 0x90,0x3F, 0,166,
  0x80, 0x81, 0x90,0x45, 0x91,0x40, 0,166, 0x81, 0x91,0x42, 0,166, 0x80, 0x81, 0x90,0x44, 0x91,0x3B, 0,166,
  0x81, 0x91,0x39, 0,166, 0x81, 0x82, 0x91,0x47, 0x92,0x38, 0,166, 0x82, 0x92,0x39, 0,166, 0x81, 0x82, 0x91,0x4C,
  0x92,0x3B, 0,166, 0x82, 0x92,0x3D, 0,166, 0x80, 0x82, 0x90,0x44, 0x92,0x3F, 0,166, 0x82, 0x92,0x40, 0,166,
  0x80, 0x82, 0x90,0x42, 0x92,0x39, 0,166, 0x82, 0x92,0x38, 0,166, 0x81, 0x82, 0x91,0x4B, 0x92,0x36, 0,166,
  0x81, 0x82, 0x91,0x4C, 0x92,0x38, 0,166, 0x81, 0x82, 0x91,0x4E, 0x92,0x39, 0,166, 0x82, 0x92,0x38, 0,166,
  0x81, 0x82, 0x91,0x4C, 0x92,0x39, 0,166, 0x81, 0x82, 0x91,0x4B, 0x92,0x3B, 0,166, 0x80, 0x81, 0x82, 0x90,0x4C,
  0x91,0x38, 0,166, 0x92,0x4B, 0,166, 0x81, 0x82, 0x91,0x49, 0x92,0x39, 0,166, 0x81, 0x82, 0x91,0x47, 0x92,0x3B,
  0,166, 0x81, 0x82, 0x91,0x45, 0x92,0x3D, 0,166, 0x80, 0x81, 0x90,0x4E, 0x91,0x44, 0,166, 0x80, 0x81, 0x90,0x4B,
  0x91,0x42, 0,166, 0x80, 0x81, 0x90,0x4C, 0x91,0x40, 0,166, 0x80, 0x81, 0x90,0x4E, 0x91,0x3F, 0,166, 0x80,
  0x82, 0x90,0x47, 0x92,0x3B, 0,166, 0x80, 0x81, 0x82, 0x90,0x50, 0x91,0x39, 0x92,0x40, 0,166, 0x80, 0x81, 0x90,0x47,
  0x91,0x38, 0,166, 0x80, 0x81, 0x90,0x51, 0x91,0x36, 0,166, 0x80, 0x90,0x47, 0,166, 0x80, 0x81, 0x82, 0x90,0x53,
  0x91,0x3F, 0x92,0x3B, 1,77, 0x80, 0x81, 0x82, 0x90,0x50, 0x91,0x34, 0x92,0x40, 0,166, 0x80, 0x90,0x51, 0,166,
  0x80, 0x90,0x53, 0,166, 0x80, 0x90,0x51, 0,166, 0x80, 0x81, 0x82, 0x90,0x50, 0,166, 0x80, 0x90,0x4E, 0x91,0x39,
  0,166, 0x80, 0x81, 0x90,0x4C, 0x91,0x38, 0,166, 0x80, 0x81, 0x90,0x4A, 0x91,0x36, 0,166, 0x80, 0x81, 0x90,0x49,
  0x91,0x35, 0,166, 0x81, 0x91,0x31, 0,166, 0x81, 0x91,0x44, 0x92,0x36, 0,166, 0x81, 0x82, 0x91,0x45, 0x92,0x31,
  0,166, 0x81, 0x82, 0x91,0x47, 0x92,0x38, 0,166, 0x82, 0x92,0x31, 0,166, 0x81, 0x82, 0x91,0x45, 0x92,0x35,
  0,166, 0x81, 0x82, 0x91,0x44, 0x92,0x31, 0,166, 0x81, 0x82, 0x91,0x45, 0x92,0x36, 0,166, 0x82, 0x92,0x31,
  0,166, 0x80, 0x82, 0x90,0x38, 0x92,0x49, 0,166, 0x80, 0x82, 0x90,0x31, 0x92,0x4A, 0,166, 0x80, 0x82, 0x90,0x36,
  0x92,0x4C, 0,166, 0x80, 0x90,0x31, 0,166, 0x80, 0x82, 0x90,0x34, 0x92,0x4A, 0,166, 0x80, 0x82, 0x90,0x31,
  0x92,0x49, 0,166, 0x80, 0x82, 0x90,0x33, 0x92,0x47, 0,166, 0x80, 0x90,0x2F, 0,166, 0x80, 0x81, 0x90,0x34,
  0x91,0x42, 0,166, 0x80, 0x81, 0x90,0x2F, 0x91,0x44, 0,166, 0x80, 0x81, 0x90,0x36, 0x91,0x45, 0,166, 0x80,
  0x90,0x2F, 0,166, 0x80, 0x81, 0x90,0x33, 0x91,0x44, 0,166, 0x80, 0x81, 0x90,0x2F, 0x91,0x42, 0,166, 0x80,
  0x81, 0x90,0x44, 0x91,0x34, 0,166, 0x81, 0x91,0x2F, 0,166, 0x81, 0x82, 0x91,0x47, 0x92,0x36, 0,166, 0x81,
  0x82, 0x91,0x49, 0x92,0x2F, 0,166, 0x81, 0x82, 0x91,0x4A, 0x92,0x34, 0,166, 0x82, 0x92,0x2F, 0,166, 0x81,
  0x82, 0x91,0x49, 0x92,0x32, 0,166, 0x81, 0x82, 0x91,0x47, 0x92,0x2F, 0,166, 0x81, 0x82, 0x91,0x45, 0x92,0x31,
  0,166, 0x82, 0x92,0x2D, 0,166, 0x80, 0x82, 0x90,0x32, 0x92,0x41, 0,166, 0x80, 0x82, 0x90,0x42, 0x92,0x2D,
  0,166, 0x80, 0x82, 0x90,0x44, 0x92,0x34, 0,166, 0x82, 0x92,0x2D, 0,166, 0x80, 0x82, 0x90,0x42, 0x92,0x31,
  0,166, 0x80, 0x82, 0x90,0x2D, 0x92,0x41, 0,166, 0x80, 0x82, 0x90,0x42, 0x92,0x32, 0,166, 0x82, 0x92,0x2D,
  0,166, 0x81, 0x82, 0x91,0x34, 0x92,0x45, 0,166, 0x81, 0x82, 0x91,0x2D, 0x92,0x47, 0,166, 0x81, 0x82, 0x91,0x32,
  0x92,0x49, 0,166, 0x81, 0x91,0x2D, 0,166, 0x81, 0x82, 0x91,0x31, 0x92,0x47, 0,166, 0x81, 0x82, 0x91,0x2D,
  0x92,0x45, 0,166, 0x81, 0x82, 0x91,0x30, 0x92,0x44, 0,166, 0x81, 0x91,0x2C, 0,166, 0x80, 0x81, 0x90,0x31,
  0x91,0x3F, 0,166, 0x80, 0x81, 0x90,0x2C, 0x91,0x41, 0,166, 0x80, 0x81, 0x90,0x33, 0x91,0x42, 0,166, 0x80,
  0x90,0x2C, 0,166, 0x80, 0x81, 0x90,0x30, 0x91,0x41, 0,166, 0x80, 0x81, 0x90,0x2C, 0x91,0x3F, 0,166, 0x80,
  0x81, 0x90,0x31, 0x91,0x41, 1,77, 0x81, 0x82, 0x91,0x44, 0x92,0x41, 0,166, 0x81, 0x82, 0x91,0x45, 0x92,0x42,
  0,166, 0x81, 0x82, 0x91,0x47, 0x92,0x44, 1,77, 0x81, 0x82, 0x91,0x45, 0x92,0x42, 0,166, 0x81, 0x82, 0x91,0x44,
  0x92,0x41, 0,166, 0x81, 0x82, 0x91,0x45, 0x92,0x42, 1,77, 0x81, 0x91,0x46, 0,166, 0x81, 0x91,0x48, 0,166,
  0x81, 0x82, 0x91,0x49, 0x92,0x40, 1,77, 0x81, 0x91,0x48, 0,166, 0x81, 0x91,0x46, 0,166, 0x81, 0x82, 0x91,0x3F,
  0x92,0x48, 1,77, 0x82, 0x92,0x48, 0,166, 0x82, 0x92,0x49, 0,166, 0x81, 0x82, 0x91,0x4B, 0x92,0x42, 1,77,
  0x81, 0x91,0x49, 0,166, 0x81, 0x91,0x48, 0,166, 0x80, 0x81, 0x90,0x49, 0,166, 0x91,0x3E, 0,166, 0x80,
  0x81, 0x90,0x44, 0x91,0x3D, 0,166, 0x80, 0x81, 0x90,0x45, 0x91,0x3B, 0,166, 0x80, 0x81, 0x82, 0x90,0x47, 0x91,0x3D,
  0x92,0x41, 0,166, 0x81, 0x91,0x39, 0,166, 0x80, 0x81, 0x90,0x45, 0x91,0x3B, 0,166, 0x80, 0x81, 0x90,0x44,
  0x91,0x3D, 0,166, 0x80, 0x81, 0x82, 0x90,0x45, 0x91,0x42, 0x92,0x36, 0,166, 0x80, 0x81, 0x90,0x42, 0,166,
  0x80, 0x82, 0x90,0x44, 0x91,0x35, 0,166, 0x80, 0x81, 0x90,0x45, 0x91,0x36, 0,166, 0x80, 0x81, 0x90,0x47, 0x91,0x38,
  0,166, 0x80, 0x90,0x49, 0,166, 0x80, 0x81, 0x90,0x4A, 0x91,0x36, 0,166, 0x80, 0x81, 0x90,0x4C, 0x91,0x35,
  0,166, 0x80, 0x81, 0x90,0x4A, 0x91,0x36, 0,166, 0x92,0x49, 0,166, 0x82, 0x92,0x47, 0,166, 0x80, 0x90,0x4A,
  0,166, 0x80, 0x90,0x49, 0,166, 0x81, 0x82, 0x91,0x47, 0x92,0x38, 0,166, 0x81, 0x82, 0x91,0x45, 0x92,0x35,
  0,166, 0x80, 0x82, 0x90,0x49, 0x92,0x36, 0,166, 0x80, 0x81, 0x82, 0x90,0x38, 0x91,0x47, 0x92,0x41, 0,166,
  0x80, 0x90,0x31, 0,166, 0x80, 0x81, 0x82, 0x90,0x39, 0x91,0x42, 0x92,0x45, 0,166, 0x80, 0x90,0x31, 0,166,
  0x80, 0x82, 0x90,0x3B, 0x92,0x44, 0,166, 0x80, 0x90,0x31, 0,166, 0x80, 0x81, 0x90,0x3D, 0x91,0x41, 0,166,
  0x80, 0x90,0x3B, 0,166, 0x80, 0x81, 0x82, 0x90,0x39, 0x91,0x42, 0,166, 0x80, 0x90,0x3B, 0,166, 0x80, 0x90,0x3D,
  0x92,0x49, 0,166, 0x80, 0x90,0x3B, 0,166, 0x80, 0x82, 0x90,0x39, 0x92,0x4E, 0,166, 0x80, 0x90,0x38, 0,166,
  0x80, 0x81, 0x90,0x36, 0x91,0x45, 0,166, 0x80, 0x90,0x34, 0,166, 0x80, 0x81, 0x90,0x4A, 0x91,0x3B, 0,166,
  0x81, 0x91,0x3D, 0,166, 0x81, 0x82, 0x91,0x44, 0x92,0x3E, 0,166, 0x82, 0x92,0x3D, 0,166, 0x81, 0x82, 0x91,0x50,
  0x92,0x3B, 0,166, 0x82, 0x92,0x39, 0,166, 0x80, 0x82, 0x90,0x44, 0x92,0x38, 0,166, 0x82, 0x92,0x36, 0,166,
  0x80, 0x82, 0x90,0x4C, 0x92,0x3D, 0,166, 0x82, 0x92,0x3E, 0,166, 0x81, 0x82, 0x91,0x49, 0x92,0x40, 0,166,
  0x82, 0x92,0x3E, 0,166, 0x81, 0x82, 0x91,0x51, 0x92,0x3D, 0,166, 0x82, 0x92,0x3B, 0,166, 0x80, 0x82, 0x90,0x49,
  0x92,0x39, 0,166, 0x82, 0x92,0x38, 0,166, 0x80, 0x82, 0x90,0x3F, 0x92,0x4E, 0,166, 0x80, 0x90,0x40, 0,166,
  0x80, 0x81, 0x90,0x42, 0x91,0x4B, 0,166, 0x80, 0x90,0x40, 0,166, 0x80, 0x81, 0x90,0x3F, 0x91,0x53, 0,166,
  0x80, 0x90,0x3D, 0,166, 0x80, 0x81, 0x90,0x3B, 0x91,0x51, 0,166, 0x80, 0x90,0x39, 0,166, 0x80, 0x81, 0x90,0x50,
  0x91,0x40, 1,77, 0x80, 0x82, 0x90,0x50, 0x92,0x47, 0,166, 0x80, 0x82, 0x90,0x51, 0x92,0x49, 0,166, 0x80,
  0x82, 0x90,0x53, 0x92,0x4A, 1,77, 0x80, 0x82, 0x90,0x51, 0x92,0x49, 0,166, 0x80, 0x82, 0x90,0x50, 0x92,0x47,
  0,166, 0x80, 0x82, 0x90,0x51, 0x92,0x49, 1,77, 0x82, 0x92,0x49, 0,166, 0x82, 0x92,0x4B, 0,166, 0x80,
  0x82, 0x90,0x50, 0x92,0x4C, 1,77, 0x82, 0x92,0x4B, 0,166, 0x82, 0x92,0x49, 0,166, 0x80, 0x82, 0x90,0x4E,
  0x92,0x4B, 1,77, 0x82, 0x92,0x4B, 0,166, 0x82, 0x92,0x4C, 0,166, 0x80, 0x82, 0x90,0x51, 0x92,0x4E, 1,77,
  0x82, 0x92,0x4C, 0,166, 0x82, 0x92,0x4B, 0,166, 0x82, 0x92,0x4C, 0,166, 0x81, 0x91,0x3D, 0,166, 0x80,
  0x81, 0x90,0x50, 0x91,0x3B, 0,166, 0x80, 0x81, 0x90,0x51, 0x91,0x39, 0,166, 0x80, 0x81, 0x90,0x53, 0x91,0x38,
  0,166, 0x80, 0x81, 0x90,0x51, 0x91,0x36, 0,166, 0x80, 0x81, 0x90,0x50, 0x91,0x34, 0,166, 0x80, 0x81, 0x90,0x4E,
  0x91,0x32, 0,166, 0x80, 0x81, 0x82, 0x90,0x4C, 0x91,0x31, 0,166, 0x80, 0x81, 0x90,0x4A, 0x91,0x39, 0,166,
  0x80, 0x81, 0x90,0x49, 0x91,0x32, 0,166, 0x80, 0x81, 0x90,0x47, 0x91,0x39, 0,166, 0x80, 0x81, 0x90,0x45, 0x91,0x34,
  0,166, 0x80, 0x81, 0x90,0x49, 0x91,0x39, 0,166, 0x80, 0x81, 0x90,0x4C, 0x91,0x31, 0,166, 0x80, 0x81, 0x90,0x4F,
  0x91,0x39, 0,166, 0x80, 0x81, 0x90,0x32, 0x91,0x4E, 0,166, 0x80, 0x90,0x39, 0,166, 0x80, 0x90,0x34, 0x92,0x45,
  0,166, 0x80, 0x82, 0x90,0x39, 0x92,0x47, 0,166, 0x80, 0x82, 0x90,0x32, 0x92,0x49, 0,166, 0x80, 0x90,0x39,
  0,166, 0x80, 0x82, 0x90,0x31, 0x92,0x47, 0,166, 0x80, 0x82, 0x90,0x39, 0x92,0x45, 0,166, 0x80, 0x82, 0x90,0x2F,
  0x92,0x44, 0,166, 0x80, 0x90,0x38, 0,166, 0x80, 0x81, 0x90,0x31, 0x91,0x4A, 0,166, 0x80, 0x81, 0x90,0x38,
  0x91,0x4C, 0,166, 0x80, 0x81, 0x90,0x32, 0x91,0x4E, 0,166, 0x80, 0x90,0x38, 0,166, 0x80, 0x81, 0x90,0x2F,
  0x91,0x4C, 0,166, 0x80, 0x81, 0x90,0x38, 0x91,0x4A, 0,166, 0x80, 0x81, 0x90,0x4C, 0x91,0x31, 0,166, 0x81,
  0x91,0x38, 0,166, 0x81, 0x82, 0x91,0x44, 0x92,0x32, 0,166, 0x81, 0x82, 0x91,0x45, 0x92,0x38, 0,166, 0x81,
  0x82, 0x91,0x47, 0x92,0x31, 0,166, 0x82, 0x92,0x38, 0,166, 0x81, 0x82, 0x91,0x45, 0x92,0x2F, 0,166, 0x81,
  0x82, 0x91,0x44, 0x92,0x38, 0,166, 0x81, 0x82, 0x91,0x42, 0x92,0x2E, 0,166, 0x82, 0x92,0x36, 0,166, 0x80,
  0x82, 0x90,0x49, 0x92,0x2F, 0,166, 0x80, 0x82, 0x90,0x4A, 0x92,0x36, 0,166, 0x80, 0x82, 0x90,0x4C, 0x92,0x31,
  0,166, 0x82, 0x92,0x36, 0,166, 0x80, 0x82, 0x90,0x4A, 0x92,0x2E, 0,166, 0x80, 0x82, 0x90,0x49, 0x92,0x36,
  0,166, 0x80, 0x82, 0x90,0x4A, 0x92,0x2F, 0,166, 0x82, 0x92,0x36, 0,166, 0x81, 0x82, 0x91,0x42, 0x92,0x31,
  0,166, 0x81, 0x82, 0x91,0x44, 0x92,0x36, 0,166, 0x81, 0x82, 0x91,0x45, 0x92,0x2F, 0,166, 0x82, 0x92,0x36,
  0,166, 0x81, 0x82, 0x91,0x44, 0x92,0x2D, 0,166, 0x81, 0x82, 0x91,0x42, 0x92,0x36, 0,166, 0x81, 0x82, 0x91,0x2C,
  0x92,0x40, 0,166, 0x81, 0x91,0x34, 0,166, 0x80, 0x81, 0x90,0x47, 0x91,0x28, 0,166, 0x80, 0x81, 0x90,0x49,
  0x91,0x34, 0,166, 0x80, 0x81, 0x90,0x4A, 0x91,0x2A, 0,166, 0x81, 0x91,0x34, 0,166, 0x80, 0x81, 0x90,0x49,
  0x91,0x2C, 0,166, 0x80, 0x81, 0x90,0x47, 0x91,0x34, 0,166, 0x80, 0x81, 0x90,0x49, 0x91,0x2D, 0,166, 0x80,
  0x90,0x45, 0,166, 0x80, 0x81, 0x82, 0x90,0x4E, 0x91,0x2C, 0,166, 0x81, 0x91,0x45, 0x92,0x2D, 0,166, 0x81,
  0x82, 0x91,0x44, 0x92,0x2F, 0,166, 0x80, 0x90,0x4B, 0,166, 0x80, 0x82, 0x90,0x4C, 0x92,0x2D, 0,166, 0x81,
  0x82, 0x91,0x44, 0x92,0x2C, 0,166, 0x81, 0x82, 0x91,0x42, 0x92,0x2D, 0,166, 0x80, 0x90,0x49, 0,166, 0x80,
  0x90,0x4A, 0,166, 0x81, 0x91,0x42, 0,166, 0x81, 0x91,0x40, 0,166, 0x80, 0x82, 0x90,0x47, 0x92,0x2F, 0,166,
  0x80, 0x82, 0x90,0x49, 0x92,0x2C, 0,166, 0x81, 0x82, 0x91,0x2D, 0x92,0x40, 0,166, 0x80, 0x81, 0x82, 0x90,0x2F,
  0x91,0x44, 0x92,0x3E, 0,166, 0x80, 0x90,0x28, 0,166, 0x80, 0x81, 0x82, 0x90,0x31, 0x91,0x3D, 0x92,0x45, 0,166,
  0x80, 0x90,0x28, 0,166, 0x80, 0x81, 0x90,0x32, 0x91,0x3B, 0,166, 0x80, 0x90,0x28, 0,166, 0x80, 0x82, 0x90,0x34,
  0x92,0x44, 1,77, 0x80, 0x81, 0x82, 0x90,0x31, 0x91,0x45, 0x92,0x40, 0,166, 0x82, 0x92,0x3D, 0,166, 0x80,
  0x81, 0x82, 0x90,0x36, 0x91,0x44, 0x92,0x3E, 0,166, 0x81, 0x91,0x45, 0,166, 0x81, 0x91,0x47, 0,166, 0x82,
  0x92,0x3B, 0,166, 0x80, 0x81, 0x82, 0x90,0x35, 0x91,0x45, 0x92,0x3D, 0,166, 0x81, 0x91,0x44, 0,166, 0x80,
  0x81, 0x90,0x45, 0x91,0x36, 0,166, 0x82, 0x92,0x39, 0,166, 0x82, 0x92,0x3B, 0,166, 0x82, 0x92,0x38, 0,166,
  0x82, 0x92,0x39, 0,166, 0x80, 0x90,0x47, 0,166, 0x80, 0x81, 0x90,0x44, 0x91,0x34, 0,166, 0x80, 0x90,0x45,
  0,166, 0x80, 0x81, 0x90,0x47, 0x91,0x32, 0,166, 0x80, 0x82, 0x90,0x38, 0x92,0x40, 0,166, 0x80, 0x81, 0x82,
  0x90,0x49, 0x91,0x31, 0x92,0x39, 0,166, 0x80, 0x90,0x40, 0,166, 0x80, 0x81, 0x90,0x4A, 0x91,0x2F, 0,166,
  0x80, 0x90,0x40, 0,166, 0x80, 0x81, 0x82, 0x90,0x4C, 0x91,0x34, 0x92,0x38, 1,77, 0x80, 0x81, 0x82, 0x90,0x49,
  0x91,0x39, 0x92,0x2D, 0,166, 0x80, 0x90,0x47, 0,166, 0x80, 0x81, 0x90,0x45, 0x91,0x3D, 0,166, 0x80, 0x90,0x47,
  0,166, 0x80, 0x81, 0x82, 0x90,0x49, 0x91,0x42, 0,166, 0x80, 0x90,0x4B, 0,166, 0x80, 0x90,0x4C, 0x92,0x39,
  0,166, 0x80, 0x90,0x4E, 0,166, 0x80, 0x82, 0x90,0x47, 0x92,0x38, 0,166, 0x80, 0x90,0x45, 0,166, 0x80,
  0x81, 0x90,0x44, 0x91,0x3B, 0,166, 0x80, 0x90,0x45, 0,166, 0x80, 0x81, 0x90,0x47, 0x91,0x40, 0,166, 0x80,
  0x90,0x49, 0,166, 0x80, 0x82, 0x90,0x4A, 0x92,0x38, 0,166, 0x80, 0x90,0x4C, 0,166, 0x80, 0x82, 0x90,0x45,
  0x92,0x36, 0,166, 0x80, 0x90,0x44, 0,166, 0x80, 0x81, 0x90,0x42, 0x91,0x39, 0,166, 0x80, 0x90,0x44, 0,166,
  0x80, 0x81, 0x90,0x45, 0x91,0x3E, 0,166, 0x80, 0x90,0x47, 0,166, 0x80, 0x82, 0x90,0x49, 0x92,0x36, 0,166,
  0x80, 0x90,0x4A, 0,166, 0x80, 0x82, 0x90,0x44, 0x92,0x34, 0,166, 0x80, 0x90,0x47, 0,166, 0x80, 0x81, 0x90,0x44,
  0x91,0x3B, 0,166, 0x80, 0x81, 0x90,0x3D, 0x91,0x40, 0,166, 0x80, 0x81, 0x90,0x4C, 0x91,0x3E, 1,77, 0x81,
  0x91,0x3D, 0,166, 0x81, 0x91,0x3B, 0,166, 0x81, 0x91,0x3D, 0,166, 0x80, 0x90,0x4C, 0,166, 0x80, 0x81,
  0x90,0x49, 0x91,0x39, 0,166, 0x80, 0x81, 0x90,0x45, 0x91,0x3B, 0,166, 0x80, 0x81, 0x90,0x51, 0x91,0x3D, 1,77,
  0x81, 0x91,0x3B, 0,166, 0x81, 0x91,0x39, 0,166, 0x81, 0x91,0x3B, 0,166, 0x80, 0x90,0x47, 0,166, 0x80,
  0x81, 0x90,0x4A, 0x91,0x38, 0,166, 0x80, 0x81, 0x90,0x4E, 0x91,0x39, 0,166, 0x80, 0x81, 0x90,0x50, 0x91,0x3B,
  1,77, 0x81, 0x91,0x39, 0,166, 0x81, 0x91,0x38, 0,166, 0x81, 0x91,0x39, 0,166, 0x80, 0x90,0x45, 0,166,
  0x80, 0x81, 0x90,0x49, 0x91,0x36, 0,166, 0x80, 0x81, 0x90,0x4C, 0x91,0x38, 0,166, 0x80, 0x81, 0x90,0x4E, 0x91,0x39,
  1,77, 0x81, 0x91,0x38, 0,166, 0x81, 0x91,0x36, 0,166, 0x81, 0x91,0x38, 0,166, 0x80, 0x82, 0x90,0x4C,
  0x92,0x34, 0,166, 0x80, 0x81, 0x82, 0x90,0x4B, 0x91,0x38, 0,166, 0x80, 0x81, 0x90,0x4C, 0x91,0x3B, 0,166,
  0x80, 0x81, 0x90,0x4E, 0x91,0x45, 0x92,0x3C, 0,166, 0x82, 0x92,0x38, 0,166, 0x80, 0x81, 0x82, 0x90,0x39, 0,166,
  0x80, 0x90,0x33, 0,166, 0x80, 0x90,0x49, 0x91,0x45, 0x92,0x34, 0,13, 1,29, 0,246, 0x80, 0x81, 0x90,0x4A,
  0x91,0x47, 0,53, 0,141, 0x80, 0x81, 0x82, 0x90,0x47, 0x91,0x44, 0x92,0x28, 0,95, 1,77, 0,202,
0x80, 0x81, 0x90,0x45, 0,91, 0,136, 0x80, 0x82, 0x90,0x45, 0x91,0x2D, 7,83, 0x80, 0x81, 0xf0};



void setup() {
  //initiate arduboy instance
  arduboy.beginNoLogo();

  // here we set the framerate to 30, we do not need to run at default 60 and
  // it saves us battery life.
  arduboy.setFrameRate(30);
  score = 40;
  pressing = false;
  lost_game = false;
  frame_counter = 0;
  diff = 0;
}



void loop() {

  if (!(arduboy.nextFrame()))
    return;
    
  if (arduboy.notPressed(ALL_DIR_BUTTONS)){
    pressing = false;
  }else{
    if (!pressing ) {
      pressing = true;
      int local_diff = 0;
      // if the right button is pressed move 1 pixel to the right every frame
      if(arduboy.pressed(RIGHT_BUTTON)) {
        local_diff+=5;
      }
    
      // if the left button is pressed move 1 pixel to the left every frame
      if(arduboy.pressed(LEFT_BUTTON)) {
        local_diff-=5;
      }
    
      // if the up button or B button is pressed move 1 pixel up every frame
      if(arduboy.pressed(UP_BUTTON)) {
        local_diff++;
      }
    
      // if the down button or A button is pressed move 1 pixel down every frame
      if (arduboy.pressed(DOWN_BUTTON)) {
        local_diff--;
      }
      
      if (arduboy.pressed(B_BUTTON) && abs(local_diff) == 1){
        if (local_diff < 0){
          cur_subscore += 1;
        }else{
          cur_subscore -= 1;
        }
        cur_subscore = (cur_subscore + num_sub_scores) % num_sub_scores;
        arduboy.tunes.tone(330, 50);
        local_diff = 0;
      }else if (arduboy.pressed(B_BUTTON) && abs(local_diff) > 1){
        local_diff /= 5;
        subscores[cur_subscore] += local_diff;
      }else{
        if (frame_counter <= 0){
          diff = 0;
        }
        frame_counter = 90;
        diff += local_diff;
        
        score += local_diff;
      }
      if (local_diff != 0){
        int temp_tone = 220;
        if (local_diff > 0){
          temp_tone = 440;
        }
        temp_tone += diff * 10;
        if (temp_tone < 50) temp_tone = 50;
        
        arduboy.tunes.tone(temp_tone, 100 + abs(local_diff * 20));
      }
    }
  }

  // we clear our screen to black
  arduboy.clear();
  
  sprintf(text, "%i", score);
  arduboy.setCursor(0, -1);
  arduboy.setTextSize(7);
  arduboy.print(text);
  

  int box_w = 20;
  int box_h = 10;


  if (arduboy.pressed(A_BUTTON)){
    if (timer_pressed == false){
      TimerState++;
      timer_pressed = true;
  
      if (TimerState == 1){
        StartTime = millis();
      }else if (TimerState == 2){
        EndTime = millis();
      }
    }
  }else{
    timer_pressed = false;
  }
  
  
  if (TimerState == 0){
    // don't do anything
  }else if ((TimerState == 1) || (TimerState == 2)){
      long t;
      if (TimerState == 1){
        t = millis() - StartTime;
      }else if (TimerState == 2){
        t = EndTime  - StartTime;
      }
      int seconds = t/1000;
      int tens = (t/100)%10;
      sprintf(text, "%d.%d", seconds, tens);
      arduboy.setCursor(WIDTH - 60, 50);
      arduboy.setTextSize(2);
      arduboy.print(text);
  }else{
    TimerState = 0;
  }
  if (arduboy.pressed(A_BUTTON)){
    //arduboy.drawRect(WIDTH-box_w, cur_subscore * box_h, box_w, box_h, WHITE);
  }
  if (arduboy.pressed(B_BUTTON)){
    // draw an arrow at what we are selecting
    int arrow_width = 18;
    arduboy.fillRect(WIDTH - box_w - arrow_width, cur_subscore * box_h , arrow_width, box_h, BLACK);
    arduboy.drawLine(WIDTH - box_w - arrow_width, cur_subscore * box_h + box_h / 2, WIDTH - box_w - 2, cur_subscore * box_h + box_h / 2, WHITE);
    arduboy.fillTriangle (
      WIDTH - box_w - 1,               cur_subscore * box_h + box_h / 2,
      WIDTH - box_w - arrow_width * 2 / 3, cur_subscore * box_h,
      WIDTH - box_w - arrow_width * 2 / 3, cur_subscore * box_h + box_h / 2,
      WHITE);
    arduboy.fillTriangle (
      WIDTH - box_w -2,                cur_subscore * box_h + box_h / 2 ,
      WIDTH - box_w - arrow_width * 2 / 3, cur_subscore * box_h + box_h,
      WIDTH - box_w - arrow_width * 2 / 3, cur_subscore * box_h + box_h / 2,
      WHITE);
  }
  for (int i = 0; i < num_sub_scores; i++){
    if (subscores[i] != 0){
      sprintf(text, "%i", subscores[i]);
      arduboy.setCursor(WIDTH-box_w + 1, box_h * i + 1);
      arduboy.setTextSize(1);
      arduboy.print(text);
    }
  }
  
  if (frame_counter > 0){
    frame_counter--;

    sprintf(text, "%+i", diff);
    arduboy.setCursor(0, 50);
    arduboy.setTextSize(2);
    arduboy.print(text);
  }else{
    if (!lost_game && score <= 0){
      lost_game = true;
      arduboy.tunes.playScore(lost_game_sound); 
    }
  }

  // then we finaly we tell the arduboy to display what we just wrote to the display.
  arduboy.display();

  
}
