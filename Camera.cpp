#include "Camera.h"
#include "Keyboard.h"
Camera::Camera(){

	// カメラの注視点をセット
	//pos = VGet( -70, 115, 457);
	//target = VGet(45,81,314);
}

void Camera::update(){
	
	
	//カメラテスト用です
	
	static int x = 0;
	if(Keyboard::pushing(KEY_INPUT_Z) ){
		x++;
	}
	if(Keyboard::pushing(KEY_INPUT_A) ){
		x--;
	}
	static int y = 0;
	if(Keyboard::pushing(KEY_INPUT_X) ){
		y++;
	}
	if(Keyboard::pushing(KEY_INPUT_S) ){
		y--;
	}
	static int z = 1;
	if(Keyboard::pushing(KEY_INPUT_C) ){
		z++;
	}
	if(Keyboard::pushing(KEY_INPUT_D) ){
		z--;
	}

		static int a = 0;
	if(Keyboard::pushing(KEY_INPUT_V) ){
		a++;
	}
	if(Keyboard::pushing(KEY_INPUT_F) ){
		a--;
	}
	static int b = 0;
	if(Keyboard::pushing(KEY_INPUT_B) ){
		b++;
	}
	if(Keyboard::pushing(KEY_INPUT_G) ){
		b--;
	}
	static int c = 0;
	if(Keyboard::pushing(KEY_INPUT_N) ){
		c++;
	}
	if(Keyboard::pushing(KEY_INPUT_H) ){
		c--;
	}
	DrawFormatString(0, 0, GetColor(255,255,255), " x:%d y:%d z:%d", x, y, z);
	DrawFormatString(0, 20, GetColor(255,255,255), " a:%d b:%d c:%d", a, b, c);
	pos = VGet( x, y, z);
	target = VGet(a,b,c);
	

	// カメラの座標をセット
	//pos = VGet( -70, 115, 457);	//仮
	//target = VGet(45,81,314);		//仮
	//pos = VGet(  11, 166, 522);
	//target = VGet(234,-6, -99);
	//target = VAdd(pos, target);
	
	// カメラの位置と向きをセットする
	SetCameraPositionAndTarget_UpVecY(pos, target);
}
