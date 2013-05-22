#pragma once
#include "BaseScene.h"

class GameoverScene : public BaseScene{
public:
	GameoverScene();
	virtual void initialize() override;
	virtual void update() override;
	virtual void draw() override;

private:
	int bg;
};