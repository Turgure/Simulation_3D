#include "DxLib.h"
#include "GameoverScene.h"
#include "StartScene.h"
#include "Sound.h"
#include "Keyboard.h"

GameoverScene::GameoverScene(){
	bg = LoadGraph("data/image/bg/gameover.png");
	cnt = 0;

	SetVolumeSoundMem(9500, Sound::gameover_scene);
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
		SetVolumeSoundMem(7000 + cnt*20, Sound::gameover_scene); 
	}

	if(cnt > 120){
		cnt = 120;
		if(Keyboard::pushed(KEY_INPUT_Z)){
			adding = false;
		}
	}

	if(cnt < -60){
		changeScene(new StartScene);
	}
}

void GameoverScene::draw(){
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt*2);
	DrawGraph(0, 0, bg, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
