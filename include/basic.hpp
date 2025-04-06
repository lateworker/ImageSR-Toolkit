#ifndef ImageSRBasic_HPP
#define ImageSRBasic_HPP

#include <bits/stdc++.h>

namespace ImageSRBasic {

	// Basic
	std::string quote(std::string s) {
		std::stringstream ss;
		ss << std::quoted(s);
		return ss.str();
	}
	std::string unquote(std::string s) {
		std::stringstream ss;
		ss << s, ss >> std::quoted(s);
		return s;
	}
	auto execute(std::string s) { return system(s.c_str()); }

	class FileConfig {
		std::string inputPath, outputPath; // these paths are all unquoted
		std::string model, scale, denoise, syncgap; // not each model has all these parameters
	public:
		FileConfig() {
			inputPath = outputPath = model = scale = denoise = syncgap = "";
		}

		bool setInputPath(const std::string& path);
		bool setOutputPath(const std::string& path);
		bool setModelInfo(const std::vector<std::string>& modelConfig);

		std::string getInputPath() const;
		std::string getOutputPath() const;
		std::vector<std::string> getModelInfo() const;
		int getModelType() const; // the result represents the parameters' number of the model

		void processAsFile() const;
	};
	
	class DirConfig : public FileConfig {
		std::string errorPath, selector;
		bool treeRestore, subdirProcess, emptydirRebuild, errorBackup;
	public:
		using FileConfig::FileConfig;
		DirConfig() {
			selector = "*";
			treeRestore = true;
			subdirProcess = true;
			emptydirRebuild = false;
			errorBackup = true;
		}
		
//		std::string getErrorPath() const;
//		bool setErrorPath(const std::string& path);
	};
	
	// FileConfig
	bool FileConfig::setInputPath(const std::string& path) { // should check the path
		inputPath = ImageSRBasic::unquote(path);
		return false;
	}
	bool FileConfig::setOutputPath(const std::string& path) {
		outputPath = ImageSRBasic::unquote(path);
		return false;
	}
	bool FileConfig::setModelInfo(const std::vector<std::string>& modelInfo) {
		model = modelInfo[0];
		int modelType = getModelType(); // here, modelType always belongto [1, 3]
		if (int(modelInfo.size()) - 1 != modelType) return true;
		for (int i = 1; i <= modelType; i++) {
			switch (i) {
				case 1 : scale = modelInfo[i]; break;
				case 2 : denoise = modelInfo[i]; break;
				case 3 : syncgap = modelInfo[i]; break;
			}
		} return false;
	}
	std::string FileConfig::getInputPath() const { return inputPath; }
	std::string FileConfig::getOutputPath() const { return outputPath; }
	std::vector<std::string> FileConfig::getModelInfo() const {
		std::vector<std::string> modelInfo;
		modelInfo.push_back(model);
		int modelType = getModelType();
		for (int i = 1; i <= modelType; i++) {
			switch (i) {
				case 1 : modelInfo.push_back(scale); break;
				case 2 : modelInfo.push_back(denoise); break;
				case 3 : modelInfo.push_back(syncgap); break;
			}
		} return modelInfo;
	}
	int FileConfig::getModelType() const {
		if (model == "realesrgan" || model == "realesrnet" || model == "realesrgan-anime" || model == "DF2K" || model == "DF2K-JPEG") return 1;
		if (model == "waifu2x-anime" || model == "waifu2x-photo") return 2;
		if (model == "realcugan") return 3;
		return 0;
	}

	// DirConfig
//	bool FileConfig::setErrorPath(const std::string& path) {
//		errorPath = ImageSRBasic::unquote(path);
//		return false;
//	}
//	std::string FileConfig::getErrorPath() const { return errorPath; }
}

#endif
