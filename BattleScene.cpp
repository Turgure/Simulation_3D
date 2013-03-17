#include <DxLib.h>
#include "BattleScene.h"

BattleScene::BattleScene(){
}

void BattleScene::initialize(){
	stage.initialize();

	ObjectManager::create(players, "data/chara/player1.csv", 1, 9);
	ObjectManager::create(players, "data/chara/player2.csv", 2, 5);
	
	ObjectManager::create(enemies, "data/stage/stage2/enemy.csv");
}

void BattleScene::update(){
	stage.update();
	cursor.update();
	for(auto& player : players){
		player.update();
	}
	for(auto& enemy : enemies){
		enemy.update();
	}
}

void BattleScene::draw(){
	stage.draw();
	cursor.draw();
	for(auto& player : players){
		player.draw();
	}
	for(auto& enemy : enemies){
		enemy.draw();
	}

	stage.lateUpdate();
}