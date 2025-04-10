#include <bits/stdc++.h>
#include "include/basic.hpp"

using namespace std;
using namespace ImageSRBasic;

filesystem::path FileConfig::ROOTPATH = filesystem::current_path();

#define Waifu2x_Anime "models-upconv_7_anime_style_art_rgb"
#define Waifu2x_Photo "models-upconv_7_photo"
void FileConfig::processAsFile() const {
	create_directories(outputPath.parent_path());
	string command;
	switch (getModelType()) {
		case 1 : {
			command = (ROOTPATH / "models" / "realesrgan.exe").string()
			          + " -i " + ImageSRBasic::quote(inputPath.string())
			          + " -o " + ImageSRBasic::quote(outputPath.string())
			          + " -n " + ImageSRBasic::quote(model + "-x" + scale)
			          + " -s " + scale;
			break;
		}
		case 2 : {
			command = (ROOTPATH / "models" / "waifu2x.exe").string()
			          + " -i " + ImageSRBasic::quote(inputPath.string())
			          + " -o " + ImageSRBasic::quote(outputPath.string())
			          + " -m " + ImageSRBasic::quote(model == "waifu2x-anime" ? Waifu2x_Anime : Waifu2x_Photo)
			          + " -s " + scale
			          + " -n " + denoise;
			break;
		}
		case 3 : {
			command = (ROOTPATH / "models" / "realcugan.exe").string()
			          + " -i " + ImageSRBasic::quote(inputPath.string())
			          + " -o " + ImageSRBasic::quote(outputPath.string())
			          + " -s " + scale
			          + " -n " + denoise
			          + " -c " + syncgap;
			break;
		}
	}
	ImageSRBasic::execute(command);
}
void Config::processAsDir() {
	for (recursive_directory_iterator it = recursive_directory_iterator(inputPath); it != recursive_directory_iterator(); it++) {
		if (it->is_directory()) {
			if (!recursive) it.disable_recursion_pending(); // if disabled recursion, break out when meeting a folder.
			continue; // when meeting a folder, do nothing and continue.
		}
		FileConfig file;
		file.setInputPath(it->path());
		file.setOutputPath(this->outputPath / relative(it->path(), this->inputPath));
		file.setModelInfo(this->getModelInfo());
		file.processAsFile();
	}
}
void Config::process() {
	if (isDir) processAsDir();
	if (isFile) processAsFile();
}

int main() {
	system("chcp 65001 > nul"); // temp
	Config x;
	cout << x.setInputPath("ImageTest\\In") << "\n";
	cout << x.setOutputPath("ImageTest\\Out") << "\n";
	x.setModelInfo({"realesrgan-anime", "2"});
	x.unsetRecursive();
	x.process();
	return 0;
}
