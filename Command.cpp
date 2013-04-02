#include <DxLib.h>
#include "Command.h"
#include "FileStream.h"
#include "Keyboard.h"

CommandSelect::CommandSelect(){
	loadCommands();
	initCommands();

	select_num = 0;
	current = MV_ACT_END;
	next = MV_ACT_END;
}

void CommandSelect::update(){
	//content[0]はindex
	if(Keyboard::pushed(KEY_INPUT_DOWN)){
		select_num = (select_num + 1) % (content[current].size()-1);
		next = setNext();
	} else if(Keyboard::pushed(KEY_INPUT_UP)){
		select_num = (select_num + (content[current].size()-2)) % (content[current].size()-1);
		next = setNext();
	}
}

void CommandSelect::draw(int x, int y){
	for(unsigned int i = 0; i < (content[current].size()-1); ++i){
		if(i == select_num){
			DrawFormatString(x, y + i*20, GetColor(255,255,255), "-> %s", content[current][i+1].words.c_str());
		} else {
			DrawFormatString(x, y + i*20, GetColor(255,255,255), "   %s", content[current][i+1].words.c_str());
		}
	}
}

void CommandSelect::loadCommands(){
	FileStream::load("data/command/00 mv_act_end.csv", commands[00]);
	FileStream::load("data/command/01 swordplay.csv", commands[01]);
	FileStream::load("data/command/02 white_magic.csv", commands[02]);
	FileStream::load("data/command/03 black_magic.csv", commands[03]);
}

void CommandSelect::initCommands(){
	for(int i = 0; i < 256; ++i){
		for(int j = 0; j < commands[i].size(); ++j){
			content[i].push_back( Contents(commands[i][j][0], commands[i][j][1]) );
		}
	}
}

bool CommandSelect::commandIs(const string& words) const{
	return (content[current][select_num+1].words == words);
}

void CommandSelect::setSelectNum(int select_num){
	this->select_num = select_num;
}

void CommandSelect::step(){
	prev.push_back(current);
	current = next;
}

void CommandSelect::back(){
	next = current;
	current = prev.back();
	prev.pop_back();
}

void CommandSelect::clear(){
	prev.clear();
}

int CommandSelect::setNext(){
	if(commandIs("MOVE")) return MV_ACT_END;
	if(commandIs("ACTION")) return MV_ACT_END;
	if(commandIs("END")) return MV_ACT_END;
}
