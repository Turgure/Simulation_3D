#include <DxLib.h>
#include "Cursor.h"
#include "GV.h"
#include "Keyboard.h"
#include "Stage.h"

Position Cursor::mypos;

Cursor::Cursor(){
	image = GetColor(0, 255, 0);
}

void Cursor::update(){
	myvec = VGet(mypos.y*chipsize, Stage::getHeight(mypos)*chipheight, mypos.x*chipsize);

	if(Keyboard::pushed(KEY_INPUT_LEFT) || (Keyboard::pushingUntil(KEY_INPUT_LEFT, 30) && Keyboard::pushingPer(KEY_INPUT_LEFT, 6))){
		mypos.Move(-1,  0);
	}
	if(Keyboard::pushed(KEY_INPUT_RIGHT) || (Keyboard::pushingUntil(KEY_INPUT_RIGHT, 30) && Keyboard::pushingPer(KEY_INPUT_RIGHT, 6))){
		mypos.Move( 1,  0);
	}
	if(Keyboard::pushed(KEY_INPUT_UP) || (Keyboard::pushingUntil(KEY_INPUT_UP, 30) && Keyboard::pushingPer(KEY_INPUT_UP, 6))){
		mypos.Move( 0, -1);
	}
	if(Keyboard::pushed(KEY_INPUT_DOWN) || (Keyboard::pushingUntil(KEY_INPUT_DOWN, 30) && Keyboard::pushingPer(KEY_INPUT_DOWN, 6))){
		mypos.Move( 0,  1);
	}
	//ステージ外にはみ出ないようにする
	if(mypos.x < 0) mypos.setX(0);
	if(mypos.y < 0) mypos.setY(0);
	if(mypos.x > Stage::getWidth()-1) mypos.setX(Stage::getWidth()-1);
	if(mypos.y > Stage::getDepth()-1) mypos.setY(Stage::getDepth()-1);

}

void Cursor::draw(){
	//球の描画（カーソル）
	DrawSphere3D(VAdd(myvec, VGet(chipsize/2, chipsize/2, chipsize/2)), chipsize/2, 50, image, image, true);
}
