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
	protected:
		path inputPath, outputPath; // these paths are all unquoted
		std::string model, scale, denoise, syncgap; // not each model has all these parameters
		static path ROOTPATH;
	public:
		FileConfig() {
			inputPath = outputPath = "";
			model = scale = denoise = syncgap = "";
		}

		bool setInputPath(const path& inputPath);
		bool setOutputPath(const path& outputPath);
		bool setModelInfo(const std::vector<std::string>& modelConfig);

		path getInputPath() const;
		path getOutputPath() const;
		std::vector<std::string> getModelInfo() const;
		static int getModelType_(const std::string& model_);
		int getModelType() const; // the result represents the parameters' number of the model

		void processAsFile() const;
	};
	
	class Config : public FileConfig {
	protected:
		std::string selector;
		bool treeRestore, subdirProcess, emptydirRebuild;
		path errorPath;
		bool errorBackup;
	public:
		bool isFile;
		Config() : FileConfig() {
			inputPath = "input";
			outputPath = "output";
			isFile = false;
			selector = "*";
			treeRestore = true;
			subdirProcess = true;
			emptydirRebuild = false;
			errorPath = "";
			errorBackup = true;
		}
		
		bool setInputPath(const path& inputPath); // the function here is different from that in FileConfig, the logic of return is not the same, and there's more task should be finished here.
		bool setSelector(const std::string& selector);
		bool setTreeRestore(bool treeRestore);
		bool setSubdirProcess(bool subdirProcess);
		bool setEmptydirRebuild(bool emptydirRebuild);
		bool setErrorPath(const path& pathError);
		bool setErrorBackup(bool errorBackup);
		
		std::string getSelector() const;
		bool getTreeRestore() const;
		bool getSubdirProcess() const;
		bool getEmptydirRebuild() const;
		path getErrorPath() const;
		bool getErrorBackup() const;
	};
	
	// FileConfig
	bool FileConfig::setInputPath(const path& inputPath) { // you actually can put an ellegal path here, it doesn't matter
		this->inputPath = inputPath;
		return !(exists(inputPath) && is_regular_file(inputPath));
	}
	bool FileConfig::setOutputPath(const path& outputPath) {
		this->outputPath = outputPath;
		return exists(outputPath);
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
	path FileConfig::getInputPath() 	const { return inputPath; }
	path FileConfig::getOutputPath() 	const { return outputPath; }
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

	// Config
	bool Config::setInputPath(const path& inputPath) {
		this->inputPath = inputPath;
		isFile = is_regular_file(inputPath); // isFile will be false if the file or folder does not exists.
		return !exists(inputPath);
	}
	bool Config::setSelector(const std::string& selector) {
		this->selector = selector;
		return false;
	}
	bool Config::setTreeRestore(bool treeRestore) {
		this->treeRestore = treeRestore;
		return false;
	}
	bool Config::setSubdirProcess(bool subdirProcess) {
		this->subdirProcess = subdirProcess;
		return false;
	}
	bool Config::setEmptydirRebuild(bool emptydirRebuild) {
		this->emptydirRebuild = emptydirRebuild;
		return false;
	}
	bool Config::setErrorPath(const path& errorPath) {
		this->errorPath = errorPath;
		return false;
	}
	bool Config::setErrorBackup(bool errorBackup) {
		this->errorBackup = errorBackup;
		return false;
	}
	std::string Config::getSelector() 	const { return selector; }
	bool Config::getTreeRestore() 		const { return treeRestore; }
	bool Config::getSubdirProcess() 	const { return subdirProcess; }
	bool Config::getEmptydirRebuild() 	const { return emptydirRebuild; }
	path Config::getErrorPath() 		const { return errorPath; }
	bool Config::getErrorBackup() 		const { return errorBackup; }
	
}

#endif
