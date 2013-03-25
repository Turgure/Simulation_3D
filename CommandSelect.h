#pragma once
#include <vector>
#include <string>
using namespace std;

class CommandSelect{
private:
	int select_num;
	
public:
	struct Contents{
		Contents(int x, int y, string words):x(x), y(y), words(words){};
		int x, y;
		string words;
	};
	vector<Contents> content;

	CommandSelect():select_num(0){};
	void update();
	void draw();
	void add(int x, int y, string words);
	void setSelectNum(int select_num);
	bool commandIs(string words) const;
};