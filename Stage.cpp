#include <DxLib.h>
#include "Stage.h"
#include "GV.h"
#include "FileStream.h"
//#include "Cursor.h"

int Stage::width;
int Stage::depth;
int Stage::leftup_positionX = 128;
int Stage::leftup_positionY = 128;
Stage::Mapchip Stage::mapchip[100][100];

Stage::Stage(){
	initialize();
}

void Stage::initialize(){
	current_map = 1;
	initID();
	initMap();
}

void Stage::initID(){
	//i => kind of id
	for(int i = 0; i < 5; i++)
		mapchipDefinition.push_back( MapchipDefinition(i) );

	for(auto& chip : mapchipDefinition){
		switch(chip.id){
		case 0://chip which can't move 
			chip.image = GetColor(255, 128, 0);
			chip.resistance = INT_MAX;
			break;

		case 1://movable（id=1 => 草原, id=2 => 岩…　などにする）
			chip.image = GetColor(128, 255, 0);
			chip.resistance = 1;
			break;

		case 2://沼地的な
			chip.image = GetColor(128, 0, 128);
			chip.resistance = 2;
			break;

		default:
			break;
		}
	}
}

void Stage::initMap(){
	vector<vector<string>> mapdata;
	FileStream::load("data/stage/stage1/map.csv", mapdata);


	auto& header = mapdata[0];
	width = stoi(header[0]);
	depth = stoi(header[1]);
	//idの読み込み
	for(int d = 0; d < depth; ++d){
		for(int w = 0; w < width; ++w){
			mapchip[d][w].definition = &mapchipDefinition[stoi(mapdata[d+1][w])];
			mapchip[d][w].height = stoi(mapdata[d+depth+1][w]);

		}
	}
}


void Stage::drawSquare(VECTOR v1, VECTOR v2, VECTOR v3, VECTOR v4,int color){
	DrawTriangle3D(v1, v2, v3, color, false);
	DrawTriangle3D(v2, v3, v4, color, false);
}

void Stage::draw(){
	VECTOR v1, v2, v3, v4;

	for(int d = 0; d < depth; ++d){
		for(int w = 0; w < width; ++w){
			v1 = VGet( d * chipsize            ,       mapchip[d][w].height*chipheight, w * chipsize );
			v2 = VGet( d * chipsize + chipsize,       mapchip[d][w].height*chipheight, w * chipsize );
			v3 = VGet( d * chipsize             ,       mapchip[d][w].height*chipheight, w * chipsize + chipsize );
			v4 = VGet( d * chipsize + chipsize,       mapchip[d][w].height*chipheight, w * chipsize + chipsize );

			drawSquare(v1, v2, v3, v4, GetColor(0,255,0));
		}
	}
}

void Stage::update(){
	camera.update();
}