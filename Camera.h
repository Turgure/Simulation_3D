#pragma once
#include<DxLib.h>

class Camera{
public:
	Camera();
	void update();

	//カメラのポジションセット
	void setX(int x){ pos.x = x; }
	void setY(int y){ pos.y = y; }
	void setZ(int z){ pos.z = z; }
	
	//カメラの注視点をセットする
	void setTargetX(int x){ target.x = x; }
	void setTargetY(int y){ target.y = y; }
	void setTargetZ(int z){ target.z = z; }

	//カメラのポジションを返す
	float getX(){ return pos.x; }
	float getY(){ return pos.y; }
	float getZ(){ return pos.z; }

		//カメラの注視点を返す
	float getTargetX(){ return target.x; }
	float getTargetY(){ return target.y; }
	float getTargetZ(){ return target.z; }

private:
	VECTOR pos;		// カメラの座標
	VECTOR target;	// カメラの注視点
};