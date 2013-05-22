#include "DxLib.h"
#include "Sound.h"

int Sound::battle_scene[5];
int Sound::start_scene;

void Sound::initialize(){
	battle_scene[0] = LoadSoundMem("data/sound/bgm/loop_13.wav");
	start_scene = LoadSoundMem("data/sound/bgm/free0219.mp3");
}