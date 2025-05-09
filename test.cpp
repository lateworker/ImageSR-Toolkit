#include <bits/stdc++.h>
#include "include/basic.hpp"
//#include "include/CLI11.hpp"
using namespace std;
using namespace filesystem;
using namespace ImageSRBasic;
void outWstring(const wstring& str) { WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), NULL, NULL); }

// rootpath: root of the project.
filesystem::path FileConfig::ROOTPATH = getExecPath().parent_path();
// maxlength: max length of the tmpdir name to random.
size_t VideoMan::MAXLENGTH = 16;
// tmpdirroot: parent path of the tmpdirs.
filesystem::path VideoMan::TMPDIRROOT = filesystem::temp_directory_path() / "ImageSR_Toolkit";

string generate_random_string(size_t length) {
	const static string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	random_device randomDevice;
	mt19937 generator(randomDevice());
	uniform_int_distribution<> distribution(0, characters.size() - 1);
	std::string result;
	for (size_t i = 0; i < length; ++i)
		result += characters[distribution(generator)];
	return result;
}

int main() {
	VideoMan x;
	cout << x.getTmpdirPath() << "\n";
	return 0;
}
