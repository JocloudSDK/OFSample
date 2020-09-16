#include "loggerExt.h"
#include <stdio.h>
#include <sstream>
#include <thread>
#include <iomanip>
#include <algorithm>
#include <regex>
#include <fstream>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

using namespace std;

typedef void(*OnWriteLog)(const char *message);

void DefaultLogFunction(const char *message) {
#if defined(_WIN32) || defined(_WIN64)
	OutputDebugStringA(message);
#endif
}

namespace base {

	enum LogLevel {
		LogLevelDebug = 0,
		LogLevelInfo = 1,
		LogLevelWarn = 2,
		LogLevelError = 3,
		LogLevelAll,
	};

	static const std::string LogLevelString[LogLevelAll] = {"Debug", "Info", "Warn", "Error"};

	static OnWriteLog g_pOnLog = DefaultLogFunction;
	static thread::id g_iMainThreadId = std::this_thread::get_id();
	static std::shared_ptr<std::ofstream> g_Fp;

	static inline std::string GetLocalTime() {
		char szbuffer[128] = { 0 };
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		sprintf_s(szbuffer, "[OFSample][%02d:%02d:%02d:%03d] => ", systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
		return szbuffer;
	}

	static inline std::string GetLocalDay() {
		char szbuffer[128] = { 0 };
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		sprintf_s(szbuffer, "-%04d-%02d-%02d-%02d-%02d-%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
		return szbuffer;
	}

	void CreateLogFile(const string& path, const string& fileName) {
		if (!g_Fp) {
			string name = path + "\\" + fileName + GetLocalDay() + ".log";
			g_Fp = std::make_shared<std::ofstream>();
			g_Fp->open(name, fstream::out | fstream::app | fstream::ate);
		}
	}
	
	void DestoryLogFile() {
		if (g_Fp) {
			g_Fp->flush();
			g_Fp->close();
		}
	}

	static void writeLog(LogLevel lv, const string& tag, const string& message, bool needSend = true) {
		string threadName;
		if (this_thread::get_id() == g_iMainThreadId) {
			threadName = "main";
		}
		else {
			std::stringstream ss;
			ss << std::this_thread::get_id();
			threadName = ss.str();
		}

		stringstream ss;
		ss << LogLevelString[lv] << '/';
		ss << GetLocalTime();

		if (!tag.empty()) {
			ss << " [" << tag << ']';
		}

		if (!message.empty()) {
			ss << ' ' << message;
		}
		ss << std::endl;

		g_pOnLog((char *)ss.str().c_str());

		if (g_Fp) {
			g_Fp->write(ss.str().c_str(), ss.str().size());
			g_Fp->flush();
		}
	}

	void Logd(const string& tag, const string& message, bool needSend) {
		writeLog(LogLevelDebug, tag, message, needSend);
	}

	void Logi(const string& tag, const string& message, bool needSend) {
		writeLog(LogLevelInfo, tag, message, needSend);
	}

	void Logw(const string& tag, const string& message, bool needSend) {
		writeLog(LogLevelWarn, tag, message, needSend);
	}

	void Loge(const string& tag, const string& message, bool needSend) {
		writeLog(LogLevelError, tag, message, needSend);
	}

	string repeatedString(size_t count, const string& ch) {
		stringstream ss;

		for (size_t i = 0; i < count; ++i) {
			ss << ch;
		}

		return ss.str();
	}

	void DisplayAsciiTable(const string& tag, int width, const string& header, map<string, string> fields) {
		static const int MaxTableLength = 128;
		width = min(width, MaxTableLength);

		string leftHeaderPadding = repeatedString((width - 4 - header.size()) / 2, "━");
		string rightHeaderPadding = repeatedString((width - 4 - header.size()) / 2 + (header.size() % 2), "━");
		string headerLine = "┏" + leftHeaderPadding + " " + header + " " + rightHeaderPadding + "┓";
		Logi(tag, headerLine);

		size_t maxLength = 0;
		for (auto& item : fields) {
			maxLength = max(maxLength, item.first.size());
		}

		char fmtLine[MaxTableLength + 1];
		sprintf_s(fmtLine, "┃ %%-%zds : %%-%zds ┃", maxLength, width - 7 - maxLength);

		char line[MaxTableLength + 1];
		for (auto& field : fields) {
			sprintf_s(line, fmtLine, field.first.c_str(), field.second.c_str());
			Logi(tag, line);
		}

		Logi(tag, "┗" + repeatedString(width - 2, "━") + "┛");
	}

}