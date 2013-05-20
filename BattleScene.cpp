#include <DxLib.h>
#include "BattleScene.h"
#include "ResultScene.h"
#include "GV.h"
#include "Keyboard.h"

BattleScene::BattleScene(){
	has_come_turn = false;
	act_only_one = false;
}

void BattleScene::initialize(){
	turn = 1;
	stage.initialize();

	ObjectManager::create(players, "data/stage/stage2/players.csv");
	ObjectManager::create(enemies, "data/stage/stage2/enemies.csv");

	simulate();
}

void BattleScene::update(){
	stage.update();
	camera.update();

	if(camera.isTurning()) return;
	cursor.update();

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
		changeScene(new ResultScene);
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

	//行動順を表示
	if(Keyboard::pushing(KEY_INPUT_LSHIFT)){
		int i = 0;
		for(auto& or : order){
			DrawFormatString(DEFAULT_SCREEN_SIZE_X-128, 16*i, GetColor(255,255,255), "%2d: %s", i, or.c_str());
			++i;
		}
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
