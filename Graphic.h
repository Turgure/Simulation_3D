#pragma once
#include <vector>
using namespace std;

class Graphic{
public:
	static void initialize();
	
	//graphic名
	static int picture_frame;
	static int cursor;

	//object
	//static vector<int> player_model;
	static int player_model[256];
	static vector<int> enemy_model;
	//static int hoge;
	//static int foo;
	//static int fuga;
};
