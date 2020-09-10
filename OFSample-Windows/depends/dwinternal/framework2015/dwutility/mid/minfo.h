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

		// Ƶ����IP�õ���
		static std::string  getMid();

		// ����õ���mid��Ϣ���㹻��������һ��Ψһ��mid
		static void  getMid2(std::string& mid, std::string& midstr);

		// ����һ��ȫ�ֺ��������ļ��������ǰ�װ�͸��³�����õ�
		static bool  createGlobalMid();

		// ȫ�ֺ������õ�
		static std::string  getGlobalMid();

		static std::string	getMacInfo(const std::string &ipAddr);

		//call getMacAllInfo2 to get mac info sorted, in case the mac addresses are random.
		static std::string getMid3();

		// ��ȡ���л���
		static bool	getRunEnv(const std::wstring& netbarlists);
	};
}