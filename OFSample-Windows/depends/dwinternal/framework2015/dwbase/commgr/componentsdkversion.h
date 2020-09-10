#pragma once

#include "dwglobal.h"

unsigned int DWBASE_EXPORTS componentSdkVersion();
std::wstring DWBASE_EXPORTS componentSdkVersionString();
std::wstring DWBASE_EXPORTS componentSdkVersionStringHex();
unsigned int DWBASE_EXPORTS majorVersion();
unsigned int DWBASE_EXPORTS minorVersion();
unsigned int DWBASE_EXPORTS patchVersion();
