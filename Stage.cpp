#include <DxLib.h>
#include "Stage.h"
#include "GV.h"
#include "FileStream.h"
#include "Cursor.h"

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
	for(int i = 0; i < 5; ++i)
		mapchipDefinition.push_back( MapchipDefinition(i) );

	for(auto& chip : mapchipDefinition){
		switch(chip.id){
		case 0://chip which can't move 
			chip.image = MV1LoadModel("data/image/3Dmodel/chip/00 null.x");
			//chip.image = GetColor(255, 128, 0);
			chip.resistance = INT_MAX;
			break;

		case 1://movable（id=1 => 草原, id=2 => 岩…　などにする）
			chip.image = MV1LoadModel("data/image/3Dmodel/chip/01 grass.x");
			//chip.image = GetColor(128, 255, 0);
			chip.resistance = 1;
			break;

		case 2://沼地的な
			chip.image = MV1LoadModel("data/image/3Dmodel/chip/02 marsh.x");
			//chip.image = GetColor(128, 0, 128);
			chip.resistance = 2;
			break;

		default:
			break;
		}
	}
}

void Stage::initMap(){
	vector<vector<string>> mapdata;
	FileStream::load("data/stage/stage2/map.csv", mapdata);

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

void Stage::update(){
}

void Stage::draw(){
	drawMap();
	drawBrightenedPoints();
}

void Stage::drawSquare(VECTOR v1, VECTOR v2, VECTOR v3, VECTOR v4,int color){
	DrawTriangle3D(v1, v2, v3, color, true);
	DrawTriangle3D(v2, v3, v4, color, true);
}

void Stage::drawChip(int x, int y, int color){
	drawSquare(
		VGet(y    *chipsize, mapchip[y][x].height*chipheight, x    *chipsize),
		VGet((y+1)*chipsize, mapchip[y][x].height*chipheight, x    *chipsize),
		VGet(y    *chipsize, mapchip[y][x].height*chipheight, (x+1)*chipsize),
		VGet((y+1)*chipsize, mapchip[y][x].height*chipheight, (x+1)*chipsize),
		color);
}

void Stage::drawMap(){
	for(int d = 0; d < depth; ++d){
		for(int w = 0; w < width; ++w){
			for(int h = 0; h <=  mapchip[d][w].height; ++h){
			MV1SetPosition(mapchip[d][w].definition->image, VAdd(VGet(d*chipsize, h*chipheight, w*chipsize), VGet(chipsize/2, -chipheight/2, chipsize/2)));
			MV1DrawModel(mapchip[d][w].definition->image);
			}
		}
	}
}

void Stage::drawBrightenedPoints(){
	for(int d = 0; d < depth; ++d){
		for(int w = 0; w < width; ++w){
			if(mapchip[d][w].is_brighting){
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
				drawChip(w, d, mapchip[d][w].bright_color);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			if(w == Cursor::pos.x && d == Cursor::pos.y){
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
				drawChip(w, d, GetColor(255,0,0));
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
}

void Stage::lateUpdate(){
	for(int d = 0; d < depth; ++d){
		for(int w = 0; w < width; ++w){
			mapchip[d][w].object = nullptr;
		}
	}
}

bool Stage::canMove(const Position& pos){
	if(pos.x >= 0 && pos.y >= 0 && pos.x < width && pos.y < depth){
		return (mapchip[pos.y][pos.x].definition->id != 0);
	} else {
		return false;
	}
}

void Stage::brighten(const Position& pos, int color){
	mapchip[pos.y][pos.x].is_brighting = true;
	mapchip[pos.y][pos.x].bright_color = color;
}

void Stage::disbrighten(){
	for(int d = 0; d < depth; ++d){
		for(int w = 0; w < width; ++w){
			mapchip[d][w].is_brighting = false;
		}
	}
}
