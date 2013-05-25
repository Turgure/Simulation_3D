#include "Object.h"
#include "Keyboard.h"
#include "Stage.h"
#include "Camera.h"
#include "Cursor.h"
#include "ChipBrightnessManager.h"

Player::Player(string name, int x, int y, int hp, int mp, int str, int def, int agi, int mobility, int jump_power):pos(x, y){
	picture_frame = LoadGraph("data/image/frame/frame.png");
	models.push_back( MV1LoadModel("data/image/3Dmodel/chara/friend/waitting.pmx") );
	models.push_back( MV1LoadModel("data/image/3Dmodel/chara/friend/attack01.pmx") );
	models.push_back( MV1LoadModel("data/image/3Dmodel/chara/friend/attack02.pmx") );
	models.push_back( MV1LoadModel("data/image/3Dmodel/chara/friend/attack03.pmx") );
	models.push_back( MV1LoadModel("data/image/3Dmodel/chara/friend/attack04.pmx") );
	models.push_back( MV1LoadModel("data/image/3Dmodel/chara/friend/attack05.pmx") );
	models.push_back( MV1LoadModel("data/image/3Dmodel/chara/friend/attack06.pmx") );
	for(auto& model : models){
		MV1SetScale(model, VGet(0.7f, 0.7f, 0.7f));	//拡大
		MV1SetUseZBuffer(model, true);
	}

	LoadDivGraph("data/image/attackeffect/fire.png", 7, 7, 1, 48, 48, attack_effect);

	mv_mng.current_dir = NORTH;
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
	can_move = true;
	can_act = true;
	has_attacked = false;
	has_brightened = false;
	attack_status = 0;
	rest_item = 2;
}

void Player::update(){
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
		DrawFormatString(0, 0, GetColor(255,255,255), "%s's turn", name.c_str());
	}

	// ３Ｄモデルの描画
	MV1DrawModel(models[attack_status]);
	if(attack_status != 0){
		DrawGraph(295, 210, attack_effect[attack_status-1], true);
	}

	if(pos == Cursor::pos){
		showStatus();
	}

	if(has_attacked){
		static int cnt;
		++cnt;
		int color;

		if(damage >= 0) color = GetColor(255, 0, 0);
		else color = GetColor(0, 255, 0);

		DrawFormatString(330, 210 - cnt, color, "%d", abs(damage));
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
					PlaySoundMem(Sound::cancel, DX_PLAYTYPE_BACK);
					command.clear();
				}
			}

			if(Keyboard::pushed(KEY_INPUT_Z)){
				if(command.commandIs("移動") && can_move){
					if(changeState(state, MOVE)){
						PlaySoundMem(Sound::step, DX_PLAYTYPE_BACK);
						command.step();
					}
				}
				if(command.commandIs("行動") && can_act){
					if(changeState(state, ACTION)){
						PlaySoundMem(Sound::step, DX_PLAYTYPE_BACK);
						command.step();
					}
				}
				if(command.commandIs("終了")){
					if(changeState(state, END)){
						PlaySoundMem(Sound::step, DX_PLAYTYPE_BACK);
						command.clear();
					}
				}
			}
		}
		break;

	case MOVE:
		if(Keyboard::pushed(KEY_INPUT_X)){
			if(changeState(state, SELECT)){
				PlaySoundMem(Sound::cancel, DX_PLAYTYPE_BACK);
				Cursor::pos = pos;
				command.clear();
			}
		}
		if(Keyboard::pushed(KEY_INPUT_Z)){
			command.clear();
			if(Stage::isBrightened(Cursor::pos) && !Stage::getObjectAt(Cursor::pos)){
				if(changeState(state, MOVING)){
					PlaySoundMem(Sound::step, DX_PLAYTYPE_BACK);
					mv_mng.trackMovement(pos, Cursor::pos, mobility, this);
				}
			} else {
				if(changeState(state, SELECT)){
					PlaySoundMem(Sound::cancel, DX_PLAYTYPE_BACK);
					Cursor::pos = pos;
				}
			}
		}
		break;

	case ACTION:
		Stage::disbrighten();
		has_brightened = false;
		if(Keyboard::pushed(KEY_INPUT_X)){
			if(changeState(state, SELECT)){
				PlaySoundMem(Sound::cancel, DX_PLAYTYPE_BACK);
				command.back();
			}
		}
		if(Keyboard::pushed(KEY_INPUT_Z)){
			if(command.commandIs("たたかう")){
				if(changeState(state, ATTACK)){
					PlaySoundMem(Sound::step, DX_PLAYTYPE_BACK);
					command.step();
				}
			}

			if(command.commandIs("アイテム")){
				if(changeState(state, SELECT)){
					if(rest_item-- > 0){
						PlaySoundMem(Sound::item, DX_PLAYTYPE_BACK);
						setDamage(-50);
						setHP(getHP() - getDamage());
						if(getHP() > maxhp) setHP(maxhp);

						can_act = false;
						has_attacked = true;
						command.clear();
					}
				}
			}
		}
		break;

	case ATTACK:
		if(Keyboard::pushed(KEY_INPUT_X)){
			if(changeState(state, ACTION)){
				PlaySoundMem(Sound::cancel, DX_PLAYTYPE_BACK);
				Cursor::pos = pos;
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
				PlaySoundMem(Sound::step, DX_PLAYTYPE_BACK);
				changeState(state, SELECT);
			}
		}
		if(Keyboard::pushed(KEY_INPUT_X)){
			PlaySoundMem(Sound::cancel, DX_PLAYTYPE_BACK);
			Cursor::pos = pos;
		}
		break;

	case ATTACKING:
		//attack(enemies);
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
				PlaySoundMem(Sound::shot, DX_PLAYTYPE_BACK);
				int diff = str-enemy->getDef()> 0 ? (str-enemy->getDef())*(((double)GetRand(40)/100)+0.8) : 0;
				enemy->setDamage(diff);
				enemy->setHP(enemy->getHP() - diff);

				//向きの指定
				for(auto& model : models){
					mv_mng.setObjectDirection(model, enemy->pos - pos);
				}

				checked = true;
				break;
			}

			//色付き空白マス
			if(enemy == enemies.end()-1){
				if(changeState(state, ACTION)){
					PlaySoundMem(Sound::cancel, DX_PLAYTYPE_BACK);
					Cursor::pos = pos;
					command.back();
					Stage::disbrighten();
					has_brightened = false;
					return;
				}
			}
		} else {
			//色なしマス
			if(changeState(state, ACTION)){
				PlaySoundMem(Sound::cancel, DX_PLAYTYPE_BACK);
				Cursor::pos = pos;
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
		PlaySoundMem(Sound::cursor, DX_PLAYTYPE_BACK);
		mv_mng.setObjectDirection(models[0], Camera::getDirection(FRONT));
	} else if(Keyboard::pushed(KEY_INPUT_DOWN)){
		PlaySoundMem(Sound::cursor, DX_PLAYTYPE_BACK);
		mv_mng.setObjectDirection(models[0], Camera::getDirection(BACK));
	} else if(Keyboard::pushed(KEY_INPUT_LEFT)){
		PlaySoundMem(Sound::cursor, DX_PLAYTYPE_BACK);
		mv_mng.setObjectDirection(models[0], Camera::getDirection(LEFT));
	} else if(Keyboard::pushed(KEY_INPUT_RIGHT)){
		PlaySoundMem(Sound::cursor, DX_PLAYTYPE_BACK);
		mv_mng.setObjectDirection(models[0], Camera::getDirection(RIGHT));
	}

	if(Keyboard::pushed(KEY_INPUT_Z)){
		PlaySoundMem(Sound::step, DX_PLAYTYPE_BACK);
		return true;
	} else {
		return false;
	}
}
