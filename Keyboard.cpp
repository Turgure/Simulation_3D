#include <DxLib.h>
#include "Keyboard.h"

int Keyboard::key[256];
int Keyboard::prevkey[256];

void Keyboard::initialize(){
	for(int i = 0; i < 256; i++){
		key[i] = 0;
		prevkey[i] = 0;
	}
}

int Keyboard::update(){
	char tmpKey[256];
	GetHitKeyStateAll(tmpKey);
	for(int i = 0; i < 256; i++){
		prevkey[i] = key[i];

		if(tmpKey[i] != 0){//i番のキーが押されたら
			++key[i];
		} else {
			key[i] = 0;
		}
	}
	return 0;
}

bool Keyboard::pushed(int keyID, bool judge_once){
	if(prevkey[keyID] == 0 && key[keyID] == 1){
		if(judge_once) key[keyID] = INT_MIN;
		return true;
	}
	return false;
}

bool Keyboard::pushing(int keyID){
	return (prevkey[keyID] > 0 && key[keyID] > 0);
}

bool Keyboard::released(int keyID){
	return (prevkey[keyID] == 1 && key[keyID] == 0);
}

bool Keyboard::pushingUntil(int keyID, int frame){
	return (key[keyID] > frame);
}

bool Keyboard::pushingPer(int keyID, int frame){
	return (key[keyID] % frame == 0);
}

int Keyboard::getFrame(int keyID){
	return key[keyID];
}