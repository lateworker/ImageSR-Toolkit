#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

std::string narrow(const std::wstring& s) {
	return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> >().to_bytes(s);
}
void outWstring(const wstring& str) { WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), NULL, NULL); }

int main() {
	system("chcp 65001 > nul");
	
	cout << narrow(L"你好，世界！") << "\n";
	outWstring(L"你好，窗口！");
	
	return 0;
}
