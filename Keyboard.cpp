#include "DxLib.h"
#include "Keyboard.h"

int Keyboard::key[256];

void Keyboard::initialize(){
	for(int i = 0; i < 256; i++) key[i] = 0;
}

int Keyboard::update(){
	char tmpKey[256];
	GetHitKeyStateAll(tmpKey);
	for(int i = 0; i < 256; i++){
		if(tmpKey[i] != 0){//i�Ԃ̃L�[�������ꂽ��
			key[i]++;
		} else {
			key[i] = 0;
		}
	}
	return 0;
}


int Keyboard::get(int keyID){
	return key[keyID];
}