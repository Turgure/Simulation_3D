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
void BaseObject::Status::showStatus(int x, int y) const{
	DrawFormatString(x, y+ 0, GetColor(255,255,255), "id  %d", id);
	DrawFormatString(x, y+16, GetColor(255,255,255), "hp  %d/%d", hp, maxhp);
	DrawFormatString(x, y+32, GetColor(255,255,255), "mp  %d/%d", mp, maxmp);
	DrawFormatString(x, y+48, GetColor(255,255,255), "str %d", str);
	DrawFormatString(x, y+64, GetColor(255,255,255), "def %d", def);
	DrawFormatString(x, y+80, GetColor(255,255,255), "agi %d", agi);
	DrawFormatString(x, y+94, GetColor(255,255,255), "mob %d", mobility);
}


///BaseObject::MovingManager
void BaseObject::MovingManager::trackMovement(const Position& pos, const Position& topos, int mob){
	initialize();
	calcShortestPath(pos, topos, mob);
	path = shortest_path;
}

void BaseObject::MovingManager::initialize(){
	shortest_path.resize(100);
}

void BaseObject::MovingManager::calcShortestPath(const Position& pos, const Position& topos, int mob){
	if(mob <= 0) return;

	Position checkpos;
	for(int i = 0; i < DIR_NUM; ++i){
		checkpos = pos + dir[i];

		if(Stage::isBrightened(checkpos) && (mob-1) >= 0){
			current_path.push_back(i);
			if(checkpos == topos){
				if(current_path.size() < shortest_path.size()){
					shortest_path = current_path;
				}
			}
			calcShortestPath(checkpos, topos, mob-1);
			current_path.pop_back();
		}
	}
}


void BaseObject::MovingManager::setObjectDirection(int MHandle){
	//Z軸の負の向きが始線。回転角は時計回り
	switch(current_dir){
	case NORTH:
		MV1SetRotationXYZ(MHandle, VGet(0.0f, DX_PI_F/2, 0.0f));
		break;
	case SOUTH:
		MV1SetRotationXYZ(MHandle, VGet(0.0f, -DX_PI_F/2, 0.0f));
		break;
	case WEST:
		MV1SetRotationXYZ(MHandle, VGet(0.0f, 0.0f, 0.0f));
		break;
	case EAST:
		MV1SetRotationXYZ(MHandle, VGet(0.0f, DX_PI_F, 0.0f));
		break;
	}
}


///ObjectManager
void ObjectManager::create(vector<Player> &players, const string& filename, int x, int y){
	vector<string> status;
	FileStream::load(filename, status);

	//int型に変換
	vector<int> d;
	for(auto& s : status){
		d.push_back(stoi(s));
	}
	players.push_back( Player(x, y, d[0], d[1], d[2], d[3], d[4], d[5], d[6]) );
}

void ObjectManager::create(vector<Enemy> &enemies, const string& filename){
	vector<vector<string>> status;
	FileStream::load(filename, status);

	Position finalpos;

	//int型に変換
	int d[256][256];
	for(unsigned int i = 0; i < status.size(); ++i){
		for(unsigned int j = 0; j < status[i].size(); ++j){
			d[i][j] = stoi(status[i][j]);
		}

		do{
			finalpos.x = GetRand(Stage::getWidth());
			finalpos.y = GetRand(Stage::getDepth());
		}while(!Stage::canMove(finalpos));
		enemies.push_back( Enemy(finalpos.x, finalpos.y, d[i][0], d[i][1], d[i][2], d[i][3], d[i][4], d[i][5], d[i][6]) );
	}
}