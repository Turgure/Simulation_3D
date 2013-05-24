#pragma once

class Sound{
public:
	static void initialize();
	
	static int start_scene;
	static int battle_scene[5];
	static int gameover_scene;
	static int result_scene;

	static int step;
	static int decide;
	static int cancel;
	static int select;
};
