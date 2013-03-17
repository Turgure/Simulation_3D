#pragma once
#include <cmath>

class Position{
public:
	int x, y;
	Position():x(0), y(0){};
	Position(int x, int y):x(x), y(y){};

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

	void setX(int toX){
		x = toX;
	}
	void setY(int toY){
		y = toY;
	}
	void set(int toX, int toY){
		x = toX;
		y = toY;
	}
	int getX() const{ return x; }
	int getY() const{ return y; }
	void Move(int diffX, int diffY){
		x += diffX;
		y += diffY;
	}
	int getDist(int x, int y, int toX, int toY) const{
		return (abs(x - toX) + abs(y - toY));
	}
};