﻿#include <DxLib.h>
#include "Object.h"
#include "Keyboard.h"
#include "Cursor.h"
#include "ChipBrightnessManager.h"
#include "Stage.h"

Player::Player(int x, int y, int id, int hp, int mp, int str, int def, int agi, int mobility):pos(x, y){
	// ３Ｄモデルの読み込み
	model = MV1LoadModel("data/image/3Dmodel/boko.pmd");
	// ３Ｄモデルのスケールをx軸方向に3倍にする
	MV1SetScale(model, VGet(3.0f, 3.0f, 3.0f));

	this->id = id;
	this->hp = maxhp = hp;
	this->mp = maxmp = mp;
	this->str = str;
	this->def = def;
	this->agi = agi;
	this->mobility = mobility;
	state = SELECT;
	ATBgauge = 100;
	can_move = true;
	can_act = true;
}

void Player::update(){
	myvec = VGet(pos.y*chipsize, Stage::getHeight(pos)*chipheight, pos.x*chipsize);
	//3Dモデルの配置
	MV1SetPosition(model, VAdd(myvec, VGet(chipsize/2, 0, chipsize/2)));
	Stage::setObjectAt(pos, this);
}

void Player::draw(){
	// ３Ｄモデルの描画
	MV1DrawModel(model);

	for(int i = 0; i < mv_manager.path.size(); ++i)
		DrawFormatString(i*16, 100, GetColor(255,255,255), "%d", mv_manager.path[i]);

	if(pos == Cursor::pos){
		showStatus(200, 0);
	}

	showCommand();

	switch(state){
	case MOVE:
		ChipBrightnessManager::range(pos, mobility, false);
		break;
	case ACTION:
		ChipBrightnessManager::reachTo(pos, ChipBrightnessManager::getColorAttack(), 1, 3);
		break;
	default:
		break;
	}
}

void Player::action(){
	DrawFormatString(0, 0, GetColor(255,255,255), "player %d's turn.", id);

	if(!can_move && !can_act) state = END;

	switch(state){
	case SELECT:
		Stage::disbrighten();
		if(pos == Cursor::pos){
			if(Keyboard::pushed(KEY_INPUT_1) && can_move) state = MOVE;
			if(Keyboard::pushed(KEY_INPUT_2) && can_act) state = ACTION;
			if(Keyboard::pushed(KEY_INPUT_3)) state = END;
		}

		break;

	case MOVE:
		if(Keyboard::pushed(KEY_INPUT_3)) state = SELECT;
		if(Keyboard::pushed(KEY_INPUT_1)){
			state = SELECT;
			if(Stage::isBrightened(Cursor::pos) && !Stage::getObjectAt(Cursor::pos)){

				mv_manager.path = mv_manager.trackMovement(pos, Cursor::pos, mobility);

				pos = Cursor::pos;
				can_move = false;
			}
		}
		break;

	case ACTION:
		//attack(enemies);
		break;

	case END:
		can_move = false;
		can_act = false;
		Stage::disbrighten();
		break;
	}
}

void Player::endMyTurn(){
	state = SELECT;
	ATBgauge += 20;
	if(!can_move) ATBgauge += 40;
	if(!can_act) ATBgauge += 60;
	can_move = true;
	can_act = true;
}

void Player::stepATBgauge(){
	ATBgauge -= agi;
}

void Player::showCommand(){
	switch(state){
	case SELECT:
		if(pos == Cursor::pos){
			if(can_move){
				DrawString(400, 0, "MOVE   : key 1", GetColor(255,255,255));
			}
			if(can_act){
				DrawString(400, 16, "ACTION : key 2", GetColor(255,255,255));
			}
			DrawString(400, 32, "END    : key 3", GetColor(255,255,255));
		}
		break;
	case MOVE:
		DrawString(400,  0, "where?", GetColor(255,255,255));
		DrawString(400, 16, "assign : key 1", GetColor(255,255,255));
		DrawString(400, 32, "cancel : key 3", GetColor(255,255,255));
		break;
	case ACTION:
		DrawString(400,  0, "to whom?", GetColor(255,255,255));
		DrawString(400, 16, "assign : key 1", GetColor(255,255,255));
		DrawString(400, 32, "cancel : key 3", GetColor(255,255,255));
		break;
	case END:
		if(pos == Cursor::pos){
			DrawString(400,  0, "end.", GetColor(255,255,255));
		}
		break;
	}
}

void Player::attack(vector<Enemy> &enemies){
	if(state != ACTION) return;

	if(Keyboard::pushed(KEY_INPUT_3)) state = SELECT;
	if(Keyboard::pushed(KEY_INPUT_1)){
		state = SELECT;
		for(auto& enemy : enemies){
			if(Stage::isBrightened(Cursor::pos)){
				if(enemy.pos == Cursor::pos){
					can_act = false;

					int diff = str - enemy.getDef();
					if(diff <= 0){
						break;
					}
					enemy.setHP(enemy.getHP() - diff);
					break;
				}
			}
		}
	}
}