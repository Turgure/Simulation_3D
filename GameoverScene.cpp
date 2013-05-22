#include "DxLib.h"
#include "GameoverScene.h"
#include "StartScene.h"
#include "Keyboard.h"

GameoverScene::GameoverScene(){
	bg = LoadGraph("data/image/bg/gameover.png");
	cnt = 0;
}

void GameoverScene::initialize(){
}

void GameoverScene::update(){
	if(adding) ++cnt;
	else --cnt;

	if(cnt > 240){
		cnt = 240;
		if(Keyboard::pushed(KEY_INPUT_Z)){
			adding = false;
		}
	}

	if(cnt < 0){
		changeScene(new StartScene);
	}
}

void GameoverScene::draw(){
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt+15);
	DrawGraph(0, 0, bg, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}