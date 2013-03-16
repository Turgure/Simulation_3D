#pragma once
#include "BaseScene.h"
#include "Cursor.h"
#include "Stage.h"
#include "Object.h"
using namespace std;

class BattleScene : public BaseScene{
public:
	BattleScene();
	virtual void initialize() override;
	virtual void update() override;
	virtual void draw() override;

private:
	Stage stage;
	Cursor cursor;
	vector<Player> players;
	vector<Enemy> enemies;
};