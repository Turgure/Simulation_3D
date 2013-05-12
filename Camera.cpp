#include "Camera.h"
#include "GV.h"
#include "Keyboard.h"
#include "Stage.h"
#include "Cursor.h"

int Camera::direction[4];

Camera::Camera(){
	// カメラの座標をセット
	/*
	pos.x = Stage::getWidth()*chipsize + 90;
	pos.z = Stage::getDepth()*chipsize + 90;
	target.x = Stage::getWidth()*chipsize-128;
	target.z = Stage::getDepth()*chipsize-128;
	*/
	pos.y = 270;
	target.y = 0;
	cameradir = MAX_MAX;
	setDirection();

	is_turning = false;
	turning_time = 0;
	//ChangeLightTypePoint(VGet( pos.x, pos.y, pos.z ),	10000, 1, 0, 0 ) ;
	//ライトの方向指定
	SetLightDirection( VGet(-0.5, -0.7, -0.5) );
}

void Camera::update(){
	if(isTurning()){
		turning_time++;
		if(turning_time == moveframe){
			turning_time = 0;
			is_turning = false;
		}
		return;
	}
	target.x = Cursor::pos.y * chipsize;
	target.z = Cursor::pos.x * chipsize;
	
	//カメラの位置を移動する
	if(Keyboard::pushing(KEY_INPUT_A) ){
		cameradir = ZERO_ZERO;
		is_turning = true;
	}
	if(Keyboard::pushing(KEY_INPUT_Q) ){
		cameradir = ZERO_MAX;
		is_turning = true;
	}

	if(Keyboard::pushing(KEY_INPUT_S) ){
		cameradir = MAX_ZERO;
		is_turning = true;
	}
	if(Keyboard::pushing(KEY_INPUT_W) ){
		cameradir = MAX_MAX;
		is_turning = true;
	}
	

	//カメラの位置を移動後
	switch(cameradir){
	case MAX_MAX:
		pos.x = target.x +220;
		pos.z = target.z + 220;
		break;
	case MAX_ZERO:
		pos.x = target.x + 220;
		pos.z = target.z -220;
		break;
	case ZERO_MAX:
		pos.x = target.x -220;
		pos.z = target.z +220;
		break;
	case ZERO_ZERO:
		pos.x = target.x-220;
		pos.z = target.z-220;

		//直後のコメントアウト消さないでくれると助かります(tannpo)
		/*
		if(Cursor::pos.y*chipsize-target.x > difffromcameratotarget){
			target.x += chipsize;
			pos.x += chipsize;
		}else if(target.x-Cursor::pos.y*chipsize > difffromcameratotarget){
			target.x -= chipsize;
			pos.x -= chipsize;
		}
		if(Cursor::pos.x*chipsize-target.z > difffromcameratotarget){
			target.z += chipsize;
			pos.z += chipsize;
		}else if(target.z-Cursor::pos.x*chipsize > difffromcameratotarget){
			target.z -= chipsize;
			pos.z -= chipsize;
		}
		*/
		break;
	}

	if(Keyboard::pushed(KEY_INPUT_R) ){
		cameradir = (cameradir + 1) % DIR_NUM;
		is_turning = true;
		setDirection();
	}
	if(Keyboard::pushed(KEY_INPUT_L) ){
		cameradir = (cameradir + (DIR_NUM-1)) % DIR_NUM;
		is_turning = true;
		setDirection();
	}

	//target = VGet(Cursor::pos.y*chipsize, Stage::getHeight(Cursor::pos)*chipheight, Cursor::pos.x*chipsize);

	DrawFormatString(0, 360, GetColor(255,255,255), " CamPos x:%.0f y:%.0f z:%.0f", pos.x, pos.y, pos.z);
	DrawFormatString(0, 380, GetColor(255,255,255), " Target   x:%.0f y:%.0f z:%.0f", target.x, target.y, target.z);
	DrawFormatString(0, 400, GetColor(255,255,255), " Cursor   x:%d y:%d z:%d", Cursor::pos.x, Cursor::pos.y, Stage::getHeight(Cursor::pos));
	//pos = VGet( pos.x, pos.y, pos.z);
	//target = VGet(target.x,target.y,target.z);

	//	平行移動
	//target = VAdd(pos, target);

	// カメラの位置と向きをセットする
	SetCameraPositionAndTarget_UpVecY(pos, target);
}

void Camera::setDirection(){
	switch(cameradir){
	case MAX_MAX:
		direction[LEFT]  = WEST;
		direction[RIGHT] = EAST;
		direction[FRONT] = NORTH;
		direction[BACK]  = SOUTH;
		break;
	case ZERO_MAX:
		direction[LEFT]  = SOUTH;
		direction[RIGHT] = NORTH;
		direction[FRONT] = WEST;
		direction[BACK]  = EAST;
		break;
	case ZERO_ZERO:
		direction[LEFT]  = EAST;
		direction[RIGHT] = WEST;
		direction[FRONT] = SOUTH;
		direction[BACK]  = NORTH;
		break;
	case MAX_ZERO:
		direction[LEFT]  = NORTH;
		direction[RIGHT] = SOUTH;
		direction[FRONT] = EAST;
		direction[BACK]  = WEST;
		break;
	default:
		printfDx("invalid direction");
	}
}
