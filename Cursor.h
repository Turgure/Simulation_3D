#pragma once
#include <DxLib.h>
#include "Position.h"

class Cursor{
public:
	Cursor();
	void update();
	void draw();
	void manipulate();

	static Position pos;

private:
	void move(int direction);

	int image;

	VECTOR myvec;
	enum Direction{NORTH, SOUTH, WEST, EAST, DIR_NUM};
	/*
	dir[0] => KEY_INPUT_LEFT
	dir[1] => KEY_INPUT_RIGHT
	dir[2] => KEY_INPUT_UP
	dir[3] => KEY_INPUT_DOWN
	*/
	int dir[4];
};
