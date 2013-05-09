#pragma once

class Keyboard{
public:
	static void initialize();
	static int update();

	static bool pushed(int keyID);
	static bool pushed(int keyID, int skipTo);
	static bool pushing(int keyID);
	static bool released(int keyID);
	
	static bool pushingUntil(int keyID, int frame);
	static bool pushingPer(int keyID, int frame);
	
	static int getFrame(int keyID);

private:
	static int key[256];
	static int prevkey[256];
};
