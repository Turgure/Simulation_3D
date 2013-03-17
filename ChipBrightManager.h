#pragma once
//#include "Position.h"

class ChipBrightManager{
public:
	static int GetColorAttack(){ return color_attack; }
	static int GetColorSupport(){ return color_support; }
	
	///移動用
	//(x, y)中心でnマス範囲
	static void range(int x, int y, int n, bool consider_resistance);
	///行動用
	//(x, y)中心でnマス先のマス
	static void reachAt(int x, int y, int color, int n);
	//(x, y)中心でmin_rangeからmax_rangeまでのマス
	static void reachTo(int x, int y, int color, int min_range, int max_range);
	//(x, y)の周囲8マス
	static void around8(int x, int y, int color);
	//(x, y)中心の四角範囲
	static void aroundTo(int x, int y, int color, int n);
	//(x, y)中心の前後左右4列(nマス先まで)
	static void rowFourTo(int x, int y, int color, int n);
	//(x, y)中心の前後左右4列
	static void rowFour(int x, int y, int color);

private:
	static int dir[8][2];	//12時起点の時計回り
	static int color_move;	
	static int color_attack;
	static int color_support;
};