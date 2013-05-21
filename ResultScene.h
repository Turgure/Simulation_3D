#pragma once
#include "BaseScene.h"
#include <vector>

class ResultScene : public BaseScene{
public:
	ResultScene();
	~ResultScene();
	virtual void initialize() override;
	virtual void update() override;
	virtual void draw() override;

private:
	int imy_mission;
	std::vector<int> my_mission;
};