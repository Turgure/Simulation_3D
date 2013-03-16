#pragma once

//シーン基底クラス
//全シーンクラスの基底クラス。このクラスのmain()を起動することで場面が切り替わる。
class BaseScene{
public:
	BaseScene(){};
	virtual ~BaseScene(){};
	BaseScene* main();

protected:
	virtual void initialize(){};
	virtual void finalize(){};
	virtual void update(){};
	virtual void draw(){};
	void changeScene(BaseScene* next_scene);

private:
	int processLoop();
	BaseScene* next_scene;
	bool looping;	//ループ中フラグ
	static bool running;
};

//シーン管理クラス
//シーンの遷移と実行を管理するクラス
class SceneManager{
	static bool running;
	static BaseScene* current_scene;

public:
	static void run(BaseScene* starter);
};