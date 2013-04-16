#include <DxLib.h>
#include "StartScene.h"
#include "BattleScene.h"
#include "Keyboard.h"

StartScene::StartScene(){
}

void StartScene::initialize(){
	selected_menu = 1;
}

void StartScene::update(){

	//change scene
	if(selected_menu == 1 && Keyboard::pushed(KEY_INPUT_Z)){
		changeScene(new BattleScene);
	}
	if(selected_menu == 1 && Keyboard::pushed(KEY_INPUT_DOWN)){
		selected_menu = 2;
	}
	if(selected_menu == 2 && Keyboard::pushed(KEY_INPUT_UP)){
		selected_menu = 1;
	}

}

void StartScene::draw(){
	if(selected_menu == 1){
		DrawString(0, 16, "Play Game", GetColor(0,255,0));
		DrawString(0, 32, "Exit", GetColor(255,255,255));
	}else{
		DrawString(0, 16, "Play Game", GetColor(255,255,255));
		DrawString(0, 32, "Exit", GetColor(0,255,0));
	}
}