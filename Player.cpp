﻿#include <DxLib.h>
#include "Object.h"
#include "Keyboard.h"
//#include "Cursor.h"
//#include "Event.h"
//#include "Stage.h"

Player::Player(int x, int z, int id, int hp, int mp, int str, int def, int agi, int mobility)/*:mypos(x, z)*/{
	image = GetColor(0, 0, 255);
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
	//Stage::setObjectAt(mypos.getXByMap(), mypos.getYByMap(), this);
}

void Player::draw(){
	//Event::DrawGraphOnMap(mypos.getXByMap(), mypos.getYByMap(), image);
	////show id on object
	//DrawFormatString(mypos.getXByPx(), mypos.getYByPx(), GetColor(255,255,255), "%d", id);

	//if(mypos.targetted(Cursor::pos().getXByMap(), Cursor::pos().getYByMap())){
	//	showStatus(200, 0);
	//}

	showCommand();

	switch(state){
	case MOVE:
		//Event::range(mypos.getXByMap(), mypos.getYByMap(), mobility, true);
		break;
	case ACTION:
		//Event::aroundTo(mypos.getXByMap(), mypos.getYByMap(), Event::GetColorAttack(), 3);
		break;
	default:
		break;
	}
}

void Player::action(){
	DrawFormatString(0, 48, GetColor(255,255,255), "player %d's turn.", id);

	if(!can_move && !can_act) state = END;

	switch(state){
	case SELECT:
		//Stage::eraseBrightPoints();
		//if(mypos.targetted(Cursor::pos().getXByMap(), Cursor::pos().getYByMap())){
			if(Keyboard::get(KEY_INPUT_1) == 1 && can_move) state = MOVE;
			if(Keyboard::get(KEY_INPUT_2) == 1 && can_act) state = ACTION;
			if(Keyboard::get(KEY_INPUT_3) == 1) state = END;
		//}
		break;

	case MOVE:
		if(Keyboard::get(KEY_INPUT_3) == 1) state = SELECT;
		if(Keyboard::get(KEY_INPUT_1) == 1){
			state = SELECT;
			//if(Stage::getBrightPoint(Cursor::pos().getXByMap(), Cursor::pos().getYByMap()) &&
				//!Stage::getObjectAt(Cursor::pos().getXByMap(), Cursor::pos().getYByMap())){
					//mypos.setByMap(Cursor::pos().getXByMap(), Cursor::pos().getYByMap());
					can_move = false;
			//}
		}
		break;

	case ACTION:
		//attack(enemies);
		break;

	case END:
		can_move = false;
		can_act = false;
		//Stage::eraseBrightPoints();
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
		//if(mypos.targetted(Cursor::pos().getXByMap(), Cursor::pos().getYByMap())){
			if(can_move){
				DrawString(400, 0, "MOVE   : key 1", GetColor(255,255,255));
			}
			if(can_act){
				DrawString(400, 16, "ACTION : key 2", GetColor(255,255,255));
			}
			DrawString(400, 32, "END    : key 3", GetColor(255,255,255));
		//}
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
		//if(mypos.targetted(Cursor::pos().getXByMap(), Cursor::pos().getYByMap())) 
			DrawString(400,  0, "end.", GetColor(255,255,255));
		break;
	}
}

void Player::attack(vector<Enemy> &enemies){
	if(state != ACTION) return;

	if(Keyboard::get(KEY_INPUT_3) == 1) state = SELECT;
	//if(Keyboard::get(KEY_INPUT_1) == 1){
	//	state = SELECT;
	//	for(auto& enemy : enemies){
	//		if(Stage::getBrightPoint(Cursor::pos().getXByMap(), Cursor::pos().getYByMap())){
	//			if(enemy.pos().targetted(Cursor::pos().getXByMap(), Cursor::pos().getYByMap())){
	//				can_act = false;

	//				int diff = str - enemy.getDef();
	//				if(diff <= 0){
	//					break;
	//				}
	//				enemy.status.setHP(enemy.getHP() - diff);
	//				break;
	//			}
	//		}
	//	}
	//}
}