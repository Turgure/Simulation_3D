#include "DxLib.h"
#include "GameoverScene.h"
#include "StartScene.h"
#include "Keyboard.h"

GameoverScene::GameoverScene(){
}

void GameoverScene::initialize(){
}

void GameoverScene::update(){
	if(Keyboard::pushed(KEY_INPUT_Z)){
		changeScene(new StartScene);
	}
}

void GameoverScene::draw(){
	DrawString(300, 200, "gameover...", GetColor(255,255,255));
}