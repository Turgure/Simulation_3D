#include "DxLib.h"
#include "Sound.h"

int Sound::start_scene;
int Sound::battle_scene[5];
int Sound::gameover_scene;
int Sound::result_scene;

int Sound::step;
int Sound::decide;
int Sound::cancel;
int Sound::select;
int Sound::shot;
int Sound::thunder;

void Sound::initialize(){
	start_scene = LoadSoundMem("data/sound/bgm/opening/free0219.mp3");
	gameover_scene = LoadSoundMem("data/sound/bgm/gameover/spmo_soten.mp3");
	result_scene = LoadSoundMem("data/sound/bgm/result/game_maoudamashii_9_jingle04.mp3");
	battle_scene[0] = LoadSoundMem("data/sound/bgm/battle/spmo_pan_s8.mp3");
	battle_scene[1] = LoadSoundMem("data/sound/bgm/battle/spmo_morn.mp3");
	battle_scene[2] = LoadSoundMem("data/sound/bgm/battle/spmo_lft.mp3");
	battle_scene[3] = LoadSoundMem("data/sound/bgm/battle/spmo_ruinh.mp3");
	battle_scene[4] = LoadSoundMem("data/sound/bgm/battle/spmo_kiiki.mp3");

	step = LoadSoundMem("data/sound/se/decide4.wav");
	decide = LoadSoundMem("data/sound/se/decide1.wav");
	cancel = LoadSoundMem("data/sound/se/tama1.wav");
	select = LoadSoundMem("data/sound/se/cursor8.wav");
	shot = LoadSoundMem("data/sound/se/se_maoudamashii_battle_gun02.mp3");
	thunder = LoadSoundMem("data/sound/se/denpa9.mp3");

}
