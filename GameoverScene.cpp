#include "DxLib.h"
#include "GameoverScene.h"
#include "StartScene.h"
#include "Sound.h"
#include "Keyboard.h"

GameoverScene::GameoverScene(){
	bg = LoadGraph("data/image/bg/gameover.png");
	cnt = 0;

	PlaySoundMem(Sound::gameover_scene, DX_PLAYTYPE_LOOP);
}

GameoverScene::~GameoverScene(){
	StopSoundMem(Sound::gameover_scene);
}

void GameoverScene::initialize(){
}

void GameoverScene::update(){
	if(adding) ++cnt;
	else {
		--cnt;
		SetVolumeSoundMem(6000 + cnt*16, Sound::gameover_scene); 
	}

	if(cnt > 240){
		cnt = 240;
		if(Keyboard::pushed(KEY_INPUT_Z)){
			adding = false;
		}
	}

	if(cnt < -60){
		changeScene(new StartScene);
	}
}

void GameoverScene::draw(){
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt+15);
	DrawGraph(0, 0, bg, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
