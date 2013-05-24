#include "Cursor.h"
#include "GV.h"
#include "Keyboard.h"
#include "Stage.h"
#include "Camera.h"

Position Cursor::pos;

Cursor::Cursor(){
	image = MV1LoadModel("data/image/3Dmodel/cursor/cursor.x");
	arrow = MV1LoadModel("data/image/3dmodel/cursor/arrow.mqo");
}

void Cursor::update(){
	myvec = VGet(pos.y*chipsize, Stage::getHeight(pos)*chipheight, pos.x*chipsize);
	MV1SetPosition(image, VAdd(myvec, VGet(chipsize/2, chipsize/2 + 60, chipsize/2)));
	MV1SetPosition(arrow, VAdd(myvec, VGet(chipsize/2, chipsize/2 + 80, chipsize/2)));
}

void Cursor::manipulate(){
	if(Keyboard::pushed(KEY_INPUT_LEFT, false) ||
		(Keyboard::pushingUntil(KEY_INPUT_LEFT, 30) && Keyboard::pushingPer(KEY_INPUT_LEFT, 6))){
			PlaySoundMem(Sound::cursor, DX_PLAYTYPE_BACK);
			move(Camera::getDirection(LEFT));
	}
	if(Keyboard::pushed(KEY_INPUT_RIGHT, false) ||
		(Keyboard::pushingUntil(KEY_INPUT_RIGHT, 30) && Keyboard::pushingPer(KEY_INPUT_RIGHT, 6))){
			PlaySoundMem(Sound::cursor, DX_PLAYTYPE_BACK);
			move(Camera::getDirection(RIGHT));
	}
	if(Keyboard::pushed(KEY_INPUT_UP, false) ||
		(Keyboard::pushingUntil(KEY_INPUT_UP, 30) && Keyboard::pushingPer(KEY_INPUT_UP, 6))){
			PlaySoundMem(Sound::cursor, DX_PLAYTYPE_BACK);
			move(Camera::getDirection(FRONT));
	}
	if(Keyboard::pushed(KEY_INPUT_DOWN, false) ||
		(Keyboard::pushingUntil(KEY_INPUT_DOWN, 30) && Keyboard::pushingPer(KEY_INPUT_DOWN, 6))){
			PlaySoundMem(Sound::cursor, DX_PLAYTYPE_BACK);
			move(Camera::getDirection(BACK));
	}
}

void Cursor::drawPoint(){
	//draw cone
	MV1DrawModel(image);
	//draw square
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	Stage::drawChip(pos.x, pos.y, GetColor(255,0,0));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Cursor::drawArrow(int dir){
	switch(dir){
	case NORTH:
		MV1SetRotationXYZ(arrow, VGet(0.0f, DX_PI_F/2, 0.0f));
		break;
	case SOUTH:
		MV1SetRotationXYZ(arrow, VGet(0.0f, -DX_PI_F/2, 0.0f));
		break;
	case WEST:
		MV1SetRotationXYZ(arrow, VGet(0.0f, 0.0f, 0.0f));
		break;
	case EAST:
		MV1SetRotationXYZ(arrow, VGet(0.0f, DX_PI_F, 0.0f));
		break;
	}
	MV1DrawModel(arrow);
}

void Cursor::move(int dir){
	switch(dir){
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
