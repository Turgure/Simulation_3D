#pragma once
#include <DxLib.h>
#include "Position.h"

class Cursor{
public:
	Cursor();
	void update();
	void draw();
	static void set(int x, int y);

	static Position pos(){ return mypos; }

private:
	int image;

	VECTOR myvec;
	static Position mypos;
};