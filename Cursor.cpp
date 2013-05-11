#include "Cursor.h"
#include "GV.h"
#include "Keyboard.h"
#include "Stage.h"
#include "Camera.h"

Position Cursor::pos;

Cursor::Cursor(){
	image = MV1LoadModel("data/image/3Dmodel/cursor/cursor.x");
	setDirection();
}

void Cursor::update(){
	myvec = VGet(pos.y*chipsize, Stage::getHeight(pos)*chipheight, pos.x*chipsize);
	MV1SetPosition(image, VAdd(myvec, VGet(chipsize/2, chipsize/2+30, chipsize/2)));
}

void Cursor::manipulate(){
	if(Keyboard::pushed(KEY_INPUT_LEFT, false) ||
		(Keyboard::pushingUntil(KEY_INPUT_LEFT, 30) && Keyboard::pushingPer(KEY_INPUT_LEFT, 6))){
			move(dir[0]);
	}
	if(Keyboard::pushed(KEY_INPUT_RIGHT, false) ||
		(Keyboard::pushingUntil(KEY_INPUT_RIGHT, 30) && Keyboard::pushingPer(KEY_INPUT_RIGHT, 6))){
			move(dir[1]);
	}
	if(Keyboard::pushed(KEY_INPUT_UP, false) ||
		(Keyboard::pushingUntil(KEY_INPUT_UP, 30) && Keyboard::pushingPer(KEY_INPUT_UP, 6))){
			move(dir[2]);
	}
	if(Keyboard::pushed(KEY_INPUT_DOWN, false) ||
		(Keyboard::pushingUntil(KEY_INPUT_DOWN, 30) && Keyboard::pushingPer(KEY_INPUT_DOWN, 6))){
			move(dir[3]);
	}
}

void Cursor::draw(){
	//draw cone
	MV1DrawModel(image);

	//draw square
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	Stage::drawChip(pos.x, pos.y, GetColor(255,0,0));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Cursor::setDirection(){
	switch(Camera::getDir()){
	case Camera::MAX_MAX:
		dir[0] = WEST;
		dir[1] = EAST;
		dir[2] = NORTH;
		dir[3] = SOUTH;
		break;
	case Camera::ZERO_MAX:
		dir[0] = SOUTH;
		dir[1] = NORTH;
		dir[2] = WEST;
		dir[3] = EAST;
		break;
	case Camera::ZERO_ZERO:
		dir[0] = EAST;
		dir[1] = WEST;
		dir[2] = SOUTH;
		dir[3] = NORTH;
		break;
	case Camera::MAX_ZERO:
		dir[0] = NORTH;
		dir[1] = SOUTH;
		dir[2] = EAST;
		dir[3] = WEST;
		break;
	}
}

void Cursor::move(int direction){
	switch(direction){
	case NORTH:
		if(pos.y > 0) pos.Move(0, -1);
		break;
	case SOUTH:
		if(pos.y < Stage::getDepth()-1) pos.Move(0,  1);
		break;
	case WEST:
		if(pos.x > 0) pos.Move(-1,  0);
		break;
	case EAST:
		if(pos.x < Stage::getWidth()-1) pos.Move(1,  0);
		break;
	}
}
