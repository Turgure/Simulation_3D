#include "Camera.h"
#include "GV.h"
#include "Keyboard.h"
#include "Stage.h"
#include "Cursor.h"

Camera::Camera(){
	// カメラの座標をセット
	pos.x = Stage::getWidth()*chipsize + 90;
	pos.y = 270;
	pos.z = Stage::getDepth()*chipsize + 90;
	target.x = Stage::getWidth()*chipsize/2;
	target.y = 0;
	target.z = Stage::getDepth()*chipsize/2;

	viewfrom = ZERO_ZERO;

	//ChangeLightTypePoint(VGet( pos.x, pos.y, pos.z ),	10000, 1, 0, 0 ) ;
	//ライトの方向指定
	SetLightDirection( VGet(-0.5, -0.7, -0.5) );
}

void Camera::update(){
	//カメラテスト用です

	//カメラの位置を移動
	if(Keyboard::pushing(KEY_INPUT_A) ){
		pos.x = -90;
		pos.z = -90;
		target.x = Stage::getWidth()*chipsize/2;
		target.z = Stage::getDepth()*chipsize/2;
		viewfrom = ZERO_ZERO;
	}
	if(Keyboard::pushing(KEY_INPUT_Q) ){
		pos.x = Stage::getWidth()*chipsize+90;
		pos.z = -90;
		target.x = Stage::getWidth()*chipsize/2;
		target.z = Stage::getDepth()*chipsize/2;
		viewfrom = ZERO_MAX;
	}

	if(Keyboard::pushing(KEY_INPUT_S) ){
		pos.x = -90;
		pos.z = Stage::getDepth()*chipsize+90;
		target.x = Stage::getWidth()*chipheight/2;
		target.z = Stage::getDepth()*chipheight/2;
		viewfrom = MAX_ZERO;
	}
	if(Keyboard::pushing(KEY_INPUT_W) ){
		pos.x = Stage::getWidth()*chipsize+90;
		pos.z = Stage::getDepth()*chipsize+90;
		target.x = Stage::getWidth()*chipsize/2;
		target.z = Stage::getDepth()*chipsize/2;
		viewfrom = MAX_MAX;
	}
	//カメラの位置を移動
	switch(viewfrom){
	case MAX_MAX:
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
		break;
	case MAX_ZERO:
		if(Cursor::pos.y*chipsize-target.x > difffromcameratotarget){
			target.x += chipsize;
			pos.x += chipsize;
		}else if(target.x-Cursor::pos.y*chipsize > difffromcameratotarget){
			target.x -= chipsize;
			pos.x -= chipsize;
		}
		if((Stage::getWidth()-Cursor::pos.x)*chipsize-target.z > difffromcameratotarget){
			target.z += chipsize;
			pos.z += chipsize;
		}else if(target.z-(Stage::getDepth()-Cursor::pos.x)*chipsize > difffromcameratotarget){
			target.z -= chipsize;
			pos.z -= chipsize;
		}
		DrawFormatString(0, 420, GetColor(255,255,255), "x:%.0f",(Stage::getWidth()-Cursor::pos.x)*chipsize-target.z );
		break;
	case ZERO_MAX:
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
		break;
	case ZERO_ZERO:
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
		break;
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
