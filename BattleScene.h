#pragma once
#include <vector>
#include "BaseScene.h"
#include "Stage.h"
#include "Cursor.h"
#include "Object.h"
#include "Camera.h"

using namespace std;

class BattleScene : public BaseScene{
public:
	BattleScene();
	virtual void initialize() override;
	virtual void update() override;
	virtual void draw() override;
	void lateUpdate();

private:
	int turn;
	bool has_come_turn;
	bool act_only_one;

	void simulate();

	Stage stage;
	Cursor cursor;
	Camera camera;
	vector<Player> players;
	vector<Enemy> enemies;

	vector<string> order;
};
