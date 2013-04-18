﻿#include <DxLib.h>
#include "Object.h"
#include "Keyboard.h"
#include "Cursor.h"
#include "ChipBrightnessManager.h"
#include "Stage.h"

Player::Player(int x, int y, int id, int hp, int mp, int str, int def, int agi, int mobility):pos(x, y){
	model = MV1LoadModel("data/image/3Dmodel/chara/boko.pmd");
	MV1SetScale(model, VGet(3.0f, 3.0f, 3.0f));	//拡大
	MV1SetRotationXYZ(model, VGet(0.0f, 90 * DX_PI_F/180.0f, 0.0f));	//向き
	mv_mng.current_dir = mv_mng.NORTH;

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
	myvec = VAdd(VGet(pos.y*chipsize, Stage::getHeight(pos)*chipheight, pos.x*chipsize), mv_mng.diff);
	//3Dモデルの配置
	MV1SetPosition(model, VAdd(myvec, VGet(chipsize/2, 0, chipsize/2)));
	Stage::setObjectAt(pos, this);

	if(isMyTurn()){
		switch(state){
		case SELECT:
		case ACTION:
			command.update();
			break;
		default:
			break;
		}
	}

	DrawFormatString(pos.x*50, pos.y*20, GetColor(255,255,255), "%d", state);
}

void Player::draw(){
	// ３Ｄモデルの描画
	MV1DrawModel(model);

	if(pos == Cursor::pos){
		showStatus(200, 0);
	}

	if(isMyTurn()){
		showCommand();
	}

	switch(state){
	case MOVE:
		ChipBrightnessManager::range(pos, mobility, true);
		break;
	case ATTACK:
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
			if(Keyboard::pushed(KEY_INPUT_X)){
				state = WAIT;
			}

			if(Keyboard::pushed(KEY_INPUT_Z)){
				if(command.commandIs("MOVE") && can_move){
					state = MOVE;
				}
				if(command.commandIs("ACTION") && can_act){
					state = ACTION;
				}
				if(command.commandIs("END")){
					state = END;
				}
				command.step();
			}
		}
		break;

	case MOVE:
		if(Keyboard::pushed(KEY_INPUT_X)){
			command.back();
			state = SELECT;
			Cursor::pos = pos;
		}
		if(Keyboard::pushed(KEY_INPUT_Z)){
			command.clear();
			if(Stage::isBrightened(Cursor::pos) && !Stage::getObjectAt(Cursor::pos)){
				state = MOVING;
				mv_mng.trackMovement(pos, Cursor::pos, mobility);
			} else {
				state = SELECT;
				Cursor::pos = pos;
			}
		}
		break;

	case ACTION:
		if(Keyboard::pushed(KEY_INPUT_X)){
			command.back();
			state = SELECT;
		}
		if(Keyboard::pushed(KEY_INPUT_Z)){
			if(command.commandIs("たたかう")){
				state = ATTACK;
			}
		}
		break;

	case ATTACK:
		//attack(enemies);
		break;

	case END:
		can_move = false;
		can_act = false;
		Stage::disbrighten();
		break;

	case WAIT:
		if(Keyboard::pushed(KEY_INPUT_Z)){
			if(Cursor::pos == pos){
				state = SELECT;
			}
		}
		if(Keyboard::pushed(KEY_INPUT_X)){
			state = SELECT;
			Cursor::pos = pos;
		}
		break;

	case MOVING:
		static int order;
		mv_mng.current_dir = mv_mng.path[order];
		mv_mng.setObjectDirection(model);

		Position topos = pos + mv_mng.dir[mv_mng.current_dir];
		mv_mng.diff = VAdd(mv_mng.diff,
			VGet(mv_mng.dir[mv_mng.current_dir].y*chipsize*mv_mng.moving_rate, 0.0f, mv_mng.dir[mv_mng.current_dir].x*chipsize*mv_mng.moving_rate));

		if(abs(topos.x*chipsize-myvec.z) < 1.0 && abs(topos.y*chipsize-myvec.x) < 1.0){
			pos = topos;
			mv_mng.diff = VGet(0.0f, 0.0f, 0.0f);
			if(++order == mv_mng.path.size()){
				order = 0;
				can_move = false;
				Stage::disbrighten();
				state = SELECT;
			}
		}
		break;
	}
}

void Player::endMyTurn(){
	command.setSelectNum(0);
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
	case ACTION:
		command.draw(400, 0);
		break;
	case MOVE:
		DrawString(400,  0, "where?", GetColor(255,255,255));
		break;
	case ATTACK:
		DrawString(400,  0, "to whom?", GetColor(255,255,255));
		break;
	case END:
		DrawString(400,  0, "end.", GetColor(255,255,255));
		break;
	}
}

void Player::attack(vector<Enemy> &enemies){
	if(state != ATTACK) return;

	if(Keyboard::pushed(KEY_INPUT_X)){
		command.back();
		state = ACTION;
		Cursor::pos = pos;
	}
	if(Keyboard::pushed(KEY_INPUT_Z)){
		for(auto& enemy : enemies){
			if(Stage::isBrightened(Cursor::pos)){
				if(enemy.pos == Cursor::pos){
					command.clear();
					state = SELECT;
					Cursor::pos = pos;
					can_act = false;

					int diff = str - enemy.getDef();
					if(diff <= 0){
						break;
					}
					enemy.setHP(enemy.getHP() - diff);
					break;
				}
			} else {
				command.back();
				state = ACTION;
				Cursor::pos = pos;
			}
		}
		Stage::disbrighten();
	}
}