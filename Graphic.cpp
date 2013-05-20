#include "DxLib.h"
#include "Graphic.h"

int Graphic::picture_frame;

void Graphic::initialize(){
	picture_frame = LoadGraph("data/image/frame/frame085_144x96+.png");

	//hoge = LoadGraph(data/sound/BGM/hoge.mp3);
}