#pragma once
#include<DxLib.h>

class Camera{
public:
	Camera();
	void update();

private:
	VECTOR pos;		// �J�����̍��W
	VECTOR target;	// �J�����̒����_
};