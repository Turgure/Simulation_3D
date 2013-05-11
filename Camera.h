#pragma once
#include <DxLib.h>

class Camera{
public:
	Camera();
	void update();

	//カメラが移動回転中かどうかを調べる
	bool isTurning(){ return is_turning; }

	//カメラの位置(stageの右手前のマスの配列)
	enum CameraPos{
		MAX_MAX,
		ZERO_MAX,
		ZERO_ZERO,
		MAX_ZERO,
		DIR_NUM
	};
	//カメラの状態を得る
	static int viewfrom;

private:
	bool is_turning;	//カメラが移動回転中かどうか
	int turning_time;
	VECTOR pos;		// カメラの座標
	VECTOR target;	// カメラの注視点

	static const int difffromcameratotarget = 100;
	static const int moveframe = 30;	//カメラの移動や旋回に掛けるフレーム数
};
