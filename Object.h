#pragma once
#include <vector>
#include <string>
#include "DxLib.h"
#include "GV.h"
#include "Position.h"
#include "Command.h"
using namespace std;

//オブジェクトの基底クラス
class BaseObject{
public:
	BaseObject(){};
	virtual ~BaseObject(){};

	void lateUpdate();
	virtual void stepATBgauge(){};
	bool isMyTurn();

	enum State{SELECT, MOVE, ACTION, ATTACK, END, WAIT, MOVING, ATTACKING} state;

protected:
	virtual void update(){};
	virtual void draw(){};
	virtual void action(){};
	virtual void endMyTurn(){};
	virtual void resetATBgauge(){};

	int ATBgauge;
	bool can_move;
	bool can_act;
	bool has_brightened;

	bool changeState(State& mystate, State next);
	bool state_changed;

	vector<int> model;
	int attack_effect[10];
	int attack_status;

	struct Status{
		void showStatus() const;
		
		string getName(){ return name; }
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
		void setMob(int mob){ mobility = mob; }
		int getMob() const { return mobility; }
		void setJumpPow(int pow){ jump_power = pow; }
		int getJumpPow() const { return jump_power; }

		void setDamage(int damage){ this->damage = damage; has_attacked = true; }
		int getDamage() const { return damage; }

	protected:
		string name;
		int maxhp;
		int maxmp;
		int hp;
		int mp;
		int str;
		int def;
		int agi;
		int mobility;
		int jump_power;
	
		int damage;
		bool has_attacked;
	};

	struct MoveManager{
		MoveManager(){
			dir[NORTH] = Position( 0, -1);
			dir[SOUTH] = Position( 0,  1);
			dir[WEST]  = Position(-1,  0);
			dir[EAST]  = Position( 1,  0);

			moving_rate = 1.0/20;	//0.05
			diff = VGet(0.0f,0.0f,0.0f);

			jump_path = NULL;
			jump_height = chipheight;
		}
		//horizontal
		void trackMovement(const Position& pos, const Position& topos, int mob, BaseObject* obj);
		void initialize();
		void calcShortestPath(const Position& pos, const Position& topos, int mob, BaseObject* obj);
		Position dir[4];
		vector<int> path;
		
		void move();
		void setObjectDirection(int model, int dir);
		void setObjectDirection(int model, const Position& dirpos);
		void setObjectDirection(int model);
		int current_dir;
		double moving_rate;
		VECTOR diff;

		//vertical
		void initJumpmotion(const Position& pos, const Position& topos);

		enum Jump{UP, DOWN} jump;
		int step;
		int jump_height;
		int jump_dist;
		double jump_path;

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
	static void create(vector<Player> &players, const string& filename);
	static void create(vector<Enemy> &enemies, const string& filename);
};

//プレイヤークラス
class Player : public BaseObject, public BaseObject::Status{
public:
	Player(string name, int x, int y, int hp, int mp, int str, int def, int agi, int mobility, int jump_power);
	virtual void update() override;
	virtual void draw() override;
	virtual void action() override;
	virtual void endMyTurn() override;
	virtual void stepATBgauge() override;
	virtual void resetATBgauge() override;

	void drawCommand();
	void attack(vector<Enemy>& enemies);
	bool assignDirection();

	Status status;
	Position pos;

	MoveManager mv_mng;
private:
	int picture_frame;

	CommandSelect command;
//	MoveManager mv_mng;
	VECTOR myvec;
};


//エネミークラス
class Enemy : public BaseObject, public BaseObject::Status{
public:
	Enemy(string name, int x, int y, int hp, int mp, int str, int def, int agi, int mobility, int jump_power);
	virtual void update() override;
	virtual void draw() override;
	virtual void action() override;
	virtual void endMyTurn() override;
	virtual void stepATBgauge() override;
	virtual void resetATBgauge() override;

	void calcMove(const vector<Player>& players);
	void calcAttack(const vector<Player>& players);
	void attack(vector<Player>& players);
	void assignDirection(const vector<Player>& players);

	Status status;
	Position pos;

private:
	bool isCountOver(int time);

	int wait_time;
	int attack_range;
	bool moved;
	bool attacked;
	vector<int> model;
	int attackeffect[10];

	MoveManager mv_mng;
	VECTOR myvec;
	Position move_pos;
	Position act_pos;
};
