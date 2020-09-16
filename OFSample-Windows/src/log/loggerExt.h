#pragma once

#include "Log.h"

#include <string>
#include <map>

using std::string;
using std::map;

namespace base {
	void Logd(const string& tag, const string& message, bool needSend = false);
	void Logi(const string& tag, const string& message, bool needSend = false);
	void Logw(const string& tag, const string& message, bool needSend = false);
	void Loge(const string& tag, const string& message, bool needSend = false);

	void CreateLogFile(const string& path, const string& fileName);
	void DestoryLogFile();

	void DisplayAsciiTable(const string& tag, int width, const string& header, map<string, string> fields);
}
