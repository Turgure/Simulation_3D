﻿#include <map>
#include "Object.h"
#include "Stage.h"
#include "Cursor.h"
#include "ChipBrightnessManager.h"

Enemy::Enemy(string name, int x, int y, int hp, int mp, int str, int def, int agi, int mobility, int jump_power):
	pos(x, y),
	move_pos(),
	act_pos(){

		models.push_back( MV1LoadModel("data/image/3Dmodel/chara/enemy/jelly_yellow_waiting.pmx") );
		models.push_back( MV1LoadModel("data/image/3Dmodel/chara/enemy/jelly_yellow_attack01.pmx") );
		models.push_back( MV1LoadModel("data/image/3Dmodel/chara/enemy/jelly_yellow_attack02.pmx") );
		models.push_back( MV1LoadModel("data/image/3Dmodel/chara/enemy/jelly_yellow_attack03.pmx") );
		models.push_back( MV1LoadModel("data/image/3Dmodel/chara/enemy/jelly_yellow_attack04.pmx") );
		models.push_back( MV1LoadModel("data/image/3Dmodel/chara/enemy/jelly_yellow_attack05.pmx") );
		models.push_back( MV1LoadModel("data/image/3Dmodel/chara/enemy/jelly_yellow_attack06.pmx") );
		for(auto& model : models){
			MV1SetScale(model, VGet(3.0f, 3.0f, 3.0f));	//拡大
			MV1SetUseZBuffer(model, true);
		}

		LoadDivGraph("data/image/attackeffect/thunder.png", 8, 8, 1, 48, 48, attack_effect);
		
		mv_mng.current_dir = SOUTH;
		mv_mng.setObjectDirection(models[0]);	//向き

		this->name = name;
		this->hp = maxhp = hp;
		this->mp = maxmp = mp;
		this->str = str;
		this->def = def;
		this->agi = agi;
		this->mobility = mobility;
		this->jump_power = jump_power;
		state = SELECT;
		ATBgauge = 100;
		can_move = false;
		can_act = false;
		moved = false;
		attacked = false;
		has_attacked = false;
		has_brightened = false;
		wait_time = 0;
		attack_range = 3;
		attack_status = 0;
}

void Enemy::update(){
	if(Stage::getID(pos) != 2){
		myvec = VAdd(VGet(pos.y*chipsize, Stage::getHeight(pos)*chipheight, pos.x*chipsize), mv_mng.diff);
	} else {
		myvec = VAdd(VGet(pos.y*chipsize, Stage::getHeight(pos)*chipheight - chipheight/2, pos.x*chipsize), mv_mng.diff);
	}

	//3Dモデルの配置
	for(auto& model : models){
		MV1SetPosition(model, VAdd(myvec, VGet(chipsize/2, 0, chipsize/2)));
	}
	Stage::setObjectAt(pos, this);
}

void Enemy::draw(){
	if(isMyTurn()){
		DrawFormatString(0, 0, GetColor(255,255,255), "%s's turn", name.c_str());
	}
	
	// ３Ｄモデルの描画
	MV1DrawModel(models[attack_status]);
	if(attack_status != 0){
		DrawGraph(295, 200, attack_effect[attack_status-1], true);
	}

	if(pos == Cursor::pos){
		showStatus();
	}
	
	if(has_attacked){
		static int cnt;
		++cnt;
		DrawFormatString(320, 210 - cnt, GetColor(255,0,0), "%d", damage);
		if(cnt >= 30){
			has_attacked = false;
			cnt = 0;
		}
	}

	switch(state){
	case MOVE:
		if(!has_brightened){
			ChipBrightnessManager::range(pos, mobility, true, this);
			has_brightened = true;
		}
		break;
	case ACTION:
		if(can_act && !has_brightened){
			ChipBrightnessManager::reachTo(pos, ChipBrightnessManager::getColorAttack(), 1, attack_range);
			has_brightened = true;
		}
		break;
	default:
		break;
	}
}

void Enemy::action(){
	switch(state){
	case SELECT:
		if(can_act)
			changeState(state, ACTION);
		else if(can_move)
			changeState(state, MOVE);
		else
			changeState(state, END);
		break;

	case MOVE:
		//calcMove(player.pos);
		if(can_move) Cursor::pos = move_pos;
		changeState(state, WAIT);
		break;

	case ACTION:
		//calcAttack(players);
		if(can_act) Cursor::pos = act_pos;
		changeState(state, ATTACK);
		//attack(players);
		break;

	case ATTACK:
		changeState(state, WAIT);
		break;

	case END:
		can_move = false;
		can_act = false;
		Stage::disbrighten();
		has_brightened = false;
		break;

	case WAIT:
		if(can_act){
			if(isCountOver(30)){
				//attack(players)
				changeState(state, ATTACKING);
			}
			break;
		} else if(can_move){
			if(isCountOver(30)){
				mv_mng.trackMovement(pos, Cursor::pos, mobility, this);
				changeState(state, MOVING);
			}
		}
		break;

	case ATTACKING:
		break;

	case MOVING:
		static int order;
		static Position topos;
		mv_mng.move(models, order, pos, topos);
		if(abs(topos.x*chipsize-myvec.z) < 1.0 && abs(topos.y*chipsize-myvec.x) < 1.0){
			pos = topos;
			mv_mng.jump_path = NULL;
			mv_mng.diff = VGet(0.0f, 0.0f, 0.0f);
			if(++order == mv_mng.path.size()){
				order = 0;
				can_move = false;
				moved = true;
				Stage::disbrighten();
				has_brightened = false;
				changeState(state, SELECT);
			}
		}
		break;
	}
}

void Enemy::endMyTurn(){
	state = SELECT;
	wait_time = 0;
	ATBgauge += 20;
	if(moved) ATBgauge += 40;
	if(attacked) ATBgauge += 60;
	can_move = false;
	can_act = false;
	moved = false;
	attacked = false;
}


void Enemy::stepATBgauge(){
	ATBgauge -= agi;
}

void Enemy::resetATBgauge(){
	ATBgauge = 100;
}

bool Enemy::isCountOver(int time){
	if(++wait_time > time){
		wait_time = 0;
		return true;
	}
	else return false;
}

void Enemy::calcMove(const vector<Player>& players){
	if(moved) return;

	ChipBrightnessManager::range(pos, mobility, true, this);

	Position finalpos(-1, -1);
	int dist = INT_MAX, diff;
	for(int y = 0; y < Stage::getDepth(); ++y){
		for(int x = 0; x < Stage::getWidth(); ++x){
			Position checkpos(x, y);
			if(!Stage::isBrightened(checkpos) || Stage::getObjectAt(checkpos)) continue;

			for(auto& player : players){
				diff = checkpos.getDist(player.pos);
				//最適な間合い（？）
				if(diff == attack_range){
					move_pos = finalpos = checkpos;
					can_move = true;
					Stage::disbrighten();
					return;
				}

				if(diff <= dist){
					finalpos = checkpos;
					can_move = true;
					dist = diff;

				}
			}
		}
	}
	move_pos = finalpos;
	Stage::disbrighten();
}

void Enemy::calcAttack(const vector<Player>& players){
	if(attacked) return;

	ChipBrightnessManager::reachTo(pos, ChipBrightnessManager::getColorAttack(), 1, attack_range);

	map<int, Position> candidate_pos;
	Position finalpos(-1, -1);
	for(int y = 0; y < Stage::getDepth(); ++y){
		for(int x = 0; x < Stage::getWidth(); ++x){
			Position checkpos(x, y);
			if(!Stage::isBrightened(checkpos)) continue;

			for(auto& player : players){
				if(checkpos == player.pos){
					int damage = str - player.getDef();
					candidate_pos[damage] = checkpos;
				}
			}
		}
	}

	//最大ダメージを与えられる敵を狙う
	int max = -1;
	if(!candidate_pos.empty()){
		for(auto& cp : candidate_pos){
			if(max < cp.first) max = cp.first;
		}
		finalpos = candidate_pos[max];
		can_act = true;
	}
	act_pos = finalpos;
	Stage::disbrighten();
}

void Enemy::attack(vector<Player>& players){
	if(state != ATTACKING) return;
	if(act_pos.x < 0) return;
	if(!can_act && attacked) return;
	
	static bool checked = false;
	for(auto& player = players.begin(); player != players.end(); ++player){
		if(checked) break;

		if(player->pos == act_pos){
			PlaySoundMem(Sound::thunder, DX_PLAYTYPE_BACK);
			int diff = str-player->getDef() > 0 ? (str-player->getDef())*(((double)GetRand(40)/100)+0.8) : 0;
			player->setDamage(diff);
			player->setHP(player->getHP() - diff);
			
			//向きの指定
			for(auto& model : models){
				mv_mng.setObjectDirection(model, player->pos - pos);
			}
			
			checked = true;
			break;
		}
	}

	static int atk_rate;
	if(++atk_rate >= 5){
		++attack_status;
		atk_rate = 0;
	}

	if(attack_status > 6){
		changeState(state, SELECT);
		attack_status = 0;
		checked = false;
		has_brightened = false;
		can_act = false;
		attacked = true;
	}
	
	Stage::disbrighten();

}

void Enemy::assignDirection(const vector<Player>& players){
	Position finalpos(-1, -1);
	for(int y = 0; y < Stage::getDepth(); ++y){
		for(int x = 0; x < Stage::getWidth(); ++x){
			Position checkpos(x, y);

			for(auto& player : players){
				if(checkpos == player.pos){
					//距離がより短い || 代入1回目
					if(pos.getDist(checkpos) < pos.getDist(finalpos) || finalpos.x < 0){
						finalpos = checkpos;
					}
				}
			}
		}
	}
	for(auto& model : models){
		mv_mng.setObjectDirection(model, finalpos - pos);
	}
}
