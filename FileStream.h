#pragma once
#include <string>
#include <vector>
using namespace std;

class FileStream{
public:
	static void load(const string& filename, vector<int>& data);
	static void load(const string& filename, vector<vector<int>>& data);
	static void loadCSV(const string& filename, vector<string>& data);
	static void loadCSV(const string& filename, vector<vector<string>>& data);

	static void write(const string& filename, vector<int>& data);
	static void write(const string& filename, vector<vector<int>>& data);
	static void writeCSV(const string& filename, vector<string>& data);
	static void writeCSV(const string& filename, vector<vector<string>>& data);
};