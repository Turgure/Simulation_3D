#pragma once
#include "DxLib.h"

static int key[256];

class Keyboard{
public:
	static void initialize(){
		for(int i = 0; i < 256; i++) key[i] = 0;
	}

	static int update(){
		char tmpKey[256];
		GetHitKeyStateAll(tmpKey);
		for(int i = 0; i < 256; i++){
			if(tmpKey[i] != 0){//i番のキーが押されたら
				key[i]++;
			} else {
				key[i] = 0;
			}
		}
		return 0;
	}

	static int get(int keyID){
		return key[keyID];
	}
};
