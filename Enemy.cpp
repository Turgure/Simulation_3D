#include <DxLib.h>
#include "Object.h"
#include "Cursor.h"
#include "ChipBrightnessManager.h"
#include "Stage.h"

Enemy::Enemy(int x, int y, int id, int hp, int mp, int str, int def, int agi, int mobility):
	mypos(x, y),
	move_pos(),
	act_pos(){
		image = GetColor(255, 0, 0);
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
		can_act = false;
		moved = false;
		attacked = false;
		wait_time = 0;
		attack_range = 3;
}

void Enemy::update(){
	myvec = VGet(mypos.y*chipsize, Stage::getHeight(mypos)*chipheight, mypos.x*chipsize);
	Stage::setObjectAt(mypos, this);
}

void Enemy::draw(){
	DrawSphere3D(VAdd(myvec, VGet(chipsize/2, chipsize/2, chipsize/2)), chipsize/2 -5, 50, image, image, true);
	
	//show id on object
	//DrawFormatString(mypos.getXByPx(), mypos.getYByPx(), GetColor(255,255,255), "%d", id);

	if(mypos == Cursor::pos()){
		showStatus(200, 0);
	}

	switch(state){
	case MOVE:
		ChipBrightnessManager::range(mypos, mobility, false);
		break;
	case ACTION:
		if(can_act)
			ChipBrightnessManager::reachTo(mypos, ChipBrightnessManager::getColorAttack(), 1, attack_range);
		break;
	default:
		break;
	}
}

void Enemy::action(){
	DrawFormatString(0, 0, GetColor(255,255,255), "enemy %d's turn.", id);

	switch(state){
	case SELECT:
		if(can_act)
			state = ACTION;
		else if(can_move)
			state = MOVE;
		else
			state = END;
		break;

	case MOVE:
		//calcMove(player.pos());
		Cursor::pos() = move_pos;
		state = WAIT;
		break;

	case ACTION:
		//calcAttack(players);
		if(can_act)
			Cursor::pos() = act_pos;
		state = WAIT;
		//attack(players);
		break;

	case END:
		can_move = false;
		can_act = false;
		Stage::disbrighten();
		break;

	case WAIT:
		//if(isCountOver(30))
		if(can_act){
			break;
		} else if(can_move){
			if(Cursor::pos() == mypos){
				can_move = false;
				Stage::disbrighten();
				state = SELECT;
				break;
			}
			if(isCountOver(30)){
				mypos = Cursor::pos();
				can_move = false;
				moved = false;
				Stage::disbrighten();
				state = SELECT;
			}
		}
		break;
	}
}

void Enemy::endMyTurn(){
	state = SELECT;
	ATBgauge =  100;
	wait_time = 0;
	//if(moved) ATBgauge += 20;
	//if(attacked) ATBgauge += 60;
	can_move = true;
	can_act = false;
	moved = false;
	attacked = false;
}


void Enemy::stepATBgauge(){
	ATBgauge -= agi;
}

bool Enemy::isCountOver(int time){
	if(++wait_time > time){
		wait_time = 0;
		return true;
	}
	else return false;
}

void Enemy::calcMove(vector<Player>& players){
	ChipBrightnessManager::range(mypos, mobility, false);

	Position finalpos(-1, -1);
	int dist = INT_MAX, diff;
	for(int y = 0; y < Stage::getDepth(); ++y){
		for(int x = 0; x < Stage::getWidth(); ++x){
			Position checkpos(x, y);
			if(!Stage::isBrightened(checkpos) || Stage::getObjectAt(checkpos)) continue;

			for(auto& player : players){
				diff = mypos.getDist(checkpos, player.pos());
				//最適な間合い（？）
				if(diff == attack_range){
					move_pos = finalpos = checkpos;
					Stage::disbrighten();
					return;
				}

				if(diff <= dist){
					finalpos = checkpos;
					dist = diff;

				}
			}
		}
	}
	move_pos = finalpos;
	Stage::disbrighten();
}

void Enemy::calcAttack(vector<Player>& players){
	if(attacked) return;

	ChipBrightnessManager::reachTo(mypos, ChipBrightnessManager::getColorAttack(), 1, attack_range);

	Position finalpos(-1, -1);
	int diff;
	for(int y = 0; y < Stage::getDepth(); ++y){
		for(int x = 0; x < Stage::getWidth(); ++x){
			Position checkpos(x, y);
			if(!Stage::isBrightened(checkpos)) continue;

			for(auto& player : players){
				diff = mypos.getDist(checkpos, player.pos());
				if(diff == 0){
					act_pos = finalpos = checkpos;
					can_act = true;
					Stage::disbrighten();
					return;
				}
			}
		}
	}
	//探索にヒットしなかったら
	act_pos = finalpos;
	Stage::disbrighten();
}

void Enemy::attack(vector<Player>& players){
	if(act_pos.x < 0) return;
	if(!can_act && attacked) return;
	if(!isCountOver(30)) return;

	can_act = false;
	attacked = true;
	state = SELECT;

	if(Stage::isBrightened(Cursor::pos())){
		for(auto& player : players){
			int diff = str - player.getDef();
			if(diff <= 0) continue;

			if(player.pos() == Cursor::pos()){
				player.setHP(player.getHP() - diff);
				break;
			}
		}
	}
}