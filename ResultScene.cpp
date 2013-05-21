#include "DxLib.h"
#include "ResultScene.h"
#include "StartScene.h"
#include "Keyboard.h"

ResultScene::ResultScene(){
}

void ResultScene::initialize(){
}

void ResultScene::update(){
	if(Keyboard::pushed(KEY_INPUT_Z)){
		changeScene(new StartScene);
	}
}

void ResultScene::draw(){
	DrawString(100, 200, "新しいミッションが解禁されました！", GetColor(255,255,255));
}