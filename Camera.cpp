#include "Camera.h"
#include "Keyboard.h"
Camera::Camera(){

	// �J�����̍��W���Z�b�g
	pos = VGet(  476, 289, 331);
	// �J�����̒����_���Z�b�g
	target = VGet(187,-56, 70);
}

void Camera::update(){


	//�J�����e�X�g�p�ł�
	
	//static int x = 0;
	//if(Keyboard::pushing(KEY_INPUT_Z) ){
	//	x++;
	//}
	//if(Keyboard::pushing(KEY_INPUT_A) ){
	//	x--;
	//}
	//static int y = 0;
	//if(Keyboard::pushing(KEY_INPUT_X) ){
	//	y++;
	//}
	//if(Keyboard::pushing(KEY_INPUT_S) ){
	//	y--;
	//}
	//static int z = 1;
	//if(Keyboard::pushing(KEY_INPUT_C) ){
	//	z++;
	//}
	//if(Keyboard::pushing(KEY_INPUT_D) ){
	//	z--;
	//}

	//static int a = 0;
	//if(Keyboard::pushing(KEY_INPUT_V) ){
	//	a++;
	//}
	//if(Keyboard::pushing(KEY_INPUT_F) ){
	//	a--;
	//}
	//static int b = 0;
	//if(Keyboard::pushing(KEY_INPUT_B) ){
	//	b++;
	//}
	//if(Keyboard::pushing(KEY_INPUT_G) ){
	//	b--;
	//}
	//static int c = 0;
	//if(Keyboard::pushing(KEY_INPUT_N) ){
	//	c++;
	//}
	//if(Keyboard::pushing(KEY_INPUT_H) ){
	//	c--;
	//}
	//DrawFormatString(0, 0, GetColor(255,255,255), " x:%d y:%d z:%d", x, y, z);
	//DrawFormatString(0, 20, GetColor(255,255,255), " a:%d b:%d c:%d", a, b, c);
	//pos = VGet( x, y, z);
	//target = VGet(a,b,c);
	//


	//target = VAdd(pos, target);

	// �J�����̈ʒu�ƌ������Z�b�g����
	SetCameraPositionAndTarget_UpVecY(pos, target);
}
