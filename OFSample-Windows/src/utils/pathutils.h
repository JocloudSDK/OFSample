#pragma once

#include <string>

std::string PathParentA(const std::string& path);
std::wstring PathParentW(const std::wstring& path);
std::string CurrentApplicationDirA();
std::wstring CurrentApplicationDirW();
void PathJoinA(std::string& dst, const std::string& base, const std::string& name);
void PathJoinW(std::wstring& dst, const std::wstring& base, const std::wstring& name);