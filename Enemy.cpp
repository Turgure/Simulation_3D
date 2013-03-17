#include <DxLib.h>
#include "Object.h"
#include "Cursor.h"
//#include "Event.h"
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
	myvec = VGet(mypos.getY()*chipsize, Stage::getHeight(mypos.getX(), mypos.getY())*chipheight, mypos.getX()*chipsize);
	//Stage::setObjectAt(mypos.getX(), mypos.getY(), this);
}

void Enemy::draw(){
	DrawSphere3D(VAdd(myvec, VGet(chipsize/2, chipsize/2, chipsize/2)), chipsize/2, 50, image, image, true);
	//Event::DrawGraphOnMap(mypos.getX(), mypos.getY(), image);
	//show id on object
	//DrawFormatString(mypos.getXByPx(), mypos.getYByPx(), GetColor(255,255,255), "%d", id);

	if(mypos == Cursor::pos()){
		showStatus(200, 0);
	}

	switch(state){
	case MOVE:
		//Event::range(mypos.getX(), mypos.getY(), mobility, true);
		break;
	case ACTION:
		if(can_act)
			//Event::reachTo(mypos.getX(), mypos.getY(), Event::GetColorAttack(), 1, attack_range);
		break;
	default:
		break;
	}
}

void Enemy::action(){
	DrawFormatString(0, 48, GetColor(255,255,255), "enemy %d's turn.", id);

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
		//Cursor::set(move_pos.getX(), move_pos.getY());
		state = WAIT;
		break;

	case ACTION:
		//calcAttack(players);
		if(can_act)
			//Cursor::set(act_pos.getX(), act_pos.getY());
		state = WAIT;
		//attack(players);
		break;

	case END:
		can_move = false;
		can_act = false;
		//Stage::eraseBrightPoints();
		break;

	case WAIT:
		//if(isCountOver(30))
		if(can_act){
			break;
		} else if(can_move){
			//if(Cursor::pos().targetted(mypos.getX(), mypos.getY())){
				can_move = false;
				//Stage::eraseBrightPoints();
				state = SELECT;
				break;
			//}
			if(isCountOver(30)){
				//mypos.set(Cursor::pos().getX(), Cursor::pos().getY());
				can_move = false;
				moved = false;
				//Stage::eraseBrightPoints();
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

//void Enemy::calcMove(vector<Player>& players){
//	Event::range(mypos.getX(), mypos.getY(), mobility, true);
//
//	int finalX = -1, finalY = -1;
//	int dist = INT_MAX, diff;
//	for(int y = 0; y < Stage::getHeight(); ++y){
//		for(int x = 0; x < Stage::getWidth(); ++x){
//			if(!Stage::getBrightPoint(x, y) || Stage::getObjectAt(x, y)) continue;
//
//			for(auto& player : players){
//				diff = mypos.getDist(x, y, player.pos().getX(), player.pos().getY());
//				//最適な間合い（？）
//				if(diff == attack_range){
//					finalX = x, finalY = y;
//					move_pos.set(finalX, finalY);
//					Stage::eraseBrightPoints();
//					return;
//				}
//
//				if(diff <= dist){
//					finalX = x, finalY = y;
//					dist = diff;
//
//				}
//			}
//		}
//	}
//	move_pos.set(finalX, finalY);
//	Stage::eraseBrightPoints();
//}
//
//void Enemy::calcAttack(vector<Player>& players){
//	if(attacked) return;
//
//	Event::reachTo(mypos.getX(), mypos.getY(), Event::GetColorAttack(), 1, attack_range);
//
//	int finalX, finalY, diff;
//	for(int y = 0; y < Stage::getHeight(); ++y){
//		for(int x = 0; x < Stage::getWidth(); ++x){
//			if(!Stage::getBrightPoint(x, y)) continue;
//
//			for(auto& player : players){
//				diff = mypos.getDist(x, y, player.pos().getX(), player.pos().getY());
//				if(diff == 0){
//					finalX = x, finalY = y;
//					act_pos.set(finalX, finalY);
//					can_act = true;
//					Stage::eraseBrightPoints();
//					return;
//				}
//			}
//		}
//	}
//	//探索にヒットしなかったら
//	act_pos.set(-1, -1);
//	Stage::eraseBrightPoints();
//}
//
//void Enemy::attack(vector<Player>& players){
//	if(act_pos.getX() < 0) return;
//	if(!can_act && attacked) return;
//	if(!isCountOver(30)) return;
//
//	can_act = false;
//	attacked = true;
//	state = SELECT;
//
//	if(Stage::getBrightPoint(Cursor::pos().getX(), Cursor::pos().getY())){
//		for(auto& player : players){
//			int diff = str - player.getDef();
//			if(diff <= 0) continue;
//
//			if(player.pos().targetted(Cursor::pos().getX(), Cursor::pos().getY())){
//				player.setHP(player.getHP() - diff);
//				break;
//			}
//		}
//	}
//}