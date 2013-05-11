#include "Camera.h"
#include "GV.h"
#include "Keyboard.h"
#include "Stage.h"
#include "Cursor.h"

int Camera::dir;

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
		dir = ZERO_ZERO;
		is_turning = true;
	}
	if(Keyboard::pushing(KEY_INPUT_Q) ){
		dir = ZERO_MAX;
		is_turning = true;
	}

	if(Keyboard::pushing(KEY_INPUT_S) ){
		dir = MAX_ZERO;
		is_turning = true;
	}
	if(Keyboard::pushing(KEY_INPUT_W) ){
		dir = MAX_MAX;
		is_turning = true;
	}
	

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

	if(Keyboard::pushed(KEY_INPUT_R) ){
		++dir;
		if(dir >= DIR_NUM) dir = MAX_MAX;
		is_turning = true;
	}
	if(Keyboard::pushed(KEY_INPUT_L) ){
		dir += DIR_NUM-1;
		dir %= DIR_NUM;
		is_turning = true;
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
