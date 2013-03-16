#pragma once
#include<DxLib.h>

class Camera{
public:
	Camera();
	void update();

private:
	VECTOR pos;		// カメラの座標
	VECTOR target;	// カメラの注視点
};