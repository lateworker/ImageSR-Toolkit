#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

void outWstring(const wstring& str) { WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), NULL, NULL); }

std::wstring get_audio_codec(const std::wstring& input_file) {
	std::wstring command = L"ffmpeg.exe -i " + input_file + L" 2>&1";
	
	FILE* pipe = _wpopen(command.c_str(), L"r");
	
	wchar_t buffer[4096];
	std::wstring audio_info;

	while (fgetws(buffer, sizeof(buffer), pipe)) audio_info += buffer;
	
	_pclose(pipe);
	
	size_t start_pos = audio_info.find(L"Audio:") + 6;
	
	wstringstream ss(audio_info.substr(start_pos));
	
	ss >> audio_info;
	
	outWstring(audio_info);
	system("pause > nul");
	
	return audio_info;
}

void extract_audio(const std::string& input_file, const std::string& audio_codec) {
	std::string audio_extension = get_audio_extension(audio_codec);
	std::string output_audio_file = "output_audio." + audio_extension;
	
	std::string command = "ffmpeg -i " + input_file + " -vn -c:a copy " + output_audio_file;
	
	if (system(command.c_str()) != 0) {
		throw std::runtime_error("FFmpeg command execution failed");
	}
	
	std::cout << "Audio extracted successfully: " << output_audio_file << std::endl;
}

int main() {
	
	std::wstring input_file = L"input.mp4";
	
	try {
		std::wstring audio_codec = get_audio_codec(input_file);
//		extract_audio(input_file, audio_codec);
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
