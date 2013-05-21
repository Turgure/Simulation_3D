#include "DxLib.h"
#include "Graphic.h"

//vector<int> Graphic::player_model;
int Graphic::player_model[256];
vector<int> Graphic::enemy_model;

void Graphic::initialize(){
	player_model[0] = MV1LoadModel("data/image/3Dmodel/chara/woman003/waitting.pmx");
	player_model[1] = MV1LoadModel("data/image/3Dmodel/chara/woman003/attack01.pmx");
	player_model[2] = MV1LoadModel("data/image/3Dmodel/chara/woman003/attack02.pmx");
	player_model[3] = MV1LoadModel("data/image/3Dmodel/chara/woman003/attack03.pmx");
	player_model[4] = MV1LoadModel("data/image/3Dmodel/chara/woman003/attack04.pmx");
	player_model[5] = MV1LoadModel("data/image/3Dmodel/chara/woman003/attack05.pmx");
	player_model[6] = MV1LoadModel("data/image/3Dmodel/chara/woman003/attack06.pmx");
	//player_model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/waitting.pmx") );
	//player_model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack01.pmx") );
	//player_model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack02.pmx") );
	//player_model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack03.pmx") );
	//player_model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack04.pmx") );
	//player_model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack05.pmx") );
	//player_model.push_back( MV1LoadModel("data/image/3Dmodel/chara/woman003/attack06.pmx") );

	enemy_model.push_back( MV1LoadModel("data/image/3Dmodel/chara/miku.pmd") );
}
