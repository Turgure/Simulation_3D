#pragma once
#include "DxLib.h"
#include "Position.h"

class Cursor{
public:
	Cursor();
	void update();
	void drawPoint();
	void drawArrow(int dir);
	void manipulate();

	static Position pos;
private:
	void move(int dir);

	int image;
	int arrow;
	VECTOR myvec;
};
