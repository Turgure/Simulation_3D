﻿#pragma once
#include <DxLib.h>

class Camera{
public:
	Camera();
	void update();

	//カメラの状態を得る
	enum CameraPos{
		ZERO_ZERO,
		ZERO_MAX,
		MAX_ZERO,
		MAX_MAX
	};
	static CameraPos viewfrom;	//カメラの位置(右下,左下,右上,左上)
	//カメラのポジションセット
	void setX(float x){ pos.x = x; }
	void setY(float y){ pos.y = y; }
	void setZ(float z){ pos.z = z; }
	float getX(){ return pos.x; }
	float getY(){ return pos.y; }
	float getZ(){ return pos.z; }

	//カメラの注視点をセットする
	void setTargetX(float x){ target.x = x; }
	void setTargetY(float y){ target.y = y; }
	void setTargetZ(float z){ target.z = z; }
	float getTargetX(){ return target.x; }
	float getTargetY(){ return target.y; }
	float getTargetZ(){ return target.z; }

private:
	VECTOR pos;		// カメラの座標
	VECTOR target;	// カメラの注視点

	static const int difffromcameratotarget = 100;
};
