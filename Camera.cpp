#include "Camera.h"

Camera::Camera(){
	// �J�����̒����_���Z�b�g
	target = VGet(  -1.0f, -1.0f,  -1.0f );
}

void Camera::update(){
	// �J�����̍��W���Z�b�g
	pos = VGet(500, 100, 100) ;
	static int i = 1;
	//target = VAdd(pos, target) ;
	
	// �J�����̈ʒu�ƌ������Z�b�g����
	SetCameraPositionAndTarget_UpVecY(pos, target) ;
}
