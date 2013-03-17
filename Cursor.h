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
	int image;

	VECTOR myvec;
};