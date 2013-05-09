#include <DxLib.h>
#include "Object.h"
#include "Keyboard.h"
#include "Cursor.h"
#include "ChipBrightnessManager.h"
#include "Stage.h"

Player::Player(int x, int y, string name, int hp, int mp, int str, int def, int agi, int mobility):pos(x, y){
	model = MV1LoadModel("data/image/3Dmodel/chara/boko.pmd");
	MV1SetScale(model, VGet(3.0f, 3.0f, 3.0f));	//拡大
	MV1SetRotationXYZ(model, VGet(0.0f, 90 * DX_PI_F/180.0f, 0.0f));	//向き
	mv_mng.current_dir = mv_mng.NORTH;

	this->name = name;
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
		ChipBrightnessManager::range(pos, mobility, true, this);
		break;
	case ATTACK:
		ChipBrightnessManager::reachTo(pos, ChipBrightnessManager::getColorAttack(), 1, 3);
		break;
	default:
		break;
	}
}
void Player::action(){
	static double diffX = chipsize/2;
	static double posY;

	DrawFormatString(0, 0, GetColor(255,255,255), "%s's turn.", name.c_str());

	if(!can_move && !can_act) changeState(state, END);

	switch(state){
	case SELECT:
		Stage::disbrighten();
		if(pos == Cursor::pos){
			if(Keyboard::pushed(KEY_INPUT_X)){
				if(changeState(state, WAIT)){
					command.clear();
				}
			}

			if(Keyboard::pushed(KEY_INPUT_Z)){
				if(command.commandIs("MOVE") && can_move){
					if(changeState(state, MOVE)){
						command.step();
					}
				}
				if(command.commandIs("ACTION") && can_act){
					if(changeState(state, ACTION)){
						command.step();
					}
				}
				if(command.commandIs("END")){
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
				mv_mng.trackMovement(pos, Cursor::pos, mobility);
			} else {
				Cursor::pos = pos;
				changeState(state, SELECT);
			}
		}
		break;

	case ACTION:
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
				changeState(state, SELECT);
			}
		}
		if(Keyboard::pushed(KEY_INPUT_X)){
			changeState(state, SELECT);
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

		//高さが違うとき
		if(Stage::getHeight(topos) != Stage::getHeight(pos)){
			static int diffY = abs(Stage::getHeight(topos) - Stage::getHeight(pos)) * chipheight;
			static double jump_height = 1 * chipheight;

			static double q = diffY + jump_height;
			static double p = ( -(chipsize/2)*(2*q-diffY) + chipsize*sqrt(q*(q-diffY)) ) / diffY;
			static double a = q / pow((chipsize/2 - p), 2);

			diffX += mv_mng.dir[mv_mng.current_dir].x*chipsize*mv_mng.moving_rate;

			posY = -a*pow(diffX-p, 2) + q;

			switch(mv_mng.current_dir){
			case mv_mng.NORTH:
			case mv_mng.SOUTH:
				//mv_mng.diff = VAdd(mv_mng.diff,
				//VGet(0.0f, , 0.0f));
				break;
			case mv_mng.EAST:
			case mv_mng.WEST:

				myvec.y = posY;

				DrawFormatString(0, pos.y*20+32, GetColor(255,255,255), "q     = %f", q);
				DrawFormatString(0, pos.y*20+48, GetColor(255,255,255), "p     = %f", p);
				DrawFormatString(0, pos.y*20+64, GetColor(255,255,255), "a     = %f", a);

				break;
			default:
				printfDx("jump direction error.");
				break;
			}
		}

		if(abs(topos.x*chipsize-myvec.z) < 1.0 && abs(topos.y*chipsize-myvec.x) < 1.0){
			pos = topos;
			diffX = chipsize/2;
			mv_mng.diff = VGet(0.0f, 0.0f, 0.0f);
			if(++order == mv_mng.path.size()){
				order = 0;
				can_move = false;
				Stage::disbrighten();
				changeState(state, SELECT);
			}
		}
		break;
	}

	DrawFormatString(0, pos.y*20   , GetColor(255,255,255), "diffX = %f", diffX);
	DrawFormatString(0, pos.y*20+16, GetColor(255,255,255), "posY  = %f", posY);
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
		Cursor::pos = pos;
		if(changeState(state, ACTION)){
			command.back();
		}
	}
	if(Keyboard::pushed(KEY_INPUT_Z)){
		for(auto& enemy : enemies){
			if(Stage::isBrightened(Cursor::pos)){
				if(enemy.pos == Cursor::pos){
					Cursor::pos = pos;
					can_act = false;
					if(changeState(state, SELECT)){
						command.clear();
					}

					int diff = str - enemy.getDef();
					if(diff > 0){
						enemy.setHP(enemy.getHP() - diff);
					}
					break;
				}
			} else {
				Cursor::pos = pos;
				if(changeState(state, ACTION)){
					command.back();
				}
				break;
			}
		}
		Stage::disbrighten();
	}
}

bool Player::assignDirection(){
	if(Keyboard::pushed(KEY_INPUT_UP)){
		MV1SetRotationXYZ(model, VGet(0.0f, DX_PI_F/2, 0.0f));
	} else if(Keyboard::pushed(KEY_INPUT_DOWN)){
		MV1SetRotationXYZ(model, VGet(0.0f, -DX_PI_F/2, 0.0f));
	} else if(Keyboard::pushed(KEY_INPUT_LEFT)){
		MV1SetRotationXYZ(model, VGet(0.0f, 0.0f, 0.0f));
	} else if(Keyboard::pushed(KEY_INPUT_RIGHT)){
		MV1SetRotationXYZ(model, VGet(0.0f, DX_PI_F, 0.0f));
	}

	if(Keyboard::pushed(KEY_INPUT_Z)) return true;
	else return false;
}
