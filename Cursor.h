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
};
