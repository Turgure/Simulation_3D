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
	static int getDirection(int dir){ return direction[dir]; }

private:
	void setDirection();

	bool is_turning;	//カメラが移動回転中かどうか
	int turning_time;
	static int direction[4];
	
	//カメラの位置(stageの右手前のマスの配列)
	enum CameraDir{
		MAX_MAX,
		ZERO_MAX,
		ZERO_ZERO,
		MAX_ZERO,
	};
	int cameradir;

	VECTOR pos;		// カメラの座標
	VECTOR target;	// カメラの注視点

	static const int difffromcameratotarget = 100;
	static const int moveframe = 30;	//カメラの移動や旋回に掛けるフレーム数
};
