#include <bits/stdc++.h>
#include "../include/json.hpp"
#include "../include/basic.hpp"

using json = nlohmann::json;
using namespace std;
using namespace ImageSRBasic;

// 分离视频中的所有音轨
void extract_audio_tracks(const wstring& video_file) {
	// 调用 ffprobe 获取视频信息
	wstring cmd = L"..\\ffmpeg\\ffprobe -v quiet -print_format json -show_streams " + video_file;
	wstring output = execute(cmd);
	
	// 解析 JSON 输出
	try {
		json data = json::parse(output);
		
		for (const auto& stream : data["streams"]) {
			if (stream["codec_type"] == "audio") {
				int index = stream["index"].get<int>();
				wstring codec_name = widen(stream["codec_name"]);
				
				// 使用 ffmpeg 分离音轨
				wstring ffmpeg_cmd = L"..\\ffmpeg\\ffmpeg -i " + video_file + L" -map 0:a:" + to_wstring(index - 1) + 
				L" -c copy " + video_file + L"_audio_" + to_wstring(index) + L"." + codec_name;
				cout << "Executing: " << narrow(ffmpeg_cmd) << endl;
				cout << narrow(execute(ffmpeg_cmd)) << endl;
			}
		}
	} catch (const json::exception& e) {
		cerr << "JSON parsing error: " << e.what() << endl;
	}
}

//// 将音频合并到无声视频中
//void merge_audio_to_video(const string& video_file, const string& audio_file, const string& output_file) {
//	// 检查视频文件是否存在
//	ifstream video(video_file);
//	if (!video) {
//		cerr << "Video file not found: " << video_file << endl;
//		return;
//	}
//	
//	// 检查音频文件是否存在
//	ifstream audio(audio_file);
//	if (!audio) {
//		cerr << "Audio file not found: " << audio_file << endl;
//		return;
//	}
//	
//	// 使用 ffmpeg 合并音频和视频
//	string ffmpeg_cmd = "ffmpeg -i " + video_file + " -i " + audio_file + " -c:v copy -map 0:v:0 -map 1:a:0 " + output_file;
//	cout << "Executing: " << ffmpeg_cmd << endl;
//	string result = exec(ffmpeg_cmd.c_str());
//	cout << result << endl;
//	
//	cout << "Successfully merged audio into video." << endl;
//}

int main() {
	extract_audio_tracks(L"test.mkv");
//	else if (command == "merge" && argc == 5) {
//		string video_file = argv[2];
//		string audio_file = argv[3];
//		string output_file = argv[4];
//		merge_audio_to_video(video_file, audio_file, output_file);
//	}
	return 0;
}
