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
		if (!this->selector.empty() && !this->selector.count(it->path().extension().wstring())) continue;
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
CLI::App app("ImageSR-Tookit");

int parseCommandLine(int argc, char* argv[]) {
	// inputPath
	std::wstring inputPathStr;
	app.add_option("-i,--input", inputPathStr, "Input Path")
	->required()->check(CLI::ExistingPath);
	// outputPath
	std::wstring outputPathStr;
	app.add_option("-o,--output", outputPathStr, "Output Path")
	->required();
//	// 模型参数
//	std::vector<std::wstring> modelConfig;
//	app.add_option("-m,--model", modelConfig, "Model configuration")
//	->default_val<std::vector<std::wstring>>({});

//	// 强制模式
//	bool isForced = false;
//	app.add_flag("--force", isForced, "Force processing");
//
//	// 选择器
//	std::vector<std::wstring> selector;
//	app.add_option("--selector", selector, "File selector");

//	// 递归模式
//	bool recursive = true;
//	app.add_flag("--recursive", recursive, "Recursive processing");
//	app.add_flag("--no-recursive", recursive, "Non-recursive processing");

	// 解析命令行参数
	CLI11_PARSE(app, argc, argv);

	// 设置Config对象
	target.setInputPath(inputPathStr);
	target.setOutputPath(outputPathStr);
	return 0;
}

int main(int argc, char* argv[]) {
	argv = app.ensure_utf8(argv);
	parseCommandLine(argc, argv);
	target.process();
	return 0;
}
