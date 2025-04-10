#include <bits/stdc++.h>
#include <windows.h>
#include "include/basic.hpp"
using namespace std;
int main() {
//	cout << ImageSRBasic::FileConfig::getModelType("realesrgan-anime") << "\n";
	using namespace std::filesystem;
//	system("chcp 65001 > nul");
	path p = u8path(u8"中文测试");
	cout << is_directory(p) << "\n";
	for (directory_iterator it = directory_iterator(p); it != directory_iterator(); it++) {
		cout << it->path() << "\n";
		puts("Yes");
	}
	cout << p << "\n";
	return 0;
}
