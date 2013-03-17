#pragma once
#include<DxLib.h>

class Camera{
public:
	Camera();
	void update();

	//�J�����̃|�W�V�����Z�b�g
	void setX(int x);
	void setY(int y);
	void setZ(int z);
	
	//�J�����̒����_���Z�b�g����
	void setTargetX(int targetZ);
	void setTargetY(int targetY);
	void setTargetZ(int targetZ);

	//�J�����̃|�W�V������Ԃ�
	int getX(){return x;}
	int getY(){return y;}
	int getZ(){return  z;}

		//�J�����̒����_��Ԃ�
	int getTargetX(){return targetX;}
	int getTargetY(){return targetY;}
	int getTargetZ(){return targetZ;}

private:
	int x, y, z;			//�J�����̍��W(x,y,z)
	int targetX, targetY, targetZ;			//�J�����̒����_(targetX, targetY, targetZ)

	VECTOR pos;		// �J�����̍��W
	VECTOR target;	// �J�����̒����_
};