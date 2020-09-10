#pragma once

#include "dwglobal.h"
#include <string>

namespace DwUtility
{
	class DW_DWUTILITY_EXPORT minfo
	{
	public:
		static std::string  getCpuInfo();

		static std::string  getCpuALLInfo();

		static std::string  getMacInfo();

		static std::string  getMacAllInfo();

		static std::string  getWMIInfo();

		static std::string  getHDinfo();

		static std::string  getBiosInfo();

		// 频道封IP用到的
		static std::string  getMid();

		// 如果得到的mid信息不足够，会生产一个唯一的mid
		static void  getMid2(std::string& mid, std::string& midstr);

		// 生成一个全局黑名单的文件，这里是安装和更新程序调用的
		static bool  createGlobalMid();

		// 全局黑名单用的
		static std::string  getGlobalMid();

		static std::string	getMacInfo(const std::string &ipAddr);

		//call getMacAllInfo2 to get mac info sorted, in case the mac addresses are random.
		static std::string getMid3();

		// 获取运行环境
		static bool	getRunEnv(const std::wstring& netbarlists);
	};
}