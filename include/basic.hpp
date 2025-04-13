#ifndef ImageSRBasic_HPP
#define ImageSRBasic_HPP

#include <bits/stdc++.h>
#include <windows.h>

// you have to "chcp 65001" manually.
namespace ImageSRBasic {
	using namespace std::filesystem;

	// Basic
	std::wstring quote(std::wstring s) {
		std::wstringstream ss;
		ss << std::quoted(s);
		return ss.str();
	}
	std::wstring unquote(std::wstring s) {
		std::wstringstream ss;
		ss << s, ss >> std::quoted(s);
		return s;
	}
	std::string narrow(const std::wstring& s) {
		return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> >().to_bytes(s);
	}
	std::wstring widen(const std::string& s) {
		return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> >().from_bytes(s);
	}
	bool execute(std::wstring cmd) {
		FILE* fp;
		fp = _wpopen(cmd.c_str(), L"r");
		if (!fp) return true;
		char MsgBuff[4096]; // using wchar_t here will cause encode problem instead.
		memset(MsgBuff, 0, sizeof MsgBuff);
		while (fgets(MsgBuff, sizeof MsgBuff, fp)) std::cout << MsgBuff;
		_pclose(fp);
		return false;
	}

	class FileConfig {
	protected:
		path inputPath, outputPath; // these paths are all unquoted
		std::wstring model, scale, denoise, syncgap; // not each model has all these parameters
		static path ROOTPATH;
	public:
		FileConfig() {
			inputPath = outputPath = path();
			model = scale = denoise = syncgap = std::wstring();
		}

		bool setInputPath(const path& inputPath);
		bool setOutputPath(const path& outputPath);
		bool setModelInfo(const std::vector<std::wstring>& modelConfig);

		path getInputPath() const;
		path getOutputPath() const;
		std::vector<std::wstring> getModelInfo() const;
		static int getModelType_(const std::wstring& model_);
		int getModelType() const; // the result represents the parameters' number of the model

		void processAsFile() const;
	};
	
	class Config : public FileConfig {
	protected:
		bool isFile, isDir, isForced;
		std::set<std::wstring> selector;
		bool recursive; // maybe there's better name.
//		bool treeRestore, emptydirRebuild; these two are the function about filesystem, intending to code in a dll file. its not important now.
	public:
		Config() : FileConfig() {
			isFile = false;
			selector.clear();
			recursive = true;
		}
		
		bool setInputPath(const path& inputPath); // the function here is different from that in FileConfig, the logic of return is not the same, and there's more task should be finished here.
		bool setSelector(const std::set<std::wstring>& selector);
		void setRecursive(); void unsetRecursive();
		void setForced(); void unsetForced();
		
		std::set<std::wstring> getSelector() const; // is it really necessary to implement?
		
		void processAsDir();
		void process();
	};
	
	// FileConfig
	bool FileConfig::setInputPath(const path& inputPath) { // you actually can put an ellegal path here, it doesn't matter
		this->inputPath = weakly_canonical(inputPath);
		return !(exists(inputPath) && is_regular_file(inputPath));
	}
	bool FileConfig::setOutputPath(const path& outputPath) {
		this->outputPath = weakly_canonical(outputPath);
		return exists(outputPath);
	}
	bool FileConfig::setModelInfo(const std::vector<std::wstring>& modelInfo) {
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
	std::vector<std::wstring> FileConfig::getModelInfo() const {
		int modelType = getModelType();
		std::vector<std::wstring> modelInfo;
		modelInfo.push_back(model);
		for (int i = 1; i <= modelType; i++) {
			switch (i) {
				case 1 : modelInfo.push_back(scale); break;
				case 2 : modelInfo.push_back(denoise); break;
				case 3 : modelInfo.push_back(syncgap); break;
			}
		} return modelInfo;
	}
	int FileConfig::getModelType_(const std::wstring& model_) {
		if (model_ == L"realesrgan" || model_ == L"realesrnet" || model_ == L"realesrgan-anime" || model_ == L"DF2K" || model_ == L"DF2K-JPEG") return 1;
		if (model_ == L"waifu2x-anime" || model_ == L"waifu2x-photo") return 2;
		if (model_ == L"realcugan") return 3;
		return 0;
	}
	int FileConfig::getModelType() const { return getModelType_(model); }

	// Config
	bool Config::setInputPath(const path& inputPath) {
		this->inputPath = weakly_canonical(inputPath);
		isFile = is_regular_file(inputPath); // isFile will be false if the file or folder does not exists.
		isDir = is_directory(inputPath);
		return !(exists(inputPath) && (isFile || isDir));
	}
	bool Config::setSelector(const std::set<std::wstring>& selector) {
		this->selector = selector;
		return false;
	}
	void Config::setRecursive() 	{ this->recursive = true; }
	void Config::unsetRecursive() 	{ this->recursive = false; }
	void Config::setForced() 		{ this->isForced = true; }
	void Config::unsetForced() 		{ this->isForced = false; }
	std::set<std::wstring> Config::getSelector()	const { return selector; }
}

#endif
