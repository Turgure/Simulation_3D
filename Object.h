#pragma once
#include <vector>
#include <string>
#include "GV.h"
#include "Position.h"
#include "Command.h"
using namespace std;

//オブジェクトの基底クラス
class BaseObject{
public:
	BaseObject(){};
	virtual ~BaseObject(){};

	virtual void stepATBgauge(){};
	bool isMyTurn();

	enum State{SELECT, MOVE, ACTION, END, WAIT, MOVING} state;

protected:
	virtual void update(){};
	virtual void draw(){};
	virtual void action(){};
	virtual void endMyTurn(){};

	int ATBgauge;
	bool can_move;
	bool can_act;

	struct Status{
		void showStatus(int x, int y) const;

		void setHP(int hp){ this->hp = hp; }
		int getHP() const { return hp; }
		int getMaxHP() const { return maxhp; }
		void setMP(int mp){ this->mp = mp; }
		int getMP() const { return mp; }
		int getMaxMP() const { return maxmp; }
		void setStr(int str){ this->str = str; }
		int getStr() const { return str; }
		void setDef(int def){ this->def = def; }
		int getDef() const { return def; }
		void setAgi(int agi){ this->agi = agi; }
		int getAgi() const { return agi; }
		void setMob(int mob){ this->mobility = mob; }
		int getMob() const { return mobility; }

	protected:
		int id;
		int model;
		string name;
		int maxhp;
		int maxmp;
		int hp;
		int mp;
		int str;
		int def;
		int agi;
		int mobility;
	};

	struct MovingManager{
		MovingManager(){
			dir[NORTH] = Position( 0, -1);
			dir[SOUTH] = Position( 0,  1);
			dir[WEST]  = Position(-1,  0);
			dir[EAST]  = Position( 1,  0);

			moving_rate = 0.05;	//1/20
			diff = VGet(0.0f,0.0f,0.0f);
		}
		void trackMovement(const Position& pos, const Position& topos, int mob);
		void initialize();
		void calcShortestPath(const Position& pos, const Position& topos, int mob);
		enum Direction{NORTH, SOUTH, WEST, EAST, DIR_NUM};
		Position dir[4];
		vector<int> path;
		
		void move();
		void setObjectDirection(int MHandle);
		int current_dir;
		double moving_rate;
		VECTOR diff;
	private:
		vector<int> current_path;
		vector<int> shortest_path;

	};
};

class Player;
class Enemy;
//オブジェクト管理クラス
//生成等を行う
class ObjectManager{
public:
	static void create(vector<Player> &players, const string& filename, int x, int y);
	static void create(vector<Enemy> &enemies, const string& filename);
};

//プレイヤークラス
class Player : public BaseObject, public BaseObject::Status{
public:
	Player(int x, int y, int id, int hp, int mp, int str, int def, int agi, int mobility);
	virtual void update() override;
	virtual void draw() override;
	virtual void action() override;
	virtual void endMyTurn() override;
	virtual void stepATBgauge() override;

	void showCommand();

	void attack(vector<Enemy>& enemies);

	Status status;
	CommandSelect command;
	Position pos;

private:
	MovingManager mv_mng;

	VECTOR myvec;
};


//エネミークラス
class Enemy : public BaseObject, public BaseObject::Status{
public:
	Enemy(int x, int y, int id, int hp, int mp, int str, int def, int agi, int mobility);
	virtual void update() override;
	virtual void draw() override;
	virtual void action() override;
	virtual void endMyTurn() override;
	virtual void stepATBgauge() override;

	bool isCountOver(int time);
	void calcMove(const vector<Player>& players);
	void calcAttack(const vector<Player>& players);
	void attack(vector<Player>& players);

	Status status;
	Position pos;

private:
	int wait_time;
	int attack_range;
	bool moved;
	bool attacked;
	MovingManager mv_mng;

	VECTOR myvec;
	Position move_pos;
	Position act_pos;
};
