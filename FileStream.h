#pragma once
#include <string>
#include <vector>
using namespace std;

class FileStream{
public:
	static void load(string filename, vector<string>& data);
	static void load(string filename, vector<vector<string>>& data);

	static void write();
};