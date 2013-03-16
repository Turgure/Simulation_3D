#include <DxLib.h>
#include "BaseScene.h"
#include "Keyboard.h"

bool BaseScene::running = false;

BaseScene* BaseScene::main(){
	if(running) return nullptr;	//1度に実行できるシーンは1つ
	running = true;
	next_scene = nullptr;

	//ループ前処理
	initialize();
	looping = true;
	
	//メインループ
	while(looping){
		if(processLoop()){
			running = false;
			return nullptr;
		}
		update();
		draw();
		ScreenFlip();	//裏画面を表画面に反映
	}

	//ループ後処理
	finalize();
	running = false;

	return next_scene;
}

void BaseScene::changeScene(BaseScene* next_scene){
	this->next_scene = next_scene;
	looping = false;
}

int BaseScene::processLoop(){
	if(ProcessMessage() != 0)  return -1;	//プロセス処理がエラーなら-1を返す
	if(ClearDrawScreen() != 0) return -1;	//画面クリア処理がエラーなら-1を返す
	if(Keyboard::update() != 0) return -1;	//キーのアップデート処理がエラーなら-1を返す
	return 0;
}


bool SceneManager::running = false;
BaseScene* SceneManager::current_scene;

void SceneManager::run(BaseScene* starter){
	if(running || !starter) return;
	running = true;

	current_scene = starter;
	BaseScene* next_scene = nullptr;
	//メインループ開始
	do{
		next_scene = current_scene->main();
		delete current_scene;
	}while(current_scene = next_scene);

	running = false;
}