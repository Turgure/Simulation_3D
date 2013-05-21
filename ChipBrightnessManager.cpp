#include "DxLib.h"
#include "ChipBrightnessManager.h"
#include "GV.h"
#include "Object.h"
#include "Stage.h"

Position ChipBrightnessManager::dir[8] = {
	Position( 0, -1),	//up
	Position( 1, -1),	//rigthup
	Position( 1,  0),	//right
	Position( 1,  1),	//rightbottom
	Position( 0,  1),	//bottom
	Position(-1,  1),	//leftbottom
	Position(-1,  0),	//left
	Position(-1, -1)	//leftup
};
int ChipBrightnessManager::color_move = GetColor(102,255,255);
int ChipBrightnessManager::color_attack = GetColor(255,102,255);
int ChipBrightnessManager::color_support = GetColor(102,255,102);

void ChipBrightnessManager::range(const Position& pos, int n, bool is_resistance, BaseObject* obj){
	if(n <= 0) return;

	Player* me_player = dynamic_cast<Player*>(obj);
	Enemy* me_enemy = dynamic_cast<Enemy*>(obj);

	for(int i = 0; i < 4; ++i){
		Position topos = pos + dir[i*2];
		if(Stage::canMove(topos)){
			//ジャンプ力が足りなかったらreturn
			int height = abs(Stage::getHeight(topos) - Stage::getHeight(pos));
			if( (me_player != NULL && me_player->getJumpPow() >= height) ||
				(me_enemy != NULL && me_enemy->getJumpPow() >= height) ){

					int rest = is_resistance ? n - Stage::getResistance(topos) : n - 1;
					if(rest >= 0){
						Stage::brighten(topos, color_move);
						//敵をすり抜けないようにする
						Player* you_player = dynamic_cast<Player*>(Stage::getObjectAt(topos));
						Enemy* you_enemy = dynamic_cast<Enemy*>(Stage::getObjectAt(topos));

						if(me_player != NULL){
							if(me_player == you_player || you_enemy == NULL){
								range(topos, rest, is_resistance, obj);
							}
							continue;
						}
						if(me_enemy != NULL){
							if(me_enemy == you_enemy || you_player == NULL){
								range(topos, rest, is_resistance, obj);
							}
							continue;
						}
					}
			}
		}
	}
}

void ChipBrightnessManager::reachAt(const Position& pos, int color, int n){
	Position topos;

	for(int m = 0; m < n; ++m){
		topos.set(pos.x -n+m, pos.y -m);
		if(Stage::canMove(topos)){
			Stage::brighten(topos, color);
		}
		topos.set(pos.x +m, pos.y -n+m);
		if(Stage::canMove(topos)){
			Stage::brighten(topos, color);
		}
		topos.set(pos.x +n-m, pos.y +m);
		if(Stage::canMove(topos)){
			Stage::brighten(topos, color);
		}
		topos.set(pos.x -m, pos.y +n-m);
		if(Stage::canMove(topos)){
			Stage::brighten(topos, color);
		}
	}
}

void ChipBrightnessManager::reachTo(const Position& pos, int color, int min_range, int max_range){
	for(int i = min_range; i <= max_range; ++i){
		reachAt(pos, color, i);
	}
}

//void ChipBrightnessManager::around8(const Position& pos, int color){
//	for(int i = 0; i < 8; ++i){
//		if(Stage::canMove(x + dir[i][0], y + dir[i][1])){
//			Stage::brighten(x + dir[i][0], y + dir[i][1], color);
//		}
//	}
//}

//void ChipBrightnessManager::aroundTo(const Position& pos, int color, int n){
//	if(n <= 0) return;
//
//	for(int i = 0; i < n; ++i){
//		for(int j = 0; j < 8; ++j){
//			around8(x + dir[j][0]*i, y + dir[j][1]*i, color);
//			aroundTo(x + dir[j][0], y + dir[j][1], color, n-1);
//		}
//	}
//
//	//for(int i = 0; i < n; ++i){
//	//	for(int j = 0; j < 8; ++j){
//	//		around8(x + dir[j][0]*i, y + dir[j][1]*i, color);
//	//	}
//	//}
//}
//
//void ChipBrightnessManager::rowFourTo(const Position& pos, int color, int n){
//	if(n <= 0) return;
//
//	for(int i = 1; i <= n; ++i){
//		for(int j = 0; j < 4; ++j){
//			if(Stage::canMove(x + dir[j*2][0]*i, y + dir[j*2][1]*i)){
//				Stage::brighten(x + dir[j*2][0]*i, y + dir[j*2][1]*i, color);
//			}
//		}
//	}
//}
//
//void ChipBrightnessManager::rowFour(const Position& pos, int color){
//	rowFourTo(x, y, color, Stage::getWidth() >= Stage::getDepth() ? Stage::getWidth() : Stage::getDepth());
//}
