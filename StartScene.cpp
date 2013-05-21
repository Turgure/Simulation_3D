#include "DxLib.h"
#include "FileStream.h"
#include "StartScene.h"
#include "BattleScene.h"
#include "Keyboard.h"

StartScene::StartScene(){
	background = LoadGraph("data/image/startscene/startscene.jpg");
	order.push_back(0);
	addMenu();
}

StartScene::~StartScene(){
	switch(order.size()-1){
	case 0:
		my_mission[1] = 0;
		break;
	case 1:
		my_mission[1] = order[order.size()-1];
		break;
	default:
		break;
	}

	FileStream::write("data/data.dat", my_mission);
}

void StartScene::initialize(){
	FileStream::load("data/data.dat", my_mission);
	if(my_mission.empty()){
		my_mission.push_back(0);
		my_mission.push_back(0);
		FileStream::write("data/data.dat", my_mission);
	}
}

void StartScene::update(){
	select(menus[order.size()-1]);
	action();
}

void StartScene::draw(){
	DrawGraph(0,0,background, true);
	for(unsigned int i = 0; i < order.size(); ++i){
		drawValues(menus[i], i);
	}
}

void StartScene::addMenu(){
	content[make_pair(280, 320)] = "New Game";
	content[make_pair(280, 340)] = "Continue";
	content[make_pair(280, 360)] = "Exit";
	menus.push_back(content);
	content.clear();

	content[make_pair(96, 16)] = "hoge 1";
	content[make_pair(96, 32)] = "hoge 2";
	content[make_pair(96, 48)] = "hoge 3";
	content[make_pair(96, 64)] = "hoge 4";
	content[make_pair(96, 80)] = "hoge 5";
	menus.push_back(content);
	content.clear();
}

void StartScene::select(map<pair<int, int>, string> maps){
	if(Keyboard::pushed(KEY_INPUT_DOWN)){
		if(maps == menus[1]){
			order[order.size()-1] = (order[order.size()-1] + 1) % (my_mission[0]+1);
		} else {
			order[order.size()-1] = (order[order.size()-1] + 1) % maps.size();
		}
	}

	if(Keyboard::pushed(KEY_INPUT_UP)){
		if(maps == menus[1]){
			order[order.size()-1] = (order[order.size()-1] + (my_mission[0])) % (my_mission[0]+1);
		} else {
			order[order.size()-1] = (order[order.size()-1] + (maps.size()-1)) % maps.size();
		}
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
				if(my_mission[0] > 0){
					order.push_back(0);
				}
				break;
			default:
				changeScene(nullptr);
				break;
			}
			break;

		case 1:
			changeScene(new BattleScene);
			break;
		}
	}

	if(Keyboard::pushed(KEY_INPUT_X) && order.size() >= 2){
		order.pop_back();
	}
}

void StartScene::drawValues(map<pair<int, int>, string> maps, int num){
	int i = 0, color;
	for(auto& map : maps){
		if(num == 1 && my_mission[0] < i) break;

		if(i == order[num]){
			color = GetColor(0,255,0);
		} else {
			color = GetColor(255,255,255);
		}

		DrawString(map.first.first, map.first.second, map.second.c_str(), color);
		++i;
	}
}
