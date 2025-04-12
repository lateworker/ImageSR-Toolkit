#include <bits/stdc++.h>
#include "include/basic.hpp"
#include "include/CLI11.hpp"

using namespace std;
using namespace ImageSRBasic;

filesystem::path FileConfig::ROOTPATH = filesystem::current_path();

#define Waifu2x_Anime L"models-upconv_7_anime_style_art_rgb"
#define Waifu2x_Photo L"models-upconv_7_photo"
void FileConfig::processAsFile() const {
	create_directories(outputPath.parent_path());
	wstring command;
	switch (getModelType()) {
		case 1 : {
			command = (ROOTPATH / "models" / "realesrgan.exe").wstring()
			          + L" -i " + ImageSRBasic::quote(inputPath.wstring())
			          + L" -o " + ImageSRBasic::quote(outputPath.wstring())
			          + L" -n " + ImageSRBasic::quote(model + L"-x" + scale)
			          + L" -s " + scale;
			break;
		}
		case 2 : {
			command = (ROOTPATH / "models" / "waifu2x.exe").wstring()
			          + L" -i " + ImageSRBasic::quote(inputPath.wstring())
			          + L" -o " + ImageSRBasic::quote(outputPath.wstring())
			          + L" -m " + ImageSRBasic::quote(model == L"waifu2x-anime" ? Waifu2x_Anime : Waifu2x_Photo)
			          + L" -s " + scale
			          + L" -n " + denoise;
			break;
		}
		case 3 : {
			command = (ROOTPATH / "models" / "realcugan.exe").wstring()
			          + L" -i " + ImageSRBasic::quote(inputPath.wstring())
			          + L" -o " + ImageSRBasic::quote(outputPath.wstring())
			          + L" -s " + scale
			          + L" -n " + denoise
			          + L" -c " + syncgap;
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

Config target;

int main(int argc, char* argv[]) {
//	CLI::App argument("ImageSR-Tookit");
//	argument.add_option("");
	target.setInputPath(L"ImageTest\\In\\input.jpg");
	target.setOutputPath(L"ImageTest\\Out\\output.jpg");
	target.setModelInfo({L"realesrgan-anime", L"2"});
	target.process();
	return 0;
}
