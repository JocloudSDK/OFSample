/** 
@file
@brief 
@version 
*/
#pragma once

const DWORD c_dwFileFlag = 0x00474F4C; //it's "LOG\0"
const DWORD c_dwFileVersion = 4;//����Thread Id���汾���޸�Ϊ2
                                //logֻ���ܾ�����־�ļ����� �����ֶβ����� �汾���޸�Ϊ4
const DWORD	c_dwCookie = 0x19810202;

struct OLD_LOGBUFFER_INFO//����logfileʱ�����ļ���С
{
    DWORD dwSize;
};
enum E_ITEM_TYPE
{
	TYPE_E_CONTENT,
	TYPE_E_CONTENT2,	//�޼���
};

struct LOG_FILE_HEAD
{
	DWORD	dwFlag;
	DWORD	dwVer;
};

struct LOG_CRYPT_BLOCK{
	DWORD	dwBlockSize;
	//BYTE	szData[dwBlockSize];
};

struct LOG_ITEM_HEADER
{
	DWORD			dwCookie;
	WORD			wType;
	WORD			wReserved;
};

struct LOG_ITEM
{
	LOG_ITEM_HEADER	header;
	DWORD			dwProcessId;
	DWORD			dwThreadId;
	SYSTEMTIME 		stTime;	//local time
	WORD			wLevel;
	WORD			wLine;
	DWORD			dwYY;

	WORD			cchFilter;
	WORD			cchFunc;
	WORD			cchCppName;
	WORD			cchModule;
	WORD			cchLog;
	/*
	CHAR			szFilter[1];
	CHAR			szFunc[1];
	CHAR			szCppName[1];
	CHAR			szModule;
	CHAR			szLog[1];
	*/
};