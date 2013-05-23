#pragma once
#include "Position.h"

class BaseObject;
class ChipBrightnessManager{
public:
	static int getColorAttack(){ return color_attack; }
	static int getColorSupport(){ return color_support; }
	
	///移動用
	//(x, y)中心でnマス範囲
	static void range(const Position& pos, int n, bool is_resistance, BaseObject* obj);
	///行動用
	//(x, y)中心でnマス先のマス
	static void reachAt(const Position& pos, int color, int n);
	//(x, y)中心でmin_rangeからmax_rangeまでのマス
	static void reachTo(const Position& pos, int color, int min_range, int max_range);
	//(x, y)の周囲8マス
	static void around8(const Position& pos, int color);
	//(x, y)中心の四角範囲
	static void aroundTo(const Position& pos, int color, int n);
	//(x, y)中心の前後左右4列(nマス先まで)
	static void rowFourTo(const Position& pos, int color, int n);
	//(x, y)中心の前後左右4列
	static void rowFour(const Position& pos, int color);

private:
	static Position dir[8];	//12時起点の時計回り
	static int color_move;	
	static int color_attack;
	static int color_support;
};