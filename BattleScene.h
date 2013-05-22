#pragma once
#include <list>
#include <vector>
#include "BaseScene.h"
#include "Stage.h"
#include "Camera.h"
#include "Cursor.h"
#include "Object.h"

using namespace std;

class BattleScene : public BaseScene{
public:
	BattleScene();
	virtual void initialize() override;
	virtual void update() override;
	virtual void draw() override;
	void lateUpdate();

private:
	int bg;

	vector<int> my_mission;
	int imy_mission;

	bool has_come_turn;
	bool act_only_one;

	void simulate();

	Stage stage;
	Camera camera;
	Cursor cursor;
	vector<Player> players;
	vector<Enemy> enemies;

	list<string> order;
};
