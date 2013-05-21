#include <DxLib.h>
#include "FileStream.h"
#include "StartScene.h"
#include "BattleScene.h"
#include "Keyboard.h"

StartScene::StartScene(){
	order.push_back(0);
	addMenu();
}

StartScene::~StartScene(){
	int imy_mission;
	if(my_mission.empty()){
		my_mission.push_back(0);
	} else {
		imy_mission = my_mission[0];
	}
	FileStream::write("data/data.dat", my_mission);
}

void StartScene::initialize(){
	FileStream::load("data/data.dat", my_mission);
}

void StartScene::update(){
	select(menus[order.size()-1]);
	action();
}

void StartScene::draw(){
	for(unsigned int i = 0; i < order.size(); ++i){
		drawValues(menus[i], i);
	}

	//debug print
	if(my_mission.empty())
		DrawFormatString(0, 100, GetColor(255,255,255), "empty");
	else
		DrawFormatString(0, 100, GetColor(255,255,255), "%d", my_mission[0]);
}

void StartScene::addMenu(){
	content[make_pair(0, 16)] = "New Game";
	content[make_pair(0, 32)] = "Continue";
	content[make_pair(0, 48)] = "Exit";
	menus.push_back(content);
	content.clear();

	content[make_pair(96, 16)] = "Mission 1";
	content[make_pair(96, 32)] = "Mission 2";
	content[make_pair(96, 48)] = "Mission 3";
	content[make_pair(96, 64)] = "Mission 4";
	content[make_pair(96, 80)] = "Mission 5";
	content[make_pair(96, 96)] = "Mission 6";
	menus.push_back(content);
	content.clear();
}

void StartScene::select(map<pair<int, int>, string> maps){
	if(Keyboard::pushed(KEY_INPUT_DOWN)){
		order[order.size()-1] = (order[order.size()-1] + 1) % maps.size();
	}
	if(Keyboard::pushed(KEY_INPUT_UP)){
		order[order.size()-1] = (order[order.size()-1] + (maps.size()-1)) % maps.size();
	}
}

void StartScene::action(){
	if(Keyboard::pushed(KEY_INPUT_Z)){
		switch(order.size()-1){
		case 0:
			switch(order[order.size()-1]){
			case 0:
				changeScene(new BattleScene);
				break;
			case 1:
				order.push_back(0);
				break;
			default:
				break;
			}
			break;

		case 1:
			switch(order[order.size()-1]){
			case 0:
				//changeScene(new Mission1);
				break;
			case 1:
				//changeScene(new Mission2);
				break;
			}
			break;
		}
	}

	if(Keyboard::pushed(KEY_INPUT_X) && order.size() >= 1){
		order.pop_back();
	}
}

void StartScene::drawValues(map<pair<int, int>, string> maps, int num){
	int i = 0, color;
	for(auto& map : maps){
		if(i == order[num]){
			color = GetColor(0,255,0);
		} else {
			color = GetColor(255,255,255);
		}

		DrawString(map.first.first, map.first.second, map.second.c_str(), color);
		++i;
	}
}
