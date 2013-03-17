#pragma once
#include <DxLib.h>
#include "Position.h"

class Cursor{
public:
	Cursor();
	void update();
	void draw();

	static Position& pos(){ return mypos; }

private:
	int image;

	VECTOR myvec;
	static Position mypos;
};