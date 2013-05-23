#include "DxLib.h"
#include "ResultScene.h"
#include "StartScene.h"
#include "GV.h"
#include "FileStream.h"
#include "Keyboard.h"

ResultScene::ResultScene(){
}

ResultScene::~ResultScene(){
	if(my_mission[0] < imy_mission && imy_mission < all_stage) my_mission[0] = imy_mission;
	FileStream::write("data/data.dat", my_mission);

	StopSoundMem(Sound::result_scene);
}

void ResultScene::initialize(){
	FileStream::load("data/data.dat", my_mission);
	imy_mission = my_mission[1];
	if(imy_mission < all_stage) ++imy_mission;

	PlaySoundMem(Sound::result_scene, DX_PLAYTYPE_BACK);
}

void ResultScene::update(){
	if(CheckSoundMem(Sound::result_scene)) return;

	if(Keyboard::pushed(KEY_INPUT_Z)){
		changeScene(new StartScene);
	}
}

void ResultScene::draw(){
	if(imy_mission < all_stage){
		DrawString(100, 200, "ステージクリアー！", GetColor(255,255,255));
		//未クリア
		if(my_mission[0] < imy_mission){
			DrawString(100, 216, "新しいミッションが解禁されました！", GetColor(255,255,255));
		}
	} else {
		DrawString(100, 200, "オールクリア！", GetColor(255,255,255));
	}
}