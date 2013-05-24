#include "DxLib.h"
#include "Command.h"
#include "Sound.h"
#include "FileStream.h"
#include "Keyboard.h"

CommandSelect::CommandSelect(){
	loadCommands();
	initCommands();

	select_num.push_back(0);
	current = SELECT;
	next = SELECT;

	cnt = -100;
}

void CommandSelect::update(){
	//content[0]はindex
	if(Keyboard::pushed(KEY_INPUT_DOWN)){
		PlaySoundMem(Sound::select, DX_PLAYTYPE_BACK);
		select_num[current] = (select_num[current] + 1) % (content[current].size()-1);
		next = setNext();
	} else if(Keyboard::pushed(KEY_INPUT_UP)){
		PlaySoundMem(Sound::select, DX_PLAYTYPE_BACK);
		select_num[current] = (select_num[current] + (content[current].size()-2)) % (content[current].size()-1);
		next = setNext();
	}

	int size = content[current][select_num[current]+1].description.size();
	if(++cnt >= DEFAULT_SCREEN_SIZE_X + size*10){
		cnt = -100;
	}
}

void CommandSelect::draw(int x, int y){
	SetFontSize(32);
	int color;
	for(unsigned int i = 0; i < (content[current].size()-1); ++i){
		if(i == select_num[current]){
			color = GetColor(255,128,0);
		} else {
			color = GetColor(255,255,255);
		}
		DrawFormatString(x, y + i*32+5, color, "%s", content[current][i+1].words.c_str());
	}
	SetFontSize(16);

	DrawFormatString(DEFAULT_SCREEN_SIZE_X - cnt, DEFAULT_SCREEN_SIZE_Y-16,
		GetColor(0,0,0), "%s", content[current][select_num[current]+1].description.c_str());
}

void CommandSelect::loadCommands(){
	FileStream::loadCSV("data/command/00 select.csv", commands[00]);
	FileStream::loadCSV("data/command/01 action.csv", commands[01]);
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
	current = SELECT;
}

int CommandSelect::setNext(){
	if(commandIs("移動")) return SELECT;
	if(commandIs("行動")) return ATTACK;
	if(commandIs("終了")) return SELECT;

	if(commandIs("たたかう")) return SELECT;
	if(commandIs("アイテム")) return SELECT;
	else return -1;
}
