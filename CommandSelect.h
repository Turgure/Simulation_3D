#pragma once
#include <vector>

class CommandSelect{
private:
	int select_num;
	
public:
	struct Contents{
		Contents(int x, int y, char* words):x(x), y(y), words(words){};
		int x, y;
		char* words;
	};
	std::vector<Contents> content;

	CommandSelect():select_num(0){};
	void update();
	void draw();
	void add(int x, int y, char* words);
	void setSelectNum(int select_num);
	char* getCommand() const;
};