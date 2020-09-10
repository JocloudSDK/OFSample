#include "MachineInfo.h"

#pragma warning( push )
#pragma warning( disable : 4091 )
#include <shlobj.h>
#include <Nb30.h>
#include "encryption/md5.h"
#include "encryption/aesencrypt.h"

// 第一版的文件结构
struct MACHINE_INFO
{
	WORD  wVersion;
	char  szGuid[32];
	char  szMid[32];
	//char  szReserved[62];
};

// 第二版加密key
#define AEC_KEY		"mu8ik,lo90p;/'-]" // 必须是16位

// 第二版的文件结构
struct MACHINE_INFO2
{
	WORD  wVersion;
	char  szGuid[32];
	char  szReserved1[64];
	char  szMid[64];
	char  szReserved2[94];
};

UCHAR dwNetbios( PNCB pncb );

std::string CMachineInfo::CreateMachineID()
{
	PERFLOG;
	if(m_machinestr.empty())
	{
		// All Users\Documents
		TCHAR szPath[MAX_PATH] = {0};
		if(SUCCEEDED(SHGetFolderPath(NULL, 
			CSIDL_APPDATA/*CSIDL_COMMON_APPDATA*/|CSIDL_FLAG_CREATE, 
			NULL, 
			0, 
			szPath)))
		{
			// 文件存放目录
			_tcscat(szPath, _T("\\systeminfo.cfg"));

			// 读本地文件
			if(!this->Read(szPath))
			{
				// 计算
				if(this->CalcGuid())
				{
					// 写到本地文件
					this->Write(szPath);
				}
			}
		}
	}
	return m_machinestr;
}

std::string CMachineInfo::GetMachineID()
{
	PERFLOG;
	if(m_machinestr.empty())
	{
		// All Users\Documents
		TCHAR szPath[MAX_PATH] = {0};
		if(SUCCEEDED(SHGetFolderPath(NULL, 
			CSIDL_APPDATA/*CSIDL_COMMON_APPDATA*/|CSIDL_FLAG_CREATE, 
			NULL, 
			0, 
			szPath)))
		{
			// 文件存放目录
			_tcscat(szPath, _T("\\systeminfo.cfg"));

			// 读本地文件
			if(!this->Read(szPath))
			{
#if 1
				// 2009/11/19
				// 如果读文件失败，还是生产guid，不如mid重复会影响正常的用户！
                if(this->CalcGuid())
				{
					// 再写到本地文件
					this->Write(szPath);
				}
#else
				this->CalcMid(m_machinestr);
#endif
			}
		}
	}
	return m_machinestr;
}

bool CMachineInfo::Read(TCHAR* szFileName)
{
	PERFLOG;
	FILE* ifs = _tfopen(szFileName, _T("rb"));
	if(ifs == NULL) {
		return false;
	}

	bool bRet=false;
    MACHINE_INFO info;
	if(fread(&info, sizeof(info), 1, ifs) == 1)
	{
#if 1
		// 2009/11/19
		// 不比较硬件信息，如果用户不小心改了硬件信息就会造成误杀！
		m_machinestr.assign(info.szGuid, 32);
#else
		std::string mid;
		mid.reserve( 33 );	//add by hehao 2009.07.29，如果不resever，在vista下的release版本会出问题
		if( !this->CalcMid(mid)
			|| memcmp(mid.c_str(), info.szMid, 32) == 0)
		{
			m_machinestr.assign(info.szGuid, 32);
		}
		else
		{
			// 何时不使用生成的guid呢？
			// 1、取mid失败
			// 2、取出的mid和当初计算guid时的mid不同
            m_machinestr = mid;
		}
#endif
		bRet=true;
	}
	fclose(ifs);
	return bRet;
}

bool CMachineInfo::CalcGuid()
{
	GUID guid;
    if(SUCCEEDED(CoCreateGuid(&guid)))
	{
		if(md5((const char*)&guid, sizeof(guid), m_machinestr))
		{
			if(m_machinestr.length() == 32) {
                return true;
			}
		}
        m_machinestr.clear();
	}
    return false;
}

void CMachineInfo::Write(TCHAR* szFileName)
{
	PERFLOG;
	// 生成一个mid
	std::string mid;
	if(!this->CalcMid(mid)) {
		return;
	}

	// 生成文件
	FILE* ofs = _tfopen(szFileName, _T("wb+"));
	if(ofs != NULL)
	{
		MACHINE_INFO info;
		memset(&info, 0, sizeof(info));
		memcpy(info.szGuid, m_machinestr.c_str(), 32);
		memcpy(info.szMid, mid.c_str(), 32);
		fwrite(&info, sizeof(info), 1, ofs);
		fclose(ofs);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 第一版本的重要的几个函数
bool CMachineInfo::CalcMid(std::string& mid)
{
    std::string str = GetCpuID();
	str += "|";
	str += GetMacStr();
	str += "|";
	str += GetDiskSeriNum();
    if(md5(str.c_str(), str.length(), mid))
	{
		if(mid.length() == 32) {
			return true;
		}
	}
	mid.clear();
    return false;
}

std::string CMachineInfo::GetCpuID()
{
	BYTE szCpu[16] = { 0 };
	UINT uCpuID    = 0U;
	try
	{
		_asm 
		{
			pushfd
			pushad

			mov eax, 0
			cpuid
			mov dword ptr szCpu[0], ebx
			mov dword ptr szCpu[4], edx
			mov dword ptr szCpu[8], ecx
			mov eax, 1
			cpuid
			mov uCpuID, edx

			popad
			popfd
		}
	}
	catch( ... )
	{

	}

	char str[64];
	sprintf(str, "%s%d", szCpu, uCpuID);
	return str;
}

std::string CMachineInfo::GetMacStr()
{
	PERFLOG;
	typedef struct _ASTAT_ 
	{ 
		ADAPTER_STATUS adapt; 
		NAME_BUFFER    NameBuff [30]; 
	} ASTAT, * PASTAT; 

	typedef struct _LANA_ENUM 
	{ // le 
		UCHAR length; 
		UCHAR lana[MAX_LANA]; 
	} LANA_ENUM ;

	NCB ncb; 
	ASTAT Adapter; 
	LANA_ENUM lana_enum; 

	UCHAR uRetCode;
	::memset( &ncb, 0, sizeof( ncb ) );
	::memset( &lana_enum, 0, sizeof( lana_enum ) );

	ncb.ncb_command = NCBENUM; 
	ncb.ncb_buffer  = (unsigned char *) &lana_enum; 
	ncb.ncb_length  = sizeof( LANA_ENUM ); 
	uRetCode        = dwNetbios( &ncb ); 
	if( uRetCode != NRC_GOODRET )
	{
		return ""; 
	}

	for( int lana = 0; lana < lana_enum.length; lana++ ) 
	{ 
		ncb.ncb_command  = NCBRESET; 
		ncb.ncb_lana_num = lana_enum.lana[lana]; 
		uRetCode         = dwNetbios( &ncb ); 
		if( uRetCode == NRC_GOODRET )
		{
			break;
		}
	}
	if( uRetCode != NRC_GOODRET ) 
	{
		return ""; 
	}

	::memset( &ncb, 0, sizeof( ncb ) ); 
	ncb.ncb_command  = NCBASTAT; 
	ncb.ncb_lana_num = lana_enum.lana[0]; 
	::strcpy( (char* )ncb.ncb_callname, "*" ); 
	ncb.ncb_buffer   = (unsigned char *) &Adapter; 
	ncb.ncb_length   = sizeof( Adapter ); 
	uRetCode         = dwNetbios( &ncb ); 
	if( uRetCode != NRC_GOODRET )
	{
		return "";
	}

	// 返回
	char str[64];
	sprintf(str,
		"%02X-%02X-%02X-%02X-%02X-%02X",
		Adapter.adapt.adapter_address[0], 
		Adapter.adapt.adapter_address[1], 
		Adapter.adapt.adapter_address[2], 
		Adapter.adapt.adapter_address[3], 
		Adapter.adapt.adapter_address[4], 
		Adapter.adapt.adapter_address[5] );
	return str;
}

std::string CMachineInfo::GetDiskSeriNum()
{
	char str[64];
	DWORD dwSerialNum=0;
	GetVolumeInformation(_T("c:\\"),NULL,NULL,&dwSerialNum,NULL,NULL,NULL,NULL);
	sprintf(str, "%d", dwSerialNum);
	return str;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMachineInfo::HasGlobalMidFile(const char* filename)
{
	PERFLOG;
	char path[MAX_PATH] = {0};
	if (FAILED(SHGetFolderPathA(NULL, 
		CSIDL_APPDATA/*CSIDL_COMMON_APPDATA*/|CSIDL_FLAG_CREATE, 
		NULL, 
		0, 
		path)))
	{
		return false;
	}

	strcat(path, "\\");
	strcat(path, filename);

	FILE* ifs = fopen(path, "rb");
	if (ifs != NULL)
	{
		fclose(ifs);
		return true;
	}

	// 2表示文件不存在
	if (GetLastError() != 2)
	{
		return true;
	}

	return false;
}

void CMachineInfo::ReadGuid(std::string& guidstr)
{
	PERFLOG;
	char path[MAX_PATH] = {0};
    if (FAILED(SHGetFolderPathA(NULL, 
        CSIDL_APPDATA/*CSIDL_COMMON_APPDATA*/|CSIDL_FLAG_CREATE, 
		NULL, 
		0, 
		path)))
	{
		return;
	}

	strcat(path, "\\systeminfo.cfg");

    FILE* ifs = fopen(path, "rb");
	if (ifs == NULL)
	{
		return;
	}

	// 读文件内容
	MACHINE_INFO info;
	if (fread(&info, sizeof(info), 1, ifs) == 1)
	{
        std::string mid;
		if (CalcMid(mid))
		{
			// 用旧版的计算mid的算法得到了一个mid
            if (memcmp(mid.c_str(), info.szMid, 32) == 0)
			{
				// 文件没问题
				guidstr.append(info.szGuid, 32);
			}
		}
	}
	fclose(ifs);
}

bool CMachineInfo::ReadGlobalMid(const char* filename, const std::string& mid, std::string& str)
{
	PERFLOG;
	char path[MAX_PATH] = {0};
	if (FAILED(SHGetFolderPathA(NULL, 
		CSIDL_APPDATA/*CSIDL_COMMON_APPDATA*/|CSIDL_FLAG_CREATE, 
		NULL, 
		0, 
		path)))
	{
		return false;
	}

	strcat(path, "\\");
	strcat(path, filename);

	FILE* ifs = fopen(path, "rb");
	if (ifs == NULL)
	{
		return false;
	}

	// 读文件
	char buff[1024];
	int len = fread(buff, 1, 1024, ifs);
	fclose(ifs);

	if (len >= sizeof(MACHINE_INFO2))
	{
		// 解密
		std::string instr(buff, len);
		std::string outstr;
		try {
			outstr = DwUtility::encryption::AESDecrypt(instr, AEC_KEY);
		} catch (...) {
			outstr.clear();			
		}

		// 判断
		if (outstr.length() == sizeof(MACHINE_INFO2))
		{
			const MACHINE_INFO2* info = (const MACHINE_INFO2*)outstr.c_str();
			if (mid == info->szMid)
			{
				str = info->szGuid;
				return true;
			}
		}
	}
    return false;
}

bool CMachineInfo::WriteGlobalMid(const char* filename, const std::string& mid, const std::string& guidstr)
{
	PERFLOG;
	char path[MAX_PATH] = {0};
	if (FAILED(SHGetFolderPathA(NULL, 
		CSIDL_APPDATA/*CSIDL_COMMON_APPDATA*/|CSIDL_FLAG_CREATE, 
		NULL, 
		0, 
		path)))
	{
		return false;
	}

	strcat(path, "\\");
	strcat(path, filename);

	FILE* ofs = fopen(path, "wb+");
	if (ofs == NULL)
	{
		return false;
	}

    // 构造文件内容
	MACHINE_INFO2 info;
    memset(&info, 0, sizeof(info));
	info.wVersion = 115; // 2010/01/15
    memcpy(info.szGuid, guidstr.c_str(), 32);
	memcpy(info.szMid , mid.c_str(), mid.length()); // 最长64位

	// 加密
	std::string instr((const char*)&info, sizeof(info));
	std::string outstr;
	try {
		outstr = DwUtility::encryption::AESEncrypt(instr, AEC_KEY);
	} catch(...) {
		outstr.clear();
	}

	// 写文件
	if (!outstr.empty())
	{
		if (fwrite(outstr.c_str(), outstr.length(), 1, ofs) == 1)
		{
			fclose(ofs);
			return true;
		}
	}

	fclose(ofs);
	return false;
}

bool CMachineInfo::md5(const char* str, int len, std::string& val)
{
	if(NULL == str || len == 0)
		return false;

	DwUtility::encryption::MD5 data;
	data.update((BYTE *)(str), len);
	data.finalize();
	char hex[33];
	data.hex_digest (hex);

	val.assign(hex, 32);

	return true;
}

//std::string CMachineInfo::AECDecrypt(std::string const& in,std::string const& key)
//{
//	assert(key.size() == 16);
//	size_t s = in.size();
//	if ( s%16 != 0 )
//	{
//		s = 16*(s/16+1);
//	}
//
//	std::string strTmp;
//	strTmp.resize( s );
//	std::string strDes;
//	strDes.resize( s );
//	std::fill(strTmp.begin(),strTmp.end(),0);
//	std::fill(strDes.begin(),strDes.end(),0);
//	std::copy( in.begin(),in.end(),strTmp.begin() );
//
//	AES_KEY k;
//	AES_set_encrypt_key( (unsigned char*)key.data(),128,&k );
//	unsigned char* pSrc = (unsigned char*)strTmp.data();
//	unsigned char* pDes = (unsigned char*)strDes.data();
//	for ( size_t i = 0;i < s/16;i++ )
//	{
//		AES_encrypt( pSrc,pDes,&k );
//		pSrc += 16;
//		pDes += 16;
//	}
//	return strDes;
//}
//
//std::string CMachineInfo::AESEncrypt(std::string const& in,std::string const& key)
//{
//	size_t s = in.size();
//	assert(s%16==0);
//	assert(key.size() == 16);
//
//	std::string strDes;
//	strDes.resize( s );
//	std::fill(strDes.begin(),strDes.end(),0);
//
//	AES_KEY k;
//	AES_set_decrypt_key( (unsigned char*)key.data(),128,&k );
//	unsigned char* pSrc = (unsigned char*)in.data();
//	unsigned char* pDes = (unsigned char*)strDes.data();
//	for ( size_t i = 0;i < s/16;i++ )
//	{
//		AES_decrypt( pSrc,pDes,&k );
//		pSrc += 16;
//		pDes += 16;
//	}
//	return strDes;
//}
#pragma warning( pop )