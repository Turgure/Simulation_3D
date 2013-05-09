#pragma once
#include<DxLib.h>

enum CameraPos{
	DOWNRIGHT,
	DOWNLEFT,
	UPRIGHT,
	UPLEFT
};


class Camera{
public:
	Camera();
	void update();

	//カメラのポジションセット
	void setX(float x){ pos.x = x; }
	void setY(float y){ pos.y = y; }
	void setZ(float z){ pos.z = z; }
	
	//カメラの注視点をセットする
	void setTargetX(float x){ target.x = x; }
	void setTargetY(float y){ target.y = y; }
	void setTargetZ(float z){ target.z = z; }

	//カメラのポジションを返す
	float getX(){ return pos.x; }
	float getY(){ return pos.y; }
	float getZ(){ return pos.z; }

		//カメラの注視点を返す
	float getTargetX(){ return target.x; }
	float getTargetY(){ return target.y; }
	float getTargetZ(){ return target.z; }


private:
	CameraPos viewfrom;	//カメラの位置(右下,左下,右上,左上)
	VECTOR pos;		// カメラの座標
	VECTOR target;	// カメラの注視点
};