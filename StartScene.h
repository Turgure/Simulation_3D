#pragma once
#include <vector>
#include <map>
#include "BaseScene.h"
#include "Position.h"
using namespace std;

class StartScene : public BaseScene{
public:
	StartScene();
	~StartScene();
	virtual void initialize() override;
	virtual void update() override;
	virtual void draw() override;

private:
	vector<int> order;
	vector<int> my_mission;

	void addMenu();
	void select(map<pair<int, int>, string> maps);
	void action();
	void drawValues(map<pair<int, int>, string> maps, int num);

	//map<Position, string> map_name;
	vector< map<pair<int, int>, string> > menus;
	map<pair<int, int>, string> content;
};
