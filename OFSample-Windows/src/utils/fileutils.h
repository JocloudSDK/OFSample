#pragma once
#include <string>

bool doesFileExist(const std::string& filename);

bool isDirectoryExists(const std::string& filename);

bool makeDirectory(const std::string& folderPath);

bool gzLoadFile(const char* gzfn, std::string& out);

bool readBuildDisplayName(const char* packfile, std::string& out);