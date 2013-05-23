#pragma once
#include <vector>
#include <string>
using namespace std;

class CommandSelect{
public:
	struct Contents{
		Contents(const string& words, const string& description):words(words), description(description){};
		string words;
		string description;
	};


	CommandSelect();
	void loadCommands();
	void initCommands();

	void update();
	void draw(int x, int y);
	
	bool commandIs(const string& words) const;
	void setSelectNum(int select_num);

	void step();
	void back();
	void clear();
	int setNext();

private:
	int cnt;

	vector<int> select_num;
	vector<int> prev;
	int current;
	int next;
	
	vector<vector<string>> commands[256];
	vector<Contents> content[256];
	enum State{
		SELECT,
		ATTACK,
		SWORDPLAY,
		WHITE_MAGIC,
		BLACK_MAGIC
	};
};
