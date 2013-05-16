#pragma once
#include <DxLib.h>

class Camera{
public:
	Camera();
	void update();

	//カメラが移動回転中かどうかを調べる
	bool isTurning(){ return is_turning; }
	//カメラ方向に応じた,
	//FRONT, BACK, LEFT, RIGHTの方向を得る
	static int getDirection(int dir){ return stage_dir[dir]; }

private:
	void setDirection();
	void turn();

	bool is_turning;	//カメラが移動回転中かどうか
	int turning_time;
	static int stage_dir[4];
	
	//カメラの位置(stageの右手前のマスの配列)
	enum Direction{
		MAX_MAX,
		ZERO_MAX,
		ZERO_ZERO,
		MAX_ZERO,
	};
	int dir;

	enum Rotation{CLOCKWISE, COUNTERCLOCKWISE};
	int rotation;

	VECTOR pos;		// カメラの座標
	VECTOR target;	// カメラの注視点

//	static const int difffromcameratotarget = 100;
	static const int moveframe = 30;	//カメラの移動や旋回に掛けるフレーム数
};
