#include "DxLib.h"
#include "ResultScene.h"
#include "StartScene.h"
#include "FileStream.h"
#include "Keyboard.h"

ResultScene::ResultScene(){
}

ResultScene::~ResultScene(){
	if(my_mission[0] < imy_mission) my_mission[0] = imy_mission;
	FileStream::write("data/data.dat", my_mission);
}

void ResultScene::initialize(){
	FileStream::load("data/data.dat", my_mission);
	imy_mission = my_mission[1];
	if(imy_mission < 5) ++imy_mission;
}

void ResultScene::update(){
	if(Keyboard::pushed(KEY_INPUT_Z)){
		changeScene(new StartScene);
	}
}

void ResultScene::draw(){
	if(imy_mission < 5){
		DrawString(100, 200, "ステージクリアー！", GetColor(255,255,255));
		//未クリア
		if(my_mission[0] < imy_mission){
			DrawString(100, 216, "新しいミッションが解禁されました！", GetColor(255,255,255));
		}
	} else {
		DrawString(100, 200, "オールクリア！", GetColor(255,255,255));
	}
}