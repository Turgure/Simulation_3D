#pragma once
#include <math.h>
const int chipsize = 32;
const int chipheight = 16;
const double root_2 = sqrt(2.0);

enum Direction{FRONT, BACK, LEFT, RIGHT, DIR_NUM};	//自分の視点から見た向き
enum PointOfCompass{NORTH, SOUTH, WEST, EAST};		//固定方向(はじめの上が北)
