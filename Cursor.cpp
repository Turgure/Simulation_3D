#include "Cursor.h"
#include "GV.h"
#include "Keyboard.h"
#include "Stage.h"

Position Cursor::pos;

Cursor::Cursor(){
	image = MV1LoadModel("data/image/3Dmodel/cursor/cursor.x");
}

void Cursor::update(){
	myvec = VGet(pos.y*chipsize, Stage::getHeight(pos)*chipheight, pos.x*chipsize);
	MV1SetPosition(image, VAdd(myvec, VGet(chipsize/2, chipsize/2+30, chipsize/2)));
}

void Cursor::manipulate(){
	if(Keyboard::pushed(KEY_INPUT_LEFT) ||
		(Keyboard::pushingUntil(KEY_INPUT_LEFT, 30) && Keyboard::pushingPer(KEY_INPUT_LEFT, 6))){
			if(pos.x > 0){
				pos.Move(-1,  0);
			}
	}
	if(Keyboard::pushed(KEY_INPUT_RIGHT) ||
		(Keyboard::pushingUntil(KEY_INPUT_RIGHT, 30) && Keyboard::pushingPer(KEY_INPUT_RIGHT, 6))){
			if(pos.x < Stage::getWidth()-1){
				pos.Move(1,  0);
			}
	}
	if(Keyboard::pushed(KEY_INPUT_UP) ||
		(Keyboard::pushingUntil(KEY_INPUT_UP, 30) && Keyboard::pushingPer(KEY_INPUT_UP, 6))){
			if(pos.y > 0){
				pos.Move(0, -1);
			}
	}
	if(Keyboard::pushed(KEY_INPUT_DOWN) ||
		(Keyboard::pushingUntil(KEY_INPUT_DOWN, 30) && Keyboard::pushingPer(KEY_INPUT_DOWN, 6))){
			if(pos.y < Stage::getDepth()-1){
				pos.Move(0,  1);
			}
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
