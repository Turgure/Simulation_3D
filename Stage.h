#pragma once
#include <vector>
//#include "Position.h"
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
	void drawBrightPoints();
	void lateUpdate();

	static void setBrightPoint(int x, int y, int color);
	static bool getBrightPoint(int x, int y);
	static void eraseBrightPoint(int x, int y);
	static void eraseBrightPoints();

	static void setObjectAt(int x, int y, BaseObject* obj);
	static BaseObject* getObjectAt(int x, int y);
	static bool canMove(int x, int y);
	static int getResistance(int x, int y);

	static int getWidth(){ return width; }
	static int getDepth(){ return depth; }
	static int getHeight(int x, int y){ return mapchip[y][x].height; }
	static int getLeftupPositionX(){ return leftup_positionX; }
	static int getLeftupPositionY(){ return leftup_positionY; }

private:
	int current_map;

	static int width;
	static int depth;
	static int leftup_positionX;
	static int leftup_positionY;

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