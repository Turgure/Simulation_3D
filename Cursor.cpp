#include <DxLib.h>
#include "Cursor.h"
#include "GV.h"
#include "Keyboard.h"
//#include "Event.h"
#include "Stage.h"

Position Cursor::mypos(0,0);


Cursor::Cursor(int x, int y){
	mypos.set(x, y);
	image = GetColor(0, 255, 0);
}

void Cursor::update(){
	if(Keyboard::get(KEY_INPUT_LEFT) == 1 || Keyboard::get(KEY_INPUT_LEFT) > 30){
		mypos.Move(-1,  0);
	}
	if(Keyboard::get(KEY_INPUT_RIGHT) == 1 || Keyboard::get(KEY_INPUT_RIGHT) > 30){
		mypos.Move( 1,  0);
	}
	if(Keyboard::get(KEY_INPUT_UP) == 1 || Keyboard::get(KEY_INPUT_UP) > 30){
		mypos.Move( 0, -1);
	}
	if(Keyboard::get(KEY_INPUT_DOWN) == 1 || Keyboard::get(KEY_INPUT_DOWN) > 30){
		mypos.Move( 0,  1);
	}
	//ステージ外にはみ出ないようにする
	if(mypos.getX() < 0) mypos.setX(0);
	if(mypos.getY() < 0) mypos.setY(0);
	if(mypos.getX() > Stage::getWidth()-1) mypos.setX(Stage::getWidth()-1);
	if(mypos.getY() > Stage::getDepth()-1) mypos.setY(Stage::getDepth()-1);

}

void Cursor::draw(){
	//球の描画（カーソル）
	DrawSphere3D(VAdd( VGet(chipsize/2, chipsize/2, chipsize/2) , VGet(mypos.getX()*chipsize, Stage::getHeight(mypos.getX(), mypos.getY())*chipheight, mypos.getY()*chipsize)), chipsize/2 , 50, GetColor(0,255,0), GetColor(0,255,0), true);
}
