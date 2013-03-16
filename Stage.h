#pragma once

class Stage{
public:
	Stage();
	void initialize();
	void update();
	void draw();

private:
	int map[10][10];
};