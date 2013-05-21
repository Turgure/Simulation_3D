#include <DxLib.h>
#include "Object.h"
#include "FileStream.h"
#include "Stage.h"

///BaseObject
bool BaseObject::isMyTurn(){
	if(ATBgauge <= 0){
		ATBgauge = 0;
		return true;
	}
	else return false;
}

void BaseObject::lateUpdate(){
	state_changed = false;
}

bool BaseObject::changeState(State& mystate, State next){
	if(state_changed) return false;

	mystate = next;
	state_changed = true;
	return true;
}


///BaseObjec::Status
void BaseObject::Status::showStatus() const{
	DrawFormatString(160, 16*0, GetColor(255,255,255), "名前 %s", name.c_str());
	DrawFormatString(160, 16*1, GetColor(255,255,255), "HP %d/%d", hp, maxhp);
	DrawFormatString(160, 16*2, GetColor(255,255,255), "MP %d/%d", mp, maxmp);
	DrawFormatString(160, 16*3, GetColor(255,255,255), "攻撃力 %d", str);
	DrawFormatString(160, 16*4, GetColor(255,255,255), "防御力 %d", def);
	DrawFormatString(160, 16*5, GetColor(255,255,255), "素早さ %d", agi);
	DrawFormatString(160, 16*6, GetColor(255,255,255), "移動力 %d", mobility);
	DrawFormatString(160, 16*7, GetColor(255,255,255), "ジャンプ力 %d", jump_power);
}

///BaseObject::MovingManager
void BaseObject::MoveManager::initialize(){
	shortest_path.resize(100);
}

void BaseObject::MoveManager::trackMovement(const Position& pos, const Position& topos, int mob, BaseObject* obj){
	initialize();
	calcShortestPath(pos, topos, mob, obj);
	path = shortest_path;
}

void BaseObject::MoveManager::calcShortestPath(const Position& pos, const Position& topos, int mob, BaseObject* obj){
	if(mob <= 0) return;

	Player* me_player = dynamic_cast<Player*>(obj);
	Enemy* me_enemy = dynamic_cast<Enemy*>(obj);

	Position checkpos;
	for(int i = 0; i < DIR_NUM; ++i){
		checkpos = pos + dir[i];

		if(Stage::isBrightened(checkpos) && (mob-1) >= 0){
			//ジャンプ力の足りる位置から移動
			int height = abs(Stage::getHeight(checkpos) - Stage::getHeight(pos));
			if( (me_player != NULL && me_player->getJumpPow() >= height) ||
				(me_enemy != NULL && me_enemy->getJumpPow() >= height) ){

					current_path.push_back(i);
					if(checkpos == topos){
						if(current_path.size() < shortest_path.size()){
							shortest_path = current_path;
						}
					}
					calcShortestPath(checkpos, topos, mob-1, obj);
					current_path.pop_back();
			}
		}
	}
}

void BaseObject::MoveManager::setObjectDirection(int model, int dir){
	//Z軸の負の向きが始線。回転角は時計回り
	switch(dir){
	case NORTH:
		MV1SetRotationXYZ(model, VGet(0.0f, DX_PI_F/2, 0.0f));
		break;
	case SOUTH:
		MV1SetRotationXYZ(model, VGet(0.0f, -DX_PI_F/2, 0.0f));
		break;
	case WEST:
		MV1SetRotationXYZ(model, VGet(0.0f, 0.0f, 0.0f));
		break;
	case EAST:
		MV1SetRotationXYZ(model, VGet(0.0f, DX_PI_F, 0.0f));
		break;
	}
}

void BaseObject::MoveManager::setObjectDirection(int model, const Position& dirpos){
	if(abs(dirpos.y) >= abs(dirpos.x)){
		if(dirpos.y < 0){
			setObjectDirection(model, NORTH);
		} else{
			setObjectDirection(model, SOUTH);
		}
	} else {
		if(dirpos.x < 0){
			setObjectDirection(model, WEST);
		} else {
			setObjectDirection(model, EAST);
		}
	}
}

void BaseObject::MoveManager::setObjectDirection(int model){
	setObjectDirection(model, current_dir);
}

void BaseObject::MoveManager::initJumpmotion(const Position& pos, const Position& topos){
	if(jump_path != NULL) return;

	step = (Stage::getHeight(topos) - Stage::getHeight(pos)) * chipheight;
	jump = step > 0 ? UP : DOWN;
	step = abs(step);
	jump_dist = step + jump_height*2;
	jump_path = jump_dist;
}


///ObjectManager
void ObjectManager::create(vector<Player> &players, const string& filename){
	vector<vector<string>> status;
	FileStream::load(filename, status);

	//int型に変換
	vector<vector<int>> d;
	for(unsigned int i = 0; i < status.size(); ++i){
		vector<int> inner;
		for(unsigned int j = 1; j < status[i].size(); ++j){
			inner.push_back( stoi(status[i][j]) );
		}
		d.push_back(inner);

		players.push_back( Player(status[i][0], d[i][0], d[i][1], d[i][2], d[i][3], d[i][4], d[i][5], d[i][6], d[i][7], d[i][8]) );
	}

}

void ObjectManager::create(vector<Enemy> &enemies, const string& filename){
	vector<vector<string>> status;
	FileStream::load(filename, status);

	Position finalpos;

	//int型に変換
	vector<vector<int>> d;
	for(unsigned int i = 0; i < status.size(); ++i){
		vector<int> inner;
		for(unsigned int j = 1; j < status[i].size(); ++j){
			inner.push_back( stoi(status[i][j]) );
		}
		d.push_back(inner);

		do{
			finalpos.x = GetRand(Stage::getWidth());
			finalpos.y = GetRand(Stage::getDepth());
		}while(!Stage::canMove(finalpos));

		enemies.push_back( Enemy(status[i][0], finalpos.x, finalpos.y, d[i][0], d[i][1], d[i][2], d[i][3], d[i][4], d[i][5], d[i][6]) );
	}
}
