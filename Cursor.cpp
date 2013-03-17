#include <DxLib.h>
#include "Cursor.h"
#include "GV.h"
#include "Keyboard.h"
#include "Stage.h"

Position Cursor::pos;

Cursor::Cursor(){
	image = GetColor(0, 255, 0);
}

void Cursor::update(){
	myvec = VGet(pos.y*chipsize, Stage::getHeight(pos)*chipheight, pos.x*chipsize);

	//ステージ外にはみ出ないようにする
	if(pos.x <= 0) pos.x = 0;
	if(pos.y <= 0) pos.y = 0;
	if(pos.x >= Stage::getWidth()-1) pos.x = Stage::getWidth()-1;
	if(pos.y >= Stage::getDepth()-1) pos.y = Stage::getDepth()-1;
}

void Cursor::manipulate(){
	if(Keyboard::pushed(KEY_INPUT_LEFT) || (Keyboard::pushingUntil(KEY_INPUT_LEFT, 30) && Keyboard::pushingPer(KEY_INPUT_LEFT, 6))){
		pos.Move(-1,  0);
	}
	if(Keyboard::pushed(KEY_INPUT_RIGHT) || (Keyboard::pushingUntil(KEY_INPUT_RIGHT, 30) && Keyboard::pushingPer(KEY_INPUT_RIGHT, 6))){
		pos.Move( 1,  0);
	}
	if(Keyboard::pushed(KEY_INPUT_UP) || (Keyboard::pushingUntil(KEY_INPUT_UP, 30) && Keyboard::pushingPer(KEY_INPUT_UP, 6))){
		pos.Move( 0, -1);
	}
	if(Keyboard::pushed(KEY_INPUT_DOWN) || (Keyboard::pushingUntil(KEY_INPUT_DOWN, 30) && Keyboard::pushingPer(KEY_INPUT_DOWN, 6))){
		pos.Move( 0,  1);
	}
}

void Cursor::draw(){
	//球の描画（カーソル）
	DrawSphere3D(VAdd(myvec, VGet(chipsize/2, chipsize/2, chipsize/2)), chipsize/2, 50, image, image, true);
}
