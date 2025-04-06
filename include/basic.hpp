#ifndef ImageSRBasic_HPP
#define ImageSRBasic_HPP

#include <bits/stdc++.h>

namespace ImageSRBasic {
	using namespace std::filesystem;

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
		path inputPath, outputPath; // these paths are all unquoted
		std::string model, scale, denoise, syncgap; // not each model has all these parameters
	protected:
		static path ROOTPATH;
	public:
		FileConfig() {
			inputPath = outputPath = "";
			model = scale = denoise = syncgap = "";
		}

		bool setInputPath(const path& pathIn);
		bool setOutputPath(const std::string& pathOut);
		bool setModelInfo(const std::vector<std::string>& modelConfig);

		path getInputPath() const;
		path getOutputPath() const;
		std::vector<std::string> getModelInfo() const;
		static int getModelType_(const std::string& model_);
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
	bool FileConfig::setInputPath(const path& pathIn) { // you actually can put an ellegal path here, it doesn't matter
		inputPath = pathIn;
		return !(exists(pathIn) && is_regular_file(pathIn));
	}
	bool FileConfig::setOutputPath(const std::string& pathOut) {
		outputPath = pathOut;
		return exists(pathOut);
	}
	bool FileConfig::setModelInfo(const std::vector<std::string>& modelInfo) {
		int modelType = getModelType_(modelInfo[0]); // here, modelType always belongto [1, 3]
		if (int(modelInfo.size()) - 1 != modelType) return true; // the procession is all controlled by the model
		model = modelInfo[0];
		for (int i = 1; i <= modelType; i++) {
			switch (i) {
				case 1 : scale = modelInfo[i]; break;
				case 2 : denoise = modelInfo[i]; break;
				case 3 : syncgap = modelInfo[i]; break;
			}
		} return false;
	}
	path FileConfig::getInputPath() const { return inputPath; }
	path FileConfig::getOutputPath() const { return outputPath; }
	std::vector<std::string> FileConfig::getModelInfo() const {
		int modelType = getModelType();
		std::vector<std::string> modelInfo;
		modelInfo.push_back(model);
		for (int i = 1; i <= modelType; i++) {
			switch (i) {
				case 1 : modelInfo.push_back(scale); break;
				case 2 : modelInfo.push_back(denoise); break;
				case 3 : modelInfo.push_back(syncgap); break;
			}
		} return modelInfo;
	}
	int FileConfig::getModelType_(const std::string& model_) {
		if (model_ == "realesrgan" || model_ == "realesrnet" || model_ == "realesrgan-anime" || model_ == "DF2K" || model_ == "DF2K-JPEG") return 1;
		if (model_ == "waifu2x-anime" || model_ == "waifu2x-photo") return 2;
		if (model_ == "realcugan") return 3;
		return 0;
	}
	int FileConfig::getModelType() const { return getModelType_(model); }

	// DirConfig
//	bool FileConfig::setErrorPath(const std::string& path) {
//		errorPath = ImageSRBasic::unquote(path);
//		return false;
//	}
//	std::string FileConfig::getErrorPath() const { return errorPath; }
}

#endif
