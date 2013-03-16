#include <DxLib.h>
#include "BattleScene.h"

BattleScene::BattleScene():cursor(0,9){
}

void BattleScene::initialize(){
}

void BattleScene::update(){
	stage.update();
	cursor.update();
}

void BattleScene::draw(){
	stage.draw();
	cursor.draw();
}