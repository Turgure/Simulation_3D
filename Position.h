#pragma once
#include <cmath>

class Position{
public:
	int x, y;
	Position():x(0), y(0){};
	Position(int x, int y):x(x), y(y){};
	Position(const Position& pos):x(pos.x), y(pos.y){};

	//比較
	bool operator==(const Position& pos) const{
		return (x == pos.x && y == pos.y);
	}
	bool operator!=(const Position& pos) const{
		return (x != pos.x || y != pos.y);
	}
	
	//代入
	Position& operator=(const Position& pos){
		x = pos.x;
		y = pos.y;
		return *this;
	}

	//加減算
	Position operator+(const Position& pos) const{
		return Position(x + pos.x, y + pos.y);
	}
	Position& operator+=(const Position& pos){
		x += pos.x;
		y += pos.y;
		return *this;
	}
	Position operator-(const Position& pos) const{
		return Position(x - pos.x, y - pos.y);
	}
	Position& operator-=(const Position& pos){
		x -= pos.x;
		y -= pos.y;
		return *this;
	}

	int getX() const{ return x; }
	int getY() const{ return y; }
	void set(int toX, int toY){
		x = toX;
		y = toY;
	}
	void Move(int diffX, int diffY){
		x += diffX;
		y += diffY;
	}
	int getDist(const Position& pos, const Position& topos) const{
		return (abs(pos.x - topos.x) + abs(pos.y - topos.y));
	}
};