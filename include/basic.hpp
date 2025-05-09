#ifndef ImageSRBasic_HPP
#define ImageSRBasic_HPP

#include <bits/stdc++.h>
#include <windows.h>

// you have to "chcp 65001" manually.
namespace ImageSRBasic {
	using namespace std::filesystem;


// Basic
	const size_t BufferSize = 4096;

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

	std::wstring execute(const std::wstring& cmd) {
		FILE* fp = _wpopen(cmd.c_str(), L"r");
		if (!fp) {
			// throw error
		}
		std::string echoResult;
		char MsgBuff[BufferSize]; // using wchar_t here will cause encode problem instead.
//		memset(MsgBuff, 0, sizeof MsgBuff); I think it's useless.
		while (fgets(MsgBuff, sizeof MsgBuff, fp)) echoResult += MsgBuff;
		_pclose(fp);
		return widen(echoResult);
	}

	std::filesystem::path getExecPath() {
		wchar_t PathBuff[BufferSize];
		GetModuleFileNameW(NULL, PathBuff, BufferSize);
		return path(PathBuff);
	}

	std::string getRandomString(size_t length) { // unnecessary to use wstring.
		const static std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_int_distribution<> distribution(0, characters.size() - 1);
		std::string result;
		for (size_t i = 0; i < length; ++i)
			result += characters[distribution(generator)];
		return result;
	}


// FileConfig
	class FileConfig {
	protected:

		path inputPath, outputPath; // these paths are all unquoted

		struct Realesr { std::wstring model, scale; };
		struct Waifu2x { std::wstring model, scale, denoise; };
		struct Realcugan { std::wstring model, scale, denoise, syncgap; };
		std::variant<std::monostate, Realesr, Waifu2x, Realcugan> coreConfig;

	public:

		bool isForced; // whether force overwriting the existing file.

		enum { UNDEFINED, REALESR, WAIFU2X, REALCUGAN, MODELCOUNTER };
		using CoreConfig_t = std::variant<std::monostate, Realesr, Waifu2x, Realcugan>;
		static path ROOTPATH, MODELPATH[MODELCOUNTER];

		FileConfig() {
			inputPath = outputPath = path();
			coreConfig.emplace<0>();
		}

		// set
		bool setInputPath(const path& inputPath);
		bool setOutputPath(const path& outputPath);

		bool setCoreModel(const std::wstring& model, bool overwrite);
		bool setCoreScale(const std::wstring& scale);
		bool setCoreDenoise(const std::wstring& denoise);
		bool setCoreSyncgap(const std::wstring& syncgap);
		bool setCoreConfig(const CoreConfig_t& coreConfig);

		// get
		path getInputPath() const;
		path getOutputPath() const;

		int getCoreType() const; // the result represents the parameters' number of the model
		CoreConfig_t getCoreConfig() const;
		static int getCoreType(const std::wstring& model);

		// func
		void processAsFile() const;
	};


// Config from FileConfig
	class Config : public FileConfig {
	protected:

		bool isFile, isDir;
//		bool treeRestore, emptydirRebuild; these two are the function about filesystem, intending to code in a dll file. its not important now.

		std::set<std::wstring> extSelector;

	public:

		bool isRecursive; // maybe there's better name.

		Config() : FileConfig() {
			isRecursive = true;
			extSelector.clear();
		}

		// set
		bool setInputPath(const path& inputPath); // the function here is different from that in FileConfig, the logic of return is not the same, and there's more task should be finished here.

		bool setExtSelector(const std::set<std::wstring>& extSelector);

		// get
		std::set<std::wstring> getExtSelector() const; // is it really necessary to implement?

		// func
		void processAsDir() const;
		void process() const;
	};


// VideoManager
	class VideoMan {
	protected:

		path sourcePath, tmpdirPath; // OnlyPath ?

	public:

		static size_t MAXLENGTH; // max length of the tmpdir name to random.
		static path TMPDIRROOT; // parent path of the tmpdirs.

		VideoMan() {
			sourcePath = path();
			tmpdirPath = TMPDIRROOT / getRandomString(MAXLENGTH);
		}

		// set
		bool setSourcePath(const path& sourcePath);
		bool setTmpdirName(const path& tmpdirName);

		// get
		path getSourcePath() const;
		path getTmpdirPath() const;
	};


// FileConfig

	// set
	bool FileConfig::setInputPath(const path& inputPath) { // you actually can put an ellegal path here, it doesn't matter
		if (!exists(inputPath)) return true;
		this->inputPath = canonical(inputPath); // use canonical because the path defenitely exists
		return !is_regular_file(inputPath);
	}

	bool FileConfig::setOutputPath(const path& outputPath) {
		this->outputPath = weakly_canonical(absolute(outputPath));
		return exists(outputPath);
	}

	bool FileConfig::setCoreModel(const std::wstring& model, bool overwrite = false) {
		if (!overwrite && this->getCoreType() != 0) return true;
		switch (getCoreType(model)) {
			case REALESR: 	this->coreConfig.emplace<REALESR>(model); break;
			case WAIFU2X: 	this->coreConfig.emplace<WAIFU2X>(model); break;
			case REALCUGAN: this->coreConfig.emplace<REALCUGAN>(model); break;
			case 0: this->coreConfig.emplace<0>(); break;
			default: return true;
		} return false;
	}

	bool FileConfig::setCoreScale(const std::wstring& scale) {
		switch (this->getCoreType()) {
			case REALESR: 	get<REALESR>(this->coreConfig).scale = scale; break;
			case WAIFU2X: 	get<WAIFU2X>(this->coreConfig).scale = scale; break;
			case REALCUGAN: get<REALCUGAN>(this->coreConfig).scale = scale; break;
			default: return true;
		} return false;
	}

	bool FileConfig::setCoreDenoise(const std::wstring& denoise) {
		switch (this->getCoreType()) {
			case WAIFU2X: 	get<WAIFU2X>(this->coreConfig).denoise = denoise; break;
			case REALCUGAN: get<REALCUGAN>(this->coreConfig).denoise = denoise; break;
			default: return true;
		} return false;
	}

	bool FileConfig::setCoreSyncgap(const std::wstring& syncgap) {
		switch (this->getCoreType()) {
			case REALCUGAN: get<REALCUGAN>(this->coreConfig).syncgap = syncgap; break;
			default: return true;
		} return false;
	}

	bool FileConfig::setCoreConfig(const FileConfig::CoreConfig_t& coreConfig) {
		this->coreConfig = coreConfig;
		return coreConfig.index() == 0;
	}

	// get
	path FileConfig::getInputPath() 	const { return inputPath; }
	path FileConfig::getOutputPath() 	const { return outputPath; }

	FileConfig::CoreConfig_t FileConfig::getCoreConfig() const { return coreConfig; }

	int FileConfig::getCoreType() const { return coreConfig.index(); }

	int FileConfig::getCoreType(const std::wstring& model) {
		if (model == L"realesrgan") 			return REALESR;
		if (model == L"realesrnet") 			return REALESR;
		if (model == L"realesrgan-anime") 		return REALESR;
		if (model == L"realesrgan-animevideo") 	return REALESR;
		if (model == L"DF2K") 					return REALESR;
		if (model == L"DF2K-JPEG") 				return REALESR;
		if (model == L"waifu2x-anime")			return WAIFU2X;
		if (model == L"waifu2x-photo")			return WAIFU2X;
		if (model == L"realcugan")				return REALCUGAN;
		return UNDEFINED;
	}


// Config

	// set
	bool Config::setInputPath(const path& inputPath) {
		if (!exists(inputPath)) return false;
		this->inputPath = canonical(inputPath);
		isFile = is_regular_file(inputPath); // isFile will be false if the file or folder does not exists.
		isDir = is_directory(inputPath);
		return !(isFile || isDir);
	}

	bool Config::setExtSelector(const std::set<std::wstring>& extSelector) {
		this->extSelector = extSelector;
		return false;
	}

	// get
	std::set<std::wstring> Config::getExtSelector()	const { return extSelector; }


// VideoMan

	// set
	bool VideoMan::setSourcePath(const path& sourcePath) {
		this->sourcePath = sourcePath;
		return false; // maybe I should check something here?
	}

	bool VideoMan::setTmpdirName(const path& tmpdirName) {
		this->tmpdirPath = TMPDIRROOT / tmpdirName;
		return false;
	}

	// get
	path VideoMan::getSourcePath() const { return sourcePath; }

	path VideoMan::getTmpdirPath() const { return tmpdirPath; }

}

#endif
