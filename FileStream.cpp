#include <iostream>
#include <cstdlib>
#include <fstream>
#include "FileStream.h"

void FileStream::load(const string& filename, vector<string>& data){
	string str;
	int p;

	ifstream file(filename);
	if(file.fail()){
		cout << filename.c_str() << " load error." << endl;
		exit(1);
	}
	
	while(getline(file, str)){
	    //コメント箇所は除く
	    if((p = str.find("//")) != str.npos) continue;
	    //コンマがあるかを探し、そこまでをvaluesに格納
	    while((p = str.find(",")) != str.npos){
	        data.push_back(str.substr(0, p));
	
	        //strの中身は", "の2文字を飛ばす
	        str = str.substr(p+2);
	    }
	    data.push_back(str);
	}
}

void FileStream::load(const string& filename, vector<vector<string>>& data){
	string str;
	int p;
	vector<string> inner;

	ifstream file(filename);
	if(file.fail()){
		cout << filename.c_str() << " load error." << endl;
		exit(1);
	}
	
	while(getline(file, str)){
	    //コメント箇所は除く
	    if((p = str.find("//")) != str.npos) continue;
	    vector<string> inner;
	
	    //コンマがあるかを探し、そこまでをvaluesに格納
	    while((p = str.find(",")) != str.npos){
	        inner.push_back(str.substr(0, p));
	
	        //strの中身は", "の2文字を飛ばす
	        str = str.substr(p+2);
	    }
	    inner.push_back(str);
	    data.push_back(inner);
	}
}