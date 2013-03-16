#include "Camera.h"

Camera::Camera(){
	// カメラの注視点をセット
	target = VGet(  -1.0f, -1.0f,  -1.0f );
}

void Camera::update(){
	// カメラの座標をセット
	pos = VGet(500, 100, 100) ;
	static int i = 1;
	//target = VAdd(pos, target) ;
	
	// カメラの位置と向きをセットする
	SetCameraPositionAndTarget_UpVecY(pos, target) ;
}
