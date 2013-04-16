#pragma once
#include "BaseScene.h"

class StartScene : public BaseScene{
public:
	StartScene();
	virtual void initialize() override;
	virtual void update() override;
	virtual void draw() override;

private:
	int selected_menu;

};