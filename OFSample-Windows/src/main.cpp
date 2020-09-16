// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include "OrangeFilterUI.h"
#include "../src/log/loggerExt.h"
#include "DumpFile.h"

static const char* TAG = "SOUIEngine";
using namespace base;
static void createLogDir(std::string& path) {
	// 1. Determine whether the log directory has been created, if not, do not create it
	// 2. Create a directory
	if (1) {
		if (::CreateDirectoryA(path.c_str(), NULL)) {
			base::Logd(TAG, base::Log("createLogDir").setMessage("Dir path[%s] create OK!!!!", path.c_str()));
		}
		else {
			Logd(TAG, Log("createLogDir").setMessage("Dir path[%s] create Failed!!!!", path.c_str()));
		}
	}
	else {
		Logd(TAG, Log("createLogDir").setMessage("Dir path[%s] is exist!!!!", path.c_str()));
	}
}

static std::string ws2s(const std::wstring &ws)
{
	size_t i;
	std::string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const wchar_t* _source = ws.c_str();
	size_t _dsize = 2 * ws.size() + 1;
	char* _dest = new char[_dsize];
	memset(_dest, 0x0, _dsize);
	wcstombs_s(&i, _dest, _dsize, _source, _dsize);
	std::string result = _dest;
	delete[] _dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

static std::string getModuleDir() {
	TCHAR exeFullPath[MAX_PATH]; // Full path  
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	wstring strPath = __TEXT("");
	strPath = (wstring)exeFullPath;    // Get full path of the file  
	int pos = strPath.find_last_of(L'\\', strPath.length());
	std::string path = ws2s(strPath);
	return path.substr(0, pos);  // Return the directory without th
}

int main()
{

	DeclareDumpFile();
	
	std::string localPath = getModuleDir();
	localPath = localPath + "//log";
	createLogDir(localPath);
	CreateLogFile(localPath, "advance");
    OrangeFilterUI orangeUI;

	// Setup window
    if (orangeUI.OrangeFilterUICreate(1280, 700))
    {
        orangeUI.RendererRun();
    }
    return 0;
}

