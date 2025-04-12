#include <bits/stdc++.h>
#include <windows.h>
#include "include/basic.hpp"
#include "include/CLI11.hpp"
using namespace std;
wstring proexe = L"D:\\CProjects\\ImageSR-Toolkit\\models\\realesrgan.exe";
wstring proarg = L"-i \"D:\\\\CProjects\\\\ImageSR-Toolkit\\\\中文测试\\\\输入.jpg\" -o \"D:\\\\CProjects\\\\ImageSR-Toolkit\\\\ImageTest\\\\输出\\\\output.jpg\" -n \"realesrgan-anime-x2\" -s 2";
void outWstring(const wstring& str) { WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), NULL, NULL); }

int main(int argc, char* argv[]) {
//int main() {
	system("chcp 65001 > nul");
//	CLI::App app{"This is a test application."};
//	argv = app.ensure_utf8(argv);
//
//	std::wstring str;
//	int age;
//
//	app.add_option("-n,--name", str, "Your name")->required();
//	app.add_option("-a,--age", age, "Your age")->required();
//
//	CLI11_PARSE(app, argc, argv);
//
////	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), NULL, NULL);
//	cout << ImageSRBasic::narrow(str);
//	cout << "\n" << age << "\n";
//	cout << ImageSRBasic::FileConfig::getModelType("realesrgan-anime") << "\n";
//	using namespace std::filesystem;
//	wstring s = L"中文测试";
//	path p = s;
//	cout << is_directory(p) << "\n";
//	for (directory_iterator it = directory_iterator(p); it != directory_iterator(); it++) {
////		cout << it->path() << "\n";
//		wstring str = it->path().wstring();
//		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), NULL, NULL);
//		cout << "\n";
//	}
	
//	ImageSRBasic::execute(proexe + L" " + proarg);
	ImageSRBasic::execute(L"t.exe");
	
	return 0;
}
