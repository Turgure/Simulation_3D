#pragma once
#include<DxLib.h>

class Camera{
public:
	Camera();
	void update();

	//カメラのポジションセット
	void setX(int x);
	void setY(int y);
	void setZ(int z);
	
	//カメラの注視点をセットする
	void setTargetX(int targetZ);
	void setTargetY(int targetY);
	void setTargetZ(int targetZ);

	//カメラのポジションを返す
	int getX(){return x;}
	int getY(){return y;}
	int getZ(){return  z;}

		//カメラの注視点を返す
	int getTargetX(){return targetX;}
	int getTargetY(){return targetY;}
	int getTargetZ(){return targetZ;}

private:
	int x, y, z;			//カメラの座標(x,y,z)
	int targetX, targetY, targetZ;			//カメラの注視点(targetX, targetY, targetZ)

	VECTOR pos;		// カメラの座標
	VECTOR target;	// カメラの注視点
};