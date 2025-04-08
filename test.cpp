#define TEST
#include <bits/stdc++.h>
#include <windows.h>
#include "include/basic.hpp"
using namespace std;
int main() {
//	cout << ImageSRBasic::FileConfig::getModelType("realesrgan-anime") << "\n";
	using namespace std::filesystem;
	path p("output\\out sdp");
	p = weakly_canonical(p);
	cout << p << "\n";
	
	return 0;
}
