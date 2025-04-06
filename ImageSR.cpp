#include <bits/stdc++.h>
#include "include/basic.hpp"

using namespace std;
using namespace ImageSRBasic;

#define Waifu2x_Anime "models-upconv_7_anime_style_art_rgb"
#define Waifu2x_Photo "models-upconv_7_photo"
void FileConfig::processAsFile() const {
	string command;
	switch (getModelType()) {
		case 1 : {
			command = (string)"models\\realesrgan.exe"
			          + " -i " + ImageSRBasic::quote(inputPath)
			          + " -o " + ImageSRBasic::quote(outputPath)
			          + " -n " + ImageSRBasic::quote(model + "-x" + scale)
			          + " -s " + scale;
			break;
		}
		case 2 : {
			command = (string)"models\\waifu2x.exe"
			          + " -i " + ImageSRBasic::quote(inputPath)
			          + " -o " + ImageSRBasic::quote(outputPath)
			          + " -m " + ImageSRBasic::quote(model == "waifu2x-anime" ? Waifu2x_Anime : Waifu2x_Photo)
			          + " -s " + scale
			          + " -n " + denoise;
			break;
		}
		case 3 : {
			command = (string)"models\\realcugan.exe"
			          + " -i " + ImageSRBasic::quote(inputPath)
			          + " -o " + ImageSRBasic::quote(outputPath)
			          + " -s " + scale
			          + " -n " + denoise
			          + " -c " + syncgap;
			break;
		}
	}
//	chdir(selfPath.c_str());	// 不知道是哪里出问题了, 这里必须手动切换目录.
//	executefile(command);
//	cout << command << "\n";
	ImageSRBasic::execute(command);
}

int main() {
	FileConfig x;
	x.setInputPath("input.jpg");
	x.setOutputPath("output");
	x.setModelInfo({"realesrgan-anime", "2", "0"});
	x.process_file();
	return 0;
}
