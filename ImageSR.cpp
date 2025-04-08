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
	current_path(ROOTPATH);
	cout << command << "\n"; // testout
	ImageSRBasic::execute(command);
}
void Config::processAsDir() {
	// Test
	cout << inputPath << "\n";
	for (recursive_directory_iterator it = recursive_directory_iterator(inputPath); it != recursive_directory_iterator(); it++) {
		path now = it->path();
		if (now.filename() == ".git") {
			cout << now << " Here\n";
			it.disable_recursion_pending();
		}
		else cout << now << "\n";
	}
}
void Config::process() {
	if (isDir) processAsDir();
	if (isFile) processAsFile();
}

int main() {
	Config x;
	cout << x.setInputPath("ImageTest\\input.jpg") << "\n";
	cout << x.setOutputPath("ImageTest\\sdf\\sdfaa\\utput.jpg") << "\n";
	x.setModelInfo({"realesrgan-anime", "2"});
	x.process();
	return 0;
}
