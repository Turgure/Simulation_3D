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
int Sound::item;
int Sound::cursor;

void Sound::initialize(){
	start_scene = LoadSoundMem("data/sound/bgm/free0219.mp3");
	gameover_scene = LoadSoundMem("data/sound/bgm/spmo_soten.mp3");
	result_scene = LoadSoundMem("data/sound/bgm/game_maoudamashii_9_jingle04.mp3");
	battle_scene[0] = LoadSoundMem("data/sound/bgm/spmo_pan_s8.mp3");
	battle_scene[1] = LoadSoundMem("data/sound/bgm/spmo_morn.mp3");
	battle_scene[2] = LoadSoundMem("data/sound/bgm/spmo_lft.mp3");
	battle_scene[3] = LoadSoundMem("data/sound/bgm/spmo_ruinh.mp3");
	battle_scene[4] = LoadSoundMem("data/sound/bgm/spmo_kiiki.mp3");

	step = LoadSoundMem("data/sound/se/decide4.wav");
	decide = LoadSoundMem("data/sound/se/decide1.wav");
	cancel = LoadSoundMem("data/sound/se/tama1.wav");
	select = LoadSoundMem("data/sound/se/cursor8.wav");
	shot = LoadSoundMem("data/sound/se/se_maoudamashii_battle_gun02.mp3");
	thunder = LoadSoundMem("data/sound/se/denpa9.mp3");
	item = LoadSoundMem("data/sound/se/a2-034_recovery_03.mp3");
	cursor = LoadSoundMem("data/sound/se/button45.mp3");
}
