#pragma once

#include <string>
#include <tchar.h>

class CMachineInfo
{
public:
	// ��һ��
	static bool CalcMid(std::string& mid);

	// �ڶ���
	// �����Դ���ģ����ɰ汾���ļ�������ļ���ȷ���ص�ʱ���ɵ�guid
	static void ReadGuid(std::string& guidstr);

	// �ļ��Ƿ����
	static bool HasGlobalMidFile(const char* filename);

	// ���ļ�����Ƚϴ����mid���ļ��е�mid��һ���ԣ������ȷ�᷵��ȫ�ֺ�����
	static bool ReadGlobalMid(const char* filename, const std::string& mid, std::string& str);

	// ����mid��guid�����ļ�
	static bool WriteGlobalMid(const char* filename, const std::string& mid, const std::string& guidstr);

	static bool md5(const char* str, int len, std::string& val);

	//static std::string AECDecrypt(std::string const& in,std::string const& key);

	//static std::string AESEncrypt(std::string const& in,std::string const& key);

private:
    // �õ�cpu id
	static std::string GetCpuID();

	// �õ�mac
	static std::string GetMacStr();

	// �õ��������к�
	static std::string GetDiskSeriNum();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ϰ汾����������
public:
	// ��װ/���µ�ʱ�����
	std::string CreateMachineID();

	// �õ�����id
	std::string GetMachineID();

public:
	bool Read(TCHAR* szFileName);
	void Write(TCHAR* szFileName);
	bool CalcGuid();

private:
	std::string  m_machinestr;
};