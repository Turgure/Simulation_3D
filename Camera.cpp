#include "Camera.h"
#include "Keyboard.h"
Camera::Camera(){

	// �J�����̍��W���Z�b�g
	x = 470;
	y = 290;
	z = 330;
	// �J�����̒����_���Z�b�g
	targetX = 190;
	targetY = -56;
	targetZ = 70;
}

void Camera::update(){

	//�J�����e�X�g�p�ł�
	//�J�����̈ʒu���ړ�
	if(Keyboard::pushing(KEY_INPUT_Z) ){
		x += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_A) ){
		x -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_X) ){
		y += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_S) ){
		y -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_C) ){
		z += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_D) ){
		z -= 3;
	}

	//�J�����̒����_�݈̂ړ�
	if(Keyboard::pushing(KEY_INPUT_V) ){
		targetX += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_F) ){
		targetX -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_B) ){
		targetY += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_G) ){
		targetY -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_N) ){
		targetZ += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_H) ){
		targetZ -= 3;
	}

	//�J�����𕽍s�ړ�
	if(Keyboard::pushing(KEY_INPUT_Q) ){
		x += 3;
		targetX +=3;
	}
	if(Keyboard::pushing(KEY_INPUT_W) ){
		x -= 3;
		targetX -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_E) ){
		y += 3;
		targetY += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_R) ){
		y -= 3;
		targetY -= 3;
	}
	if(Keyboard::pushing(KEY_INPUT_T) ){
		z += 3;
		targetZ += 3;
	}
	if(Keyboard::pushing(KEY_INPUT_Y) ){
		z -= 3;
		targetZ -= 3;
	}
	DrawFormatString(0, 60, GetColor(255,255,255), " x:%d y:%d z:%d", x, y, z);
	DrawFormatString(0, 80, GetColor(255,255,255), " a:%d b:%d c:%d", targetX, targetY, targetZ);
	pos = VGet( x, y, z);
	target = VGet(targetX,targetY,targetZ);

	//	���s�ړ�
	//target = VAdd(pos, target);


	// �J�����̈ʒu�ƌ������Z�b�g����
	SetCameraPositionAndTarget_UpVecY(pos, target);
}
