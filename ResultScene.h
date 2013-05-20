#pragma once
#include "BaseScene.h"

class ResultScene : public BaseScene{
public:
	ResultScene();
	virtual void initialize() override;
	virtual void update() override;
	virtual void draw() override;
};