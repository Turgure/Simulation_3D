#include "DxLib.h"
#include "GameoverScene.h"
#include "StartScene.h"
#include "Keyboard.h"

GameoverScene::GameoverScene(){
	bg = LoadGraph("data/image/bg/gameover.png");
}

void GameoverScene::initialize(){
}

void GameoverScene::update(){
	if(Keyboard::pushed(KEY_INPUT_Z)){
		changeScene(new StartScene);
	}
}

void GameoverScene::draw(){
	DrawGraph(0, 0, bg, true);
}