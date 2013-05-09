#include <math.h>
#include <DxLib.h>
#include "BattleScene.h"
#include "HomeScene.h"
#include "Keyboard.h"

BattleScene::BattleScene(){
	has_come_turn = false;
	act_only_one = false;
}

void BattleScene::initialize(){
	turn = 1;
	stage.initialize();

	ObjectManager::create(players, "data/chara/player1.csv", 3, 7);
	ObjectManager::create(players, "data/chara/player2.csv", 6, 7);

	ObjectManager::create(enemies, "data/stage/stage2/enemy.csv");

	simulate();
}

void BattleScene::update(){
	stage.update();
	cursor.update();
	camera.update();

	for(auto& player : players){
		player.update();
	}
	for(auto& enemy : enemies){
		enemy.update();
	}

	//calculate ATBgauge
	while(!has_come_turn){
		for(auto& player : players){
			//sub ATBgauge
			player.stepATBgauge();

			//check
			if(player.isMyTurn()){
				has_come_turn = true;
				cursor.pos = player.pos;
				break;
			}
		}
		//敵との重複を避ける
		if(has_come_turn) break;

		for(auto& enemy : enemies){
			//sub ATBgauge
			enemy.stepATBgauge();

			//check
			if(enemy.isMyTurn()){
				has_come_turn = true;
				cursor.pos = enemy.pos;
				break;
			}
		}
		break;
	}

	//action
	while(has_come_turn){
		for(auto& player : players){
			if(!player.isMyTurn()) continue;
			act_only_one = true;

			switch(player.state){//コマンド選択時以外動かす
			case player.WAIT:
			case player.MOVE:
			case player.ATTACK:
				cursor.manipulate();
				break;
			}

			player.attack(enemies);
			player.action();
			if(player.state == player.END){
				if(player.assignDirection()){
					player.endMyTurn();
					has_come_turn = false;
					act_only_one = false;
					simulate();
				}
			}
			break;
		}
		//敵との重複を避ける
		if(act_only_one) break;

		for(auto& enemy : enemies){
			if(!enemy.isMyTurn()) continue;

			if(enemy.state == enemy.SELECT){
				enemy.calcMove(players);
				enemy.calcAttack(players);
			}
			enemy.action();
			enemy.attack(players);

			if(enemy.state == enemy.END){
				enemy.endMyTurn();
				enemy.assignDirection(players);
				has_come_turn = false;
				simulate();
			}
			break;
		}
		break;
	}

	//delete
	auto player = players.begin();
	while(player != players.end()){
		if(player->getHP() <= 0){
			player = players.erase(player);
		} else {
			++player;
		}
	}
	auto enemy = enemies.begin();
	while(enemy != enemies.end()){
		if(enemy->getHP() <= 0){
			enemy = enemies.erase(enemy);
		} else {
			++enemy;
		}
	}

	//change scene
	if(players.empty() || enemies.empty()){
		changeScene(new HomeScene);
	}
}

void BattleScene::draw(){
	if(!has_come_turn) DrawString(0, 16, "waiting...", GetColor(255,255,255));

	stage.draw();
	cursor.draw();

	for(auto& player : players){
		player.draw();
	}
	for(auto& enemy : enemies){
		enemy.draw();
	}

	for(unsigned int i = 0; i < order.size(); ++i){
		DrawFormatString(0, 64+16*i, GetColor(255,255,255), "%2d: %s", i, order[i].c_str());
	}

	lateUpdate();
}

void BattleScene::lateUpdate(){
	stage.lateUpdate();
	for(auto& player : players){
		player.lateUpdate();
	}
	for(auto& enemy : enemies){
		enemy.lateUpdate();
	}
}

void BattleScene::simulate(){
	vector<Player> splayers = players;
	vector<Enemy> senemies = enemies;
	order.clear();

	while(order.size() < 20){
		for(auto& player : splayers){
			//sub ATBgauge
			player.stepATBgauge();

			//check
			if(player.isMyTurn()){
				order.push_back(player.getName());
				if(order.size() >= 20) return;
				player.resetATBgauge();
			}
		}

		for(auto& enemy : senemies){
			//sub ATBgauge
			enemy.stepATBgauge();

			//check
			if(enemy.isMyTurn()){
				order.push_back(enemy.getName());
				if(order.size() >= 20) return;
				enemy.resetATBgauge();
			}
		}
	}
}
