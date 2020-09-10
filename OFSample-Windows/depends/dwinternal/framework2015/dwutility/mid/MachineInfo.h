#pragma once

#include <string>
#include <tchar.h>

class CMachineInfo
{
public:
	// 第一版
	static bool CalcMid(std::string& mid);

	// 第二版
	// 兼容性处理的，读旧版本的文件，如果文件正确返回当时生成的guid
	static void ReadGuid(std::string& guidstr);

	// 文件是否存在
	static bool HasGlobalMidFile(const char* filename);

	// 读文件，会比较传入的mid和文件中的mid的一致性，如果正确会返回全局黑名单
	static bool ReadGlobalMid(const char* filename, const std::string& mid, std::string& str);

	// 根据mid和guid生成文件
	static bool WriteGlobalMid(const char* filename, const std::string& mid, const std::string& guidstr);

	static bool md5(const char* str, int len, std::string& val);

	//static std::string AECDecrypt(std::string const& in,std::string const& key);

	//static std::string AESEncrypt(std::string const& in,std::string const& key);

private:
    // 得到cpu id
	static std::string GetCpuID();

	// 得到mac
	static std::string GetMacStr();

	// 得到磁盘序列号
	static std::string GetDiskSeriNum();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 老版本的其他函数
public:
	// 安装/更新的时候调用
	std::string CreateMachineID();

	// 得到机器id
	std::string GetMachineID();

public:
	bool Read(TCHAR* szFileName);
	void Write(TCHAR* szFileName);
	bool CalcGuid();

private:
	std::string  m_machinestr;
};