#include <bits/stdc++.h>
#include "include/basic.hpp"
//#include "include/CLI11.hpp"
using namespace std;
using namespace filesystem;
using namespace ImageSRBasic;
void outWstring(const wstring& str) { WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), NULL, NULL); }

int main() {
	system("chcp 65001 > nul");
	cout << getExecPath().parent_path() << "\n";
	return 0;
}
