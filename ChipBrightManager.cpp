#include <DxLib.h>
#include "ChipBrightManager.h"
#include "GV.h"
#include "Stage.h"

int ChipBrightManager::dir[8][2] = {
	{ 0, -1},	//up
	{ 1, -1},	//rigthup
	{ 1,  0},	//right
	{ 1,  1},	//rightbottom
	{ 0,  1},	//bottom
	{-1,  1},	//leftbottom
	{-1,  0},	//left
	{-1, -1}	//leftup
};
int ChipBrightManager::color_move = GetColor(102,255,255);
int ChipBrightManager::color_attack = GetColor(255,102,255);
int ChipBrightManager::color_support = GetColor(102,255,102);

void ChipBrightManager::range(int x, int y, int n, bool consider_resistance){
	if(n <= 0) return;

	int toX, toY;
	int rest[4];
	for(int i = 0; i < 4; ++i){
		toX = x + dir[i*2][0];
		toY = y + dir[i*2][1];
		rest[i] = consider_resistance ? n - Stage::getResistance(toX, toY) : n - 1;

		if(Stage::canMove(toX, toY) && rest[i] >= 0){
			Stage::brighten(toX, toY, color_move);
			//敵をすり抜けないようにする
			if(Stage::getObjectAt(toX, toY)) continue;
			range(toX, toY, rest[i], consider_resistance);
		}
	}
}

void ChipBrightManager::reachAt(int x, int y, int color, int n){
	for(int m = 0; m < n; ++m){
		if(Stage::canMove(x-n+m, y-m)){
			Stage::brighten(x-n+m, y-m, color);
		}
		if(Stage::canMove(x+m, y-n+m)){
			Stage::brighten(x+m, y-n+m, color);
		}
		if(Stage::canMove(x+n-m, y+m)){
			Stage::brighten(x+n-m, y+m, color);
		}
		if(Stage::canMove(x-m, y+n-m)){
			Stage::brighten(x-m, y+n-m, color);
		}
	}
}

void ChipBrightManager::reachTo(int x, int y, int color, int min_range, int max_range){
	for(int i = min_range; i <= max_range; ++i){
		reachAt(x, y, color, i);
	}
}

void ChipBrightManager::around8(int x, int y, int color){
	for(int i = 0; i < 8; ++i){
		if(Stage::canMove(x + dir[i][0], y + dir[i][1])){
			Stage::brighten(x + dir[i][0], y + dir[i][1], color);
		}
	}
}

void ChipBrightManager::aroundTo(int x, int y, int color, int n){
	if(n <= 0) return;

	for(int i = 0; i < n; ++i){
		for(int j = 0; j < 8; ++j){
			around8(x + dir[j][0]*i, y + dir[j][1]*i, color);
			aroundTo(x + dir[j][0], y + dir[j][1], color, n-1);
		}
	}

	//for(int i = 0; i < n; ++i){
	//	for(int j = 0; j < 8; ++j){
	//		around8(x + dir[j][0]*i, y + dir[j][1]*i, color);
	//	}
	//}
}

void ChipBrightManager::rowFourTo(int x, int y, int color, int n){
	if(n <= 0) return;

	for(int i = 1; i <= n; ++i){
		for(int j = 0; j < 4; ++j){
			if(Stage::canMove(x + dir[j*2][0]*i, y + dir[j*2][1]*i)){
				Stage::brighten(x + dir[j*2][0]*i, y + dir[j*2][1]*i, color);
			}
		}
	}
}

void ChipBrightManager::rowFour(int x, int y, int color){
	rowFourTo(x, y, color, Stage::getWidth() >= Stage::getDepth() ? Stage::getWidth() : Stage::getDepth());
}
