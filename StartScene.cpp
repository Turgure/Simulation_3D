#include "DxLib.h"
#include "FileStream.h"
#include "StartScene.h"
#include "BattleScene.h"
#include "Keyboard.h"

StartScene::StartScene(){
	bg = LoadGraph("data/image/bg/start.jpg");
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	SetUseZBuffer3D(true);
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

	StopSoundMem(Sound::start_scene);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void StartScene::initialize(){
	FileStream::load("data/data.dat", my_mission);
	if(my_mission.empty()){
		my_mission.push_back(0);
		my_mission.push_back(0);
		FileStream::write("data/data.dat", my_mission);
	}
	if(my_mission[0] <= 0) order.push_back(0);
	else order.push_back(1);

	PlaySoundMem(Sound::start_scene, DX_PLAYTYPE_LOOP);
}

void StartScene::update(){
	select(menus[order.size()-1]);
	action();
}

void StartScene::draw(){
	DrawGraph(0, 0, bg, true);
	drawValues(menus[order.size()-1], (order.size() == 1));
}

void StartScene::addMenu(){
	content[make_pair(280, 330)] = "New Game";
	content[make_pair(280, 360)] = "Continue";
	content[make_pair(280, 390)] = "Exit";
	menus.push_back(content);
	content.clear();

	content[make_pair(60,  40)] = "Mission 1";
	content[make_pair(60,  70)] = "Mission 2";
	content[make_pair(60, 100)] = "Mission 3";
	content[make_pair(60, 130)] = "Mission 4";
	content[make_pair(60, 160)] = "Mission 5";
	menus.push_back(content);
	content.clear();
}

void StartScene::select(map<pair<int, int>, string> maps){
	if(Keyboard::pushed(KEY_INPUT_DOWN)){
		PlaySoundMem(Sound::select, DX_PLAYTYPE_BACK);
		if(maps == menus[1]){
			order[order.size()-1] = (order[order.size()-1] + 1) % (my_mission[0]+1);
		} else {
			order[order.size()-1] = (order[order.size()-1] + 1) % maps.size();
		}
	}

	if(Keyboard::pushed(KEY_INPUT_UP)){
		PlaySoundMem(Sound::select, DX_PLAYTYPE_BACK);
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
				PlaySoundMem(Sound::decide, DX_PLAYTYPE_BACK);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				DrawString(5, DEFAULT_SCREEN_SIZE_Y-20, "Now Loading...", GetColor(255,255,255));
				changeScene(new BattleScene);
				break;
			case 1:
				if(my_mission[0] > 0){
					PlaySoundMem(Sound::step, DX_PLAYTYPE_BACK);
					order.push_back(0);
				}
				break;
			default:
				PlaySoundMem(Sound::step, DX_PLAYTYPE_BACK);
				changeScene(nullptr);
				break;
			}
			break;

		case 1:
			PlaySoundMem(Sound::decide, DX_PLAYTYPE_BACK);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawString(5, DEFAULT_SCREEN_SIZE_Y-20, "Now Loading...", GetColor(255,255,255));
			changeScene(new BattleScene);
			break;
		}
	}

	if(Keyboard::pushed(KEY_INPUT_X) && order.size() >= 2){
		PlaySoundMem(Sound::cancel, DX_PLAYTYPE_BACK);
		order.pop_back();
	}
}

void StartScene::drawValues(map<pair<int, int>, string> maps, bool show_all){
	SetFontSize(20);
	int i = 0, color;
	for(auto& map : maps){
		if(!show_all && my_mission[0] < i) break;

		if(i == order[order.size()-1]){
			color = GetColor(0,255,0);
		} else {
			color = GetColor(255,255,255);
		}

		DrawString(map.first.first, map.first.second, map.second.c_str(), color);
		++i;
	}
	SetFontSize(16);
}
