#include <bits/stdc++.h>
#include <windows.h>
#include "include/basic.hpp"
#include <CLI11/CLI11.hpp>
using namespace std;
int main(int argc, char* argv[]) {
	CLI::App app{"This is a test application."};
	
	std::string name;
	int age;
	
	app.add_option("-n,--name", name, "Your name")->required();
	app.add_option("-a,--age", age, "Your age")->required();
	
	CLI11_PARSE(app, argc, argv);
	
	std::cout << "Hello, " << name << "! You are " << age << " years old." << std::endl;
////	cout << ImageSRBasic::FileConfig::getModelType("realesrgan-anime") << "\n";
//	using namespace std::filesystem;
////	system("chcp 65001 > nul");
//	path p = u8path(u8"中文测试");
//	cout << is_directory(p) << "\n";
//	for (directory_iterator it = directory_iterator(p); it != directory_iterator(); it++) {
//		cout << it->path() << "\n";
//		puts("Yes");
//	}
//	cout << p << "\n";
	return 0;
}
