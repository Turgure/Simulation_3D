#include "DxLib.h"
#include "Sound.h"
#include "Keyboard.h"
#include "BattleScene.h"
#include "StartScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	// ウインドウモードに変更
	ChangeWindowMode(true);
	if(DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0) return -1;	//初期化と裏画面処理
	SetMainWindowText("tannpo");

	Keyboard::initialize();
	Sound::initialize();
	
	SceneManager::run(new StartScene);

	DxLib_End();
	return 0;
}