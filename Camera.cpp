#include "Camera.h"

Camera::Camera(){
	// �J�����̒����_���Z�b�g
	target = VGet(-1.0f, -1.0f, -1.0f);
}

void Camera::update(){
	static int i = 1;
	// �J�����̍��W���Z�b�g
	pos = VGet(500, 100, 100);
	//target = VAdd(pos, target);
	
	// �J�����̈ʒu�ƌ������Z�b�g����
	SetCameraPositionAndTarget_UpVecY(pos, target);
}
