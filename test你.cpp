#include <bits/stdc++.h>
#include <windows.h>
#include <direct.h>
#include "include/basic.hpp"
//#include "include/CLI11.hpp"
using namespace std;
wstring proexe = L"D:\\CProjects\\ImageSR-Toolkit\\models\\realesrgan.exe";
wstring proarg = L"-i \"D:\\\\CProjects\\\\ImageSR-Toolkit\\\\中文测试\\\\输入.jpg\" -o \"D:\\\\CProjects\\\\ImageSR-Toolkit\\\\ImageTest\\\\输出\\\\output.jpg\" -n \"realesrgan-anime-x2\" -s 2";
void outWstring(const wstring& str) { WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), NULL, NULL); }
wstring wgetcwd() {
	wchar_t buffer[ImageSRBasic::BufferSize];
	if (_wgetcwd(buffer, sizeof buffer)) {
		return buffer;
	} else {
		
	}
	return L"";
}

int main(int x, char* y[]) {
	system("chcp 65001 > nul");
	cout << y[0] << "\n";
	return 0;
}
