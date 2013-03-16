#pragma once
#include <vector>
#include <string>
#include "GV.h"
#include "ObjectStatus.h"
using namespace std;

//オブジェクトの基底クラス
class BaseObject{
public:
	BaseObject(){};
	virtual ~BaseObject(){};

	virtual void stepATBgauge(){};
	bool isMyTurn();


protected:
	virtual void update(){};
	virtual void draw(){};
	virtual void action(){};
	virtual void endMyTurn(){};

	enum State{SELECT, MOVE, ACTION, END, WAIT} state;
	int ATBgauge;
	bool can_move;
	bool can_act;

	struct Status{
		void showStatus(int x, int z) const;

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
		int image;
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

public:
	//0=>SELECT, 1=>MOVE, ACTION=>2, 3=>END, 4=>WAIT
	State getState() const { return state; }
};

class Player;
class Enemy;
//オブジェクト管理クラス
//生成等を行う
class ObjectManager{
public:
	static void create(vector<Player> &players, string filename, int x, int z);
	static void create(vector<Enemy> &enemies, string filename);
};

//プレイヤークラス
class Player : public BaseObject, public BaseObject::Status{
public:
	Player(int x, int z, int id, int hp, int mp, int str, int def, int agi, int mobility);
	virtual void update() override;
	virtual void draw() override;
	virtual void action() override;
	virtual void endMyTurn() override;
	virtual void stepATBgauge() override;

	void showCommand();

	void attack(vector<Enemy>& enemies);

	Status status;
};


//エネミークラス
class Enemy : public BaseObject, public BaseObject::Status{
public:
	Enemy(int x, int z, int id, int hp, int mp, int str, int def, int agi, int mobility);
	virtual void update() override;
	virtual void draw() override;
	virtual void action() override;
	virtual void endMyTurn() override;
	virtual void stepATBgauge() override;

	bool isCountOver(int time);
	void calcMove(vector<Player>& players);
	void calcAttack(vector<Player>& players);
	void attack(vector<Player>& players);

	Status status;

private:
	int wait_time;
	int attack_range;
	bool moved;
	bool attacked;
};
