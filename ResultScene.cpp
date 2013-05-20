#include "DxLib.h"
#include "ResultScene.h"

ResultScene::ResultScene(){
}

void ResultScene::initialize(){
}

void ResultScene::update(){
}

void ResultScene::draw(){
	DrawString(300, 200, "game over.", GetColor(255,255,255));
}