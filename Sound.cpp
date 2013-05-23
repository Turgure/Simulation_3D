#include "DxLib.h"
#include "Sound.h"

int Sound::start_scene;
int Sound::battle_scene[5];
int Sound::gameover_scene;
int Sound::result_scene;

void Sound::initialize(){
	start_scene = LoadSoundMem("data/sound/bgm/opening/spmo_air.mp3");
	gameover_scene = LoadSoundMem("data/sound/bgm/gameover/spmo_soten.mp3");

	battle_scene[0] = LoadSoundMem("data/sound/bgm/battle/spmo_pan_s8.mp3");
	battle_scene[1] = LoadSoundMem("data/sound/bgm/battle/spmo_lft.mp3");
	battle_scene[2] = LoadSoundMem("data/sound/bgm/battle/spmo_ruinh.mp3");
}
