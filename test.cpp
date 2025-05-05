#include <bits/stdc++.h>
#include <windows.h>
//#include "include/basic.hpp"
//#include "include/CLI11.hpp"
using namespace std;
wstring proexe = L"D:\\CProjects\\ImageSR-Toolkit\\models\\realesrgan.exe";
wstring proarg = L"-i \"D:\\\\CProjects\\\\ImageSR-Toolkit\\\\中文测试\\\\输入.jpg\" -o \"D:\\\\CProjects\\\\ImageSR-Toolkit\\\\ImageTest\\\\输出\\\\output.jpg\" -n \"realesrgan-anime-x2\" -s 2";
void outWstring(const wstring& str) { WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), NULL, NULL); }
std::string narrow(const std::wstring& s) {
	return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> >().to_bytes(s);
}
std::wstring widen(const std::string& s) {
	return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> >().from_bytes(s);
}
wstring execute(const std::wstring& cmd) {
	FILE* fp;
	fp = _wpopen(cmd.c_str(), L"r");
	string res;
	char MsgBuff[4096];
	memset(MsgBuff, 0, sizeof MsgBuff);
	while (fgets(MsgBuff, sizeof MsgBuff, fp)) res += MsgBuff;
	_pclose(fp);
	return widen(res);
}

int main() {
	system("chcp 65001 > nul");
	
	cout << narrow(execute(L"中文\\测试.exe"));

	return 0;
}
