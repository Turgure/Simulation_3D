#pragma once
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

	static Position mypos;
};