#include <DxLib.h>
#include "Command.h"
#include "FileStream.h"
#include "Keyboard.h"

CommandSelect::CommandSelect(){
	loadCommands();
	initCommands();

	select_num.push_back(0);
	current = MV_ACT_END;
	next = MV_ACT_END;

	cnt = -100;
}

void CommandSelect::update(){
	//content[0]はindex
	if(Keyboard::pushed(KEY_INPUT_DOWN)){
		select_num[current] = (select_num[current] + 1) % (content[current].size()-1);
		next = setNext();
	} else if(Keyboard::pushed(KEY_INPUT_UP)){
		select_num[current] = (select_num[current] + (content[current].size()-2)) % (content[current].size()-1);
		next = setNext();
	}

	int size = content[current][select_num[current]+1].description.size();
	if(++cnt >= DEFAULT_SCREEN_SIZE_X + size*10){
		cnt = -100;
	}
}

void CommandSelect::draw(int x, int y){
	for(unsigned int i = 0; i < (content[current].size()-1); ++i){
		if(i == select_num[current]){
			DrawFormatString(x, y + i*20, GetColor(255,255,255), "-> %s", content[current][i+1].words.c_str());
		} else {
			DrawFormatString(x, y + i*20, GetColor(255,255,255), "   %s", content[current][i+1].words.c_str());
		}
	}

	DrawFormatString(DEFAULT_SCREEN_SIZE_X - cnt, DEFAULT_SCREEN_SIZE_Y-16,
		GetColor(255,255,255), "%s", content[current][select_num[current]+1].description.c_str());
}

void CommandSelect::loadCommands(){
	FileStream::load("data/command/00 mv_act_end.csv", commands[00]);
	FileStream::load("data/command/01 action.csv", commands[01]);
	FileStream::load("data/command/02 swordplay.csv", commands[02]);
	FileStream::load("data/command/03 white_magic.csv", commands[02]);
	FileStream::load("data/command/04 black_magic.csv", commands[03]);
}

void CommandSelect::initCommands(){
	for(unsigned int i = 0; i < 256; ++i){
		for(unsigned int j = 0; j < commands[i].size(); ++j){
			content[i].push_back( Contents(commands[i][j][0], commands[i][j][1]) );
		}
	}
}

bool CommandSelect::commandIs(const string& words) const{
	return (content[current][select_num[current]+1].words == words);
}

void CommandSelect::setSelectNum(int select_num){
	this->select_num[current] = select_num;
}

void CommandSelect::step(){
	select_num.push_back(0);

	prev.push_back(current);
	current = next;
}

void CommandSelect::back(){
	select_num.pop_back();

	next = current;
	current = prev.back();
	prev.pop_back();
}

void CommandSelect::clear(){
	select_num.clear();
	select_num.push_back(0);
	prev.clear();
	current = MV_ACT_END;
}

int CommandSelect::setNext(){
	if(commandIs("MOVE")) return MV_ACT_END;
	if(commandIs("ACTION")) return ATTACK;
	if(commandIs("END")) return MV_ACT_END;

	if(commandIs("たたかう")) return MV_ACT_END;
	if(commandIs("とくぎ")) return MV_ACT_END;
	else return -1;
}
