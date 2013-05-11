#include "Object.h"
#include "Stage.h"
#include "Cursor.h"
#include "ChipBrightnessManager.h"

Enemy::Enemy(int x, int y, string name, int hp, int mp, int str, int def, int agi, int mobility):
	pos(x, y),
	move_pos(),
	act_pos(){
		model = MV1LoadModel("data/image/3Dmodel/chara/miku.pmd");
		MV1SetScale(model, VGet(3.0f, 3.0f, 3.0f));	//拡大
		MV1SetRotationXYZ(model, VGet(0.0f, -90 * DX_PI_F/180.0f, 0.0f));	//向き
		mv_mng.current_dir = mv_mng.SOUTH;

		this->name = name;
		this->hp = maxhp = hp;
		this->mp = maxmp = mp;
		this->str = str;
		this->def = def;
		this->agi = agi;
		this->mobility = mobility;
		state = SELECT;
		ATBgauge = 100;
		can_move = false;
		can_act = false;
		moved = false;
		attacked = false;
		wait_time = 0;
		attack_range = 3;
}

void Enemy::update(){
	myvec = VAdd(VGet(pos.y*chipsize, Stage::getHeight(pos)*chipheight, pos.x*chipsize), mv_mng.diff);
	//3Dモデルの配置
	MV1SetPosition(model, VAdd(myvec, VGet(chipsize/2, 0, chipsize/2)));
	Stage::setObjectAt(pos, this);
}

void Enemy::draw(){
	// ３Ｄモデルの描画
	MV1DrawModel(model);

	if(pos == Cursor::pos){
		showStatus(200, 0);
	}

	switch(state){
	case MOVE:
		ChipBrightnessManager::range(pos, mobility, true, this);
		break;
	case ACTION:
		if(can_act)
			ChipBrightnessManager::reachTo(pos, ChipBrightnessManager::getColorAttack(), 1, attack_range);
		break;
	default:
		break;
	}
}

void Enemy::action(){
	DrawFormatString(0, 0, GetColor(255,255,255), "%s's turn.", name.c_str());

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
		changeState(state, WAIT);
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
			if(isCountOver(30)){
				mv_mng.trackMovement(pos, Cursor::pos, mobility);
				changeState(state, MOVING);
			}
		}
		break;

	case MOVING:
		static int order;
		static Position topos;
		mv_mng.current_dir = mv_mng.path[order];
		topos = pos + mv_mng.dir[mv_mng.current_dir];
		mv_mng.setObjectDirection(model);

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
				moved = true;
				Stage::disbrighten();
				changeState(state, SELECT);
			}
		}
		break;
	}
}

void Enemy::endMyTurn(){
	changeState(state, SELECT);
	ATBgauge =  100;
	wait_time = 0;
	//if(moved) ATBgauge += 20;
	//if(attacked) ATBgauge += 60;
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

	Position finalpos(-1, -1);
	for(int y = 0; y < Stage::getDepth(); ++y){
		for(int x = 0; x < Stage::getWidth(); ++x){
			Position checkpos(x, y);
			if(!Stage::isBrightened(checkpos)) continue;

			for(auto& player : players){
				if(checkpos == player.pos){
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
	changeState(state, SELECT);

	if(Stage::isBrightened(Cursor::pos)){
		for(auto& player : players){
			int diff = str - player.getDef();
			if(diff <= 0) continue;

			if(player.pos == Cursor::pos){
				player.setHP(player.getHP() - diff);
				break;
			}
		}
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

	Position dirpos = pos - finalpos;
	if(abs(dirpos.y) >= abs(dirpos.x) && dirpos.y > 0){
		//north
		MV1SetRotationXYZ(model, VGet(0.0f, DX_PI_F/2, 0.0f));
	} else if(abs(dirpos.y) >= abs(dirpos.x) && dirpos.y < 0){
		//south
		MV1SetRotationXYZ(model, VGet(0.0f, -DX_PI_F/2, 0.0f));
	} else if(abs(dirpos.y) <= abs(dirpos.x) && dirpos.x > 0){
		//west
		MV1SetRotationXYZ(model, VGet(0.0f, 0.0f, 0.0f));
	} else if(abs(dirpos.y) <= abs(dirpos.x) && dirpos.x > 0){
		//east
		MV1SetRotationXYZ(model, VGet(0.0f, DX_PI_F, 0.0f));
	}
}
