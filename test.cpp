#include <bits/stdc++.h>
#include <windows.h>
#include "include/basic.hpp"
using namespace std;
int main() {
//	cout << ImageSRBasic::FileConfig::getModelType("realesrgan-anime") << "\n";
	using namespace std::filesystem;
	_wchdir(L"D:\\");
	cout << current_path()  << "\n";
	path p("output\\out sdp");
	cout << p << "\n";
	cout << p.string() << "\n";
	cout << absolute(p) << "\n";
	cout << exists(p) << "\n";
	cout << is_regular_file(p) << "\n";
	cout << is_directory(p) << "\n";
	return 0;
}
