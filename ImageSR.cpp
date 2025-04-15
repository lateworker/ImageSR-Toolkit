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
		if (!extSelector.empty() && !extSelector.count(it->path().extension().wstring())) continue;

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
	app.add_option("-i,--input", inputPathStr, "Input path, an existing file or directory") -> required() -> check(CLI::ExistingPath);
	// outputPath
	std::wstring outputPathStr;
	app.add_option("-o,--output", outputPathStr, "Output path, the same pathtype as input path") -> required();
	
	// coreModel
	std::wstring coreModel;
	app.add_option("-m,--model", coreModel, "Core model") -> required();
	// coreScale
	int coreScale;
	app.add_option("-s,--scale", coreScale, "Core scale");
	// coreDenoise
	int coreDenoise;
	app.add_option("-d,--denoise", coreDenoise, "Core denoise");
	// coreSyncgap
	int coreSyncgap;
	app.add_option("-g,--syncgap", coreSyncgap, "Core syncgap");
	
	// extSelector
	vector<wstring> extSelectorData;
	app.add_option("-e,--extension", extSelectorData, "Extension (something like 'jpg') selector, be empty to process everything");
	set<wstring> extSelector;
	for (const wstring& x : extSelectorData) extSelector.insert(L"." + x);
	
	// isForced
	bool isForced = false;
	app.add_flag("-f,--force", isForced, "Overwrite output path in force") -> default_val("false");
	// isRecursive
	bool isRecursive = true;
	app.add_flag("-r,--recurse", isRecursive, "Go into the subdir to process") -> default_val("true");

	CLI11_PARSE(app, argc, argv);

	// apply to the target
	target.setInputPath(inputPathStr);
	target.setOutputPath(outputPathStr);
	target.setCoreModel(coreModel);
	target.setCoreScale(to_wstring(coreScale));
	target.setCoreDenoise(to_wstring(coreDenoise));
	target.setCoreSyncgap(to_wstring(coreSyncgap));
	target.setExtSelector(extSelector);
	if (isForced) target.setForced(); else target.unsetForced();
	if (isRecursive) target.setRecursive(); else target.unsetRecursive();
	return 0;
}

int main(int argc, char* argv[]) {
	system("chcp 65001 > nul");
	argv = app.ensure_utf8(argv);
	parseCommandLine(argc, argv);
	target.process();
	return 0;
}
