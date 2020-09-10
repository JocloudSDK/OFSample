/** 
@file
@brief 
@version 
*/
#pragma once

const DWORD c_dwFileFlag = 0x00474F4C; //it's "LOG\0"
const DWORD c_dwFileVersion = 4;//增加Thread Id，版本号修改为2
                                //log只加密具体日志文件内容 其他字段不加密 版本号修改为4
const DWORD	c_dwCookie = 0x19810202;

struct OLD_LOGBUFFER_INFO//覆盖logfile时传递文件大小
{
    DWORD dwSize;
};
enum E_ITEM_TYPE
{
	TYPE_E_CONTENT,
	TYPE_E_CONTENT2,	//无加密
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