#include "DxLib.h"
#include "Sound.h"
#include "Keyboard.h"
#include "BattleScene.h"
#include "StartScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	// �E�C���h�E���[�h�ɕύX
	ChangeWindowMode(true);
	if(DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0) return -1;	//�������Ɨ���ʏ���
	SetMainWindowText("tannpo");

	Keyboard::initialize();
	Sound::initialize();
	
	SceneManager::run(new StartScene);

	DxLib_End();
	return 0;
}