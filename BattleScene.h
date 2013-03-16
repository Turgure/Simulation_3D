#pragma once
#include "BaseScene.h"

class BattleScene : public BaseScene{
public:
	BattleScene();
	virtual void initialize() override;
	virtual void update() override;
	virtual void draw() override;
};