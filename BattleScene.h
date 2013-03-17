#pragma once
#include <vector>
#include "BaseScene.h"
#include "Stage.h"
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
	int turn;
	bool has_come_turn;
	bool act_only_one;

	Stage stage;
	Cursor cursor;
	vector<Player> players;
	vector<Enemy> enemies;
};