#include "Camera.h"
#include "Keyboard.h"
Camera::Camera(){

	// カメラの座標をセット
	x = 470;
	y = 290;
	z = 330;
	// カメラの注視点をセット
	targetX = 190;
	targetY = -56;
	targetZ = 70;
}

void Camera::update(){

	//カメラテスト用です
	//カメラの位置を移動
	if(Keyboard::pushing(KEY_INPUT_A) ){
		x += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_Q) ){
		x -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_S) ){
		y += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_W) ){
		y -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_D) ){
		z += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_E) ){
		z -= 3;
	}

	//カメラの注視点のみ移動
	if(Keyboard::pushing(KEY_INPUT_F) ){
		targetX += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_R) ){
		targetX -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_G) ){
		targetY += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_T) ){
		targetY -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_H) ){
		targetZ += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_Y) ){
		targetZ -= 3;
	}

	//カメラを平行移動
	if(Keyboard::pushing(KEY_INPUT_1) ){
		x += 3;
		targetX +=3;
	}
	if(Keyboard::pushing(KEY_INPUT_2) ){
		x -= 3;
		targetX -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_3) ){
		y += 3;
		targetY += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_4) ){
		y -= 3;
		targetY -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_5) ){
		z += 3;
		targetZ += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_6) ){
		z -= 3;
		targetZ -= 3;
	}
	DrawFormatString(0, 60, GetColor(255,255,255), " x:%d y:%d z:%d", x, y, z);
	DrawFormatString(0, 80, GetColor(255,255,255), " a:%d b:%d c:%d", targetX, targetY, targetZ);
	pos = VGet( x, y, z);
	target = VGet(targetX,targetY,targetZ);

	//	平行移動
	//target = VAdd(pos, target);


	// カメラの位置と向きをセットする
	SetCameraPositionAndTarget_UpVecY(pos, target);
}
