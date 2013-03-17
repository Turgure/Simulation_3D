#pragma once
#include "BaseScene.h"

class HomeScene : public BaseScene{
public:
	HomeScene();
	virtual void initialize() override;
	virtual void update() override;
	virtual void draw() override;
};