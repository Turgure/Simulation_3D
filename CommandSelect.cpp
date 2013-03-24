﻿#include <DxLib.h>
#include "CommandSelect.h"
#include "Keyboard.h"

void CommandSelect::update(){
	if(Keyboard::pushed(KEY_INPUT_DOWN)){
		select_num = (select_num + 1) % content.size();
	} else if(Keyboard::pushed(KEY_INPUT_UP)){
		select_num = (select_num + (content.size()-1)) % content.size();
	}
}

void CommandSelect::draw(){
	for(unsigned int i = 0; i < content.size(); ++i){
		if(i == select_num){
			DrawFormatString(content[i].x, content[i].y, GetColor(255,255,255), "-> %s", content[i].words);
		} else {
			DrawFormatString(content[i].x, content[i].y, GetColor(255,255,255), "   %s", content[i].words);
		}
	}
}

void CommandSelect::add(int x, int y, char* words){
	content.push_back( Contents(x, y, words) );
}

void CommandSelect::setSelectNum(int select_num){
	this->select_num = select_num;
}

char* CommandSelect::getCommand() const{
	return content[select_num].words;
}