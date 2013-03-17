#pragma once
#include <vector>
#include "Position.h"
#include "Camera.h"
using namespace std;

class BaseObject;
class Stage{
public:
	Stage();
	void initialize();
	void initID();
	void initMap();

	void update();
	void draw();
	void drawMap();
	void drawBrightenedPoints();
	void lateUpdate();

	static void brighten(const Position& pos, int color);
	static bool isBrightened(const Position& pos);
	static void disbrighten();

	static void setObjectAt(const Position& pos, BaseObject* obj);
	static BaseObject* getObjectAt(const Position& pos);
	static bool canMove(const Position& pos);
	static int getResistance(const Position& pos);

	static int getWidth(){ return width; }
	static int getDepth(){ return depth; }
	static int getHeight(const Position& pos){ return mapchip[pos.y][pos.x].height; }
	static int getLeftupPositionX(){ return leftup_positionX; }
	static int getLeftupPositionY(){ return leftup_positionY; }

private:
	int current_map;

	static int width;
	static int depth;
	static int leftup_positionX;
	static int leftup_positionY;
	
	Camera camera;
	
	void drawSquare(VECTOR v1, VECTOR v2, VECTOR v3, VECTOR v4, int color , bool fillFlag);
	void drawChip(int x, int y, int color, bool fillFlag);

	struct MapchipDefinition{
		MapchipDefinition(int id){ this-> id = id; }
		int id;
		int image;
		int resistance;
	};

	struct Mapchip{
		Mapchip(){
			is_brighting = false;
			object = nullptr;
		}
		MapchipDefinition* definition;
		int height;
		int bright_color;
		bool is_brighting;
		BaseObject* object;
	};

	vector<MapchipDefinition> mapchipDefinition;
	static Mapchip mapchip[100][100];	//予め100x100のメモリを確保
};