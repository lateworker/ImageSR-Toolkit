#include <bits/stdc++.h>
#include "include/basic.hpp"
#include "include/CLI11.hpp"

using namespace std;
using namespace ImageSRBasic;

filesystem::path FileConfig::ROOTPATH = filesystem::current_path();

#define Waifu2x_Anime L"models-upconv_7_anime_style_art_rgb"
#define Waifu2x_Photo L"models-upconv_7_photo"
void FileConfig::processAsFile() const {
	if (exists(outputPath) && !isForced) {
		// give an option later.
		return;
	}
	create_directories(outputPath.parent_path());
	wstring command;
	switch (getCoreType()) {
		case REALESR: {
			const auto& [model, scale] = get<REALESR>(coreConfig);
			command = (ROOTPATH / "models" / "realesrgan.exe").wstring()
			          + L" -i " + ImageSRBasic::quote(inputPath.wstring())
			          + L" -o " + ImageSRBasic::quote(outputPath.wstring())
			          + L" -n " + ImageSRBasic::quote(model + L"-x" + scale)
			          + L" -s " + scale;
			break;
		}
		case WAIFU2X: {
			const auto& [model, scale, denoise] = get<WAIFU2X>(coreConfig);
			command = (ROOTPATH / "models" / "waifu2x.exe").wstring()
			          + L" -i " + ImageSRBasic::quote(inputPath.wstring())
			          + L" -o " + ImageSRBasic::quote(outputPath.wstring())
			          + L" -m " + ImageSRBasic::quote(model == L"waifu2x-anime" ? Waifu2x_Anime : Waifu2x_Photo)
			          + L" -s " + scale
			          + L" -n " + denoise;
			break;
		}
		case REALCUGAN: {
			const auto& [model, scale, denoise, syncgap] = get<REALCUGAN>(coreConfig);
			command = (ROOTPATH / "models" / "realcugan.exe").wstring()
			          + L" -i " + ImageSRBasic::quote(inputPath.wstring())
			          + L" -o " + ImageSRBasic::quote(outputPath.wstring())
			          + L" -s " + scale
			          + L" -n " + denoise
			          + L" -c " + syncgap;
			break;
		}
		default: return;
	}
	ImageSRBasic::execute(command);
}
void Config::processAsDir() const {
	for (recursive_directory_iterator it = recursive_directory_iterator(inputPath); it != recursive_directory_iterator(); it++) {
		if (it->is_directory()) {
			if (!isRecursive) it.disable_recursion_pending(); // if disabled recursion, break out when meeting a folder.
			continue; // when meeting a folder, do nothing and continue.
		}
		// check selector
		if (!selector.empty() && !selector.count(it->path().extension().wstring())) continue;

		FileConfig file;
		file.setInputPath(it->path());
		file.setOutputPath(outputPath / relative(it->path(), inputPath));
		file.setCoreConfig(this->getCoreConfig());
		file.processAsFile();
	}
}
void Config::process() const {
	if (isDir) processAsDir();
	if (isFile) processAsFile();
}

Config target;
CLI::App app("ImageSR-Tookit");

int parseCommandLine(int argc, char* argv[]) {
	// inputPath
	std::wstring inputPathStr;
	app.add_option("-i,--input", inputPathStr, "Input path, an existing file or directory")
	->required()->check(CLI::ExistingPath);
	// outputPath
	std::wstring outputPathStr;
	app.add_option("-o,--output", outputPathStr, "Output path, the same pathtype as input path")
	->required();
	// modelConfig
//	std::vector<std::wstring> modelConfig;
//	wstring model,
//	        app.add_option("-m,--model",, "Model configuration")
//	        // isForced
//	        bool isForced = false;
//	app.add_flag("-f,--force", isForced, "Whether overwriting output path in force");
//
//	// 选择器
//	std::vector<std::wstring> selector;
//	app.add_option("--selector", selector, "File selector");

//	// 递归模式
//	bool recursive = true;
//	app.add_flag("--recursive", recursive, "Recursive processing");
//	app.add_flag("--no-recursive", recursive, "Non-recursive processing");

	CLI11_PARSE(app, argc, argv);

	// apply to the target
	target.setInputPath(inputPathStr);
	target.setOutputPath(outputPathStr);
	return 0;
}

int main(int argc, char* argv[]) {
	system("chcp 65001 > nul");
//	argv = app.ensure_utf8(argv);
//	parseCommandLine(argc, argv);
	target.setInputPath(L"ImageTest\\输入文件夹");
	target.setOutputPath(L"ImageTest\\输出文件夹");
	target.setCoreModel(L"realesrgan-anime");
	target.setCoreScale(L"2");
//	target.unsetRecursive();
	target.process();
	return 0;
}
