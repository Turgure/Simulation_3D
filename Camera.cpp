#include <cmath>
#include "Camera.h"
#include "GV.h"
#include "Keyboard.h"
#include "Stage.h"
#include "Cursor.h"

int Camera::stage_dir[4];

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
	dir = MAX_MAX;
	setDirection();

	is_turning = false;
	turning_time = 0;
	//ChangeLightTypePoint(VGet( pos.x, pos.y, pos.z ),	10000, 1, 0, 0 ) ;
	//ライトの方向指定
	SetLightDirection( VGet(-0.5, -0.7, -0.5) );
}

void Camera::update(){
	if(is_turning){
		turn();
		return;
	}
	target.x = Cursor::pos.y * chipsize+chipsize/2;
	target.z = Cursor::pos.x * chipsize+chipsize/2;
	target.y = Stage::getHeight(Cursor::pos)*chipheight;
	//カメラの位置を移動後
	switch(dir){
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

	if(Keyboard::pushed(KEY_INPUT_Q) ){
		rotation = CLOCKWISE;
		dir = (dir + 1) % DIR_NUM;
		is_turning = true;
		setDirection();
	}
	if(Keyboard::pushed(KEY_INPUT_W) ){
		rotation = COUNTERCLOCKWISE;
		dir = (dir + (DIR_NUM-1)) % DIR_NUM;
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
	switch(dir){
	case MAX_MAX:
		stage_dir[LEFT]  = WEST;
		stage_dir[RIGHT] = EAST;
		stage_dir[FRONT] = NORTH;
		stage_dir[BACK]  = SOUTH;
		break;
	case ZERO_MAX:
		stage_dir[LEFT]  = SOUTH;
		stage_dir[RIGHT] = NORTH;
		stage_dir[FRONT] = WEST;
		stage_dir[BACK]  = EAST;
		break;
	case ZERO_ZERO:
		stage_dir[LEFT]  = EAST;
		stage_dir[RIGHT] = WEST;
		stage_dir[FRONT] = SOUTH;
		stage_dir[BACK]  = NORTH;
		break;
	case MAX_ZERO:
		stage_dir[LEFT]  = NORTH;
		stage_dir[RIGHT] = SOUTH;
		stage_dir[FRONT] = EAST;
		stage_dir[BACK]  = WEST;
		break;
	default:
		printfDx("invalid stage_dir");
	}
}

void Camera::turn(){
	turning_time++;

	//時計回り
	switch(rotation){
	case CLOCKWISE:
		//移動後のポジション
		switch(dir){
		case ZERO_MAX:
			pos.x = target.x+sqrt(2)*220*sin(DX_PI_F/4-DX_PI_F/2*turning_time/moveframe);
			pos.z = target.z+sqrt(2)*220*cos(DX_PI_F/4-DX_PI_F/2*turning_time/moveframe);
			break;
		case ZERO_ZERO:
			pos.x = target.x+sqrt(2)*220*sin(DX_PI_F*7/4-DX_PI_F/2*turning_time/moveframe);
			pos.z = target.z+sqrt(2)*220*cos(DX_PI_F*7/4-DX_PI_F/2*turning_time/moveframe);
			break;
		case MAX_ZERO:
			pos.x = target.x+sqrt(2)*220*sin(DX_PI_F*5/4-DX_PI_F/2*turning_time/moveframe);
			pos.z = target.z+sqrt(2)*220*cos(DX_PI_F*5/4-DX_PI_F/2*turning_time/moveframe);
			break;
		case MAX_MAX:
			pos.x = target.x+sqrt(2)*220*sin(DX_PI_F*3/4-DX_PI_F/2*turning_time/moveframe);
			pos.z = target.z+sqrt(2)*220*cos(DX_PI_F*3/4-DX_PI_F/2*turning_time/moveframe);
			break;
		}
		break;
		//反時計回り
	case COUNTERCLOCKWISE:
		switch(dir){
		case ZERO_MAX:
			pos.x = target.x+sqrt(2)*220*sin(DX_PI_F*5/4+DX_PI_F/2*turning_time/moveframe);
			pos.z = target.z+sqrt(2)*220*cos(DX_PI_F*5/4+DX_PI_F/2*turning_time/moveframe);
			break;
		case ZERO_ZERO:
			pos.x = target.x+sqrt(2)*220*sin(DX_PI_F*3/4+DX_PI_F/2*turning_time/moveframe);
			pos.z = target.z+sqrt(2)*220*cos(DX_PI_F*3/4+DX_PI_F/2*turning_time/moveframe);
			break;
		case MAX_ZERO:
			pos.x = target.x+sqrt(2)*220*sin(DX_PI_F/4+DX_PI_F/2*turning_time/moveframe);
			pos.z = target.z+sqrt(2)*220*cos(DX_PI_F/4+DX_PI_F/2*turning_time/moveframe);
			break;
		case MAX_MAX:
			pos.x = target.x+sqrt(2)*220*sin(DX_PI_F*7/4+DX_PI_F/2*turning_time/moveframe);
			pos.z = target.z+sqrt(2)*220*cos(DX_PI_F*7/4+DX_PI_F/2*turning_time/moveframe);
			break;
		}
		break;
	}
	// カメラの位置と向きをセットする
	SetCameraPositionAndTarget_UpVecY(pos, target);
	if(turning_time >= moveframe){
		turning_time = 0;
		is_turning = false;
	}
}
