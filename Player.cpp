#include "Object.h"
#include "Keyboard.h"
#include "Stage.h"
#include "Camera.h"
#include "Cursor.h"
#include "ChipBrightnessManager.h"

Player::Player(string name, int x, int y, int hp, int mp, int str, int def, int agi, int mobility, int jump_power):pos(x, y){
	picture_frame = LoadGraph("data/image/frame/f02_208x160.png");
	model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/waitting.pmx") );
	model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack01.pmx") );
	model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack02.pmx") );
	model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack03.pmx") );
	model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack04.pmx") );
	model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack05.pmx") );
	model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack06.pmx") );
	for(int i = 0; i < 7; i++){
		MV1SetScale(model[i], VGet(3.0f, 3.0f, 3.0f));	//拡大
	}

	LoadDivGraph("data/image/attackeffect/fire.png",10,10,1,48,48, attackeffect);
	
	mv_mng.current_dir = NORTH;
	mv_mng.setObjectDirection(model[0]);	//向き

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
	can_move = true;
	can_act = true;
	has_attacked = false;
	has_brightened = false;
	attack_status = 0;
}

void Player::update(){
	myvec = VAdd(VGet(pos.y*chipsize, Stage::getHeight(pos)*chipheight, pos.x*chipsize), mv_mng.diff);
	//3Dモデルの配置
	for(int i = 0; i < 7; ++i){
		MV1SetPosition(model[i], VAdd(myvec, VGet(chipsize/2, 0, chipsize/2)));
	}
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
}

void Player::draw(){
	if(isMyTurn()){
		DrawFormatString(0, 0, GetColor(255,255,255), "%s's turn.", name.c_str());
	}

	// ３Ｄモデルの描画
	switch(attack_status){
	case 0:MV1DrawModel(model[0]);	break;
	case 1:MV1DrawModel(model[1]);	DrawGraph(295, 210, attackeffect[1], true); break;
	case 2:MV1DrawModel(model[2]);	DrawGraph(295, 210, attackeffect[2], true); break;
	case 3:MV1DrawModel(model[3]);	DrawGraph(295, 210, attackeffect[3], true); break;
	case 4:MV1DrawModel(model[4]);	DrawGraph(295, 210, attackeffect[4], true); break;
	case 5:MV1DrawModel(model[5]);	DrawGraph(295, 210, attackeffect[5], true); break;
	case 6:MV1DrawModel(model[6]);	DrawGraph(295, 210, attackeffect[6], true); break;
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
	case ATTACK:
		if(!has_brightened){
			ChipBrightnessManager::reachTo(pos, ChipBrightnessManager::getColorAttack(), 1, 3);
			has_brightened = true;
		}
		break;
	default:
		break;
	}
}
void Player::action(){
	if(!can_move && !can_act) changeState(state, END);

	switch(state){
	case SELECT:
		Stage::disbrighten();
		has_brightened = false;
		if(pos == Cursor::pos){
			if(Keyboard::pushed(KEY_INPUT_X)){
				if(changeState(state, WAIT)){
					command.clear();
				}
			}

			if(Keyboard::pushed(KEY_INPUT_Z)){
				if(command.commandIs("移動") && can_move){
					if(changeState(state, MOVE)){
						command.step();
					}
				}
				if(command.commandIs("行動") && can_act){
					if(changeState(state, ACTION)){
						command.step();
					}
				}
				if(command.commandIs("終了")){
					if(changeState(state, END)){
						command.clear();
					}
				}
			}
		}
		break;

	case MOVE:
		if(Keyboard::pushed(KEY_INPUT_X)){
			Cursor::pos = pos;
			if(changeState(state, SELECT)){
				command.clear();
			}
		}
		if(Keyboard::pushed(KEY_INPUT_Z)){
			command.clear();
			if(Stage::isBrightened(Cursor::pos) && !Stage::getObjectAt(Cursor::pos)){
				changeState(state, MOVING);
				mv_mng.trackMovement(pos, Cursor::pos, mobility, this);
			} else {
				Cursor::pos = pos;
				changeState(state, SELECT);
			}
		}
		break;

	case ACTION:
		Stage::disbrighten();
		has_brightened = false;
		if(Keyboard::pushed(KEY_INPUT_X)){
			if(changeState(state, SELECT)){
				command.back();
			}
		}
		if(Keyboard::pushed(KEY_INPUT_Z)){
			if(command.commandIs("たたかう")){
				if(changeState(state, ATTACK)){
					command.step();
				}
			}
		}
		break;

	case ATTACK:
		if(Keyboard::pushed(KEY_INPUT_X)){
			Cursor::pos = pos;
			if(changeState(state, ACTION)){
				command.back();
			}
		}
		if(Keyboard::pushed(KEY_INPUT_Z)){
			state = ATTACKING;
		}
		break;

	case END:
		can_move = false;
		can_act = false;
		Stage::disbrighten();
		has_brightened = false;
		break;

	case WAIT:
		if(Keyboard::pushed(KEY_INPUT_Z)){
			if(Cursor::pos == pos){
				changeState(state, SELECT);
			}
		}
		if(Keyboard::pushed(KEY_INPUT_X)){
			changeState(state, SELECT);
			Cursor::pos = pos;
		}
		break;

	case ATTACKING:
		//attack(enemies);
		break;

	case MOVING:
		static int order;
		static Position topos;
		mv_mng.current_dir = mv_mng.path[order];
		topos = pos + mv_mng.dir[mv_mng.current_dir];

		for(int i = 0; i < 7; ++i){
			mv_mng.setObjectDirection(model[i]);
		}

		mv_mng.diff = VAdd(mv_mng.diff,
			VGet(mv_mng.dir[mv_mng.current_dir].y*chipsize*mv_mng.moving_rate, 0.0f, mv_mng.dir[mv_mng.current_dir].x*chipsize*mv_mng.moving_rate));

		//高さが違うとき
		if(Stage::getHeight(topos) != Stage::getHeight(pos)){
			mv_mng.initJumpmotion(pos, topos);
			mv_mng.jump_path -= mv_mng.jump_dist*mv_mng.moving_rate;

			switch(mv_mng.jump){
			case mv_mng.UP:
				if(mv_mng.jump_path < mv_mng.jump_height){
					mv_mng.jump_dist *= -1;
				}
				break;
			case mv_mng.DOWN:
				if(mv_mng.jump_path < mv_mng.jump_height + mv_mng.step){
					mv_mng.jump_dist *= -1;
				}
				break;
			}
			mv_mng.diff = VAdd(mv_mng.diff, VGet(0.0f, mv_mng.jump_dist*mv_mng.moving_rate, 0.0f));
		}

		if(abs(topos.x*chipsize-myvec.z) < 1.0 && abs(topos.y*chipsize-myvec.x) < 1.0){
			pos = topos;
			mv_mng.jump_path = NULL;
			mv_mng.diff = VGet(0.0f, 0.0f, 0.0f);
			if(++order == mv_mng.path.size()){
				order = 0;
				can_move = false;
				Stage::disbrighten();
				has_brightened = false;
				changeState(state, SELECT);
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

void Player::resetATBgauge(){
	ATBgauge = 100;
}


void Player::drawCommand(){
	if(!isMyTurn()) return;

	switch(state){
	case SELECT:
	case ACTION:
		DrawGraph(DEFAULT_SCREEN_SIZE_X-208, DEFAULT_SCREEN_SIZE_Y-160-16-5, picture_frame, true);
		command.draw(DEFAULT_SCREEN_SIZE_X-170, DEFAULT_SCREEN_SIZE_Y-155);
		break;
	case MOVE:
	case ATTACK:
	case END:
		break;
	}
}

void Player::attack(vector<Enemy> &enemies){
	if(state != ATTACKING) return;

	static bool checked = false;
	for(auto& enemy = enemies.begin(); enemy != enemies.end(); ++enemy){
		if(checked) break;

		if(Stage::isBrightened(Cursor::pos)){
			if(enemy->pos == Cursor::pos){
				int diff = str-enemy->getDef() > 0 ? str-enemy->getDef() : 0;
				enemy->setDamage(diff);
				enemy->setHP(enemy->getHP() - diff);

				//向きの指定
				for(int i = 0; i < 7; ++i){
					mv_mng.setObjectDirection(model[i], enemy->pos - pos);
				}


				checked = true;
				break;
			}

			//色付き空白マス
			if(enemy == enemies.end()-1){
				Cursor::pos = pos;
				if(changeState(state, ACTION)){
					command.back();
					Stage::disbrighten();
					has_brightened = false;
					return;
				}
			}
		} else {
			//色なしマス
			Cursor::pos = pos;
			if(changeState(state, ACTION)){
				command.back();
				Stage::disbrighten();
				has_brightened = false;
				return;
			}
		}
	}


	static int atk_rate;
	if(++atk_rate >= 5){
		++attack_status;
		atk_rate = 0;
	}

	if(attack_status > 6){
		Cursor::pos = pos;
		changeState(state, SELECT);
		command.clear();
		attack_status = 0;
		checked = false;
		can_act = false;
		has_brightened = false;
	}
}

bool Player::assignDirection(){

	if(Keyboard::pushed(KEY_INPUT_UP)){
		mv_mng.setObjectDirection(model[0], Camera::getDirection(FRONT));
	} else if(Keyboard::pushed(KEY_INPUT_DOWN)){
		mv_mng.setObjectDirection(model[0], Camera::getDirection(BACK));
	} else if(Keyboard::pushed(KEY_INPUT_LEFT)){
		mv_mng.setObjectDirection(model[0], Camera::getDirection(LEFT));
	} else if(Keyboard::pushed(KEY_INPUT_RIGHT)){
		mv_mng.setObjectDirection(model[0], Camera::getDirection(RIGHT));
	}

	if(Keyboard::pushed(KEY_INPUT_Z)) return true;
	else return false;
}
