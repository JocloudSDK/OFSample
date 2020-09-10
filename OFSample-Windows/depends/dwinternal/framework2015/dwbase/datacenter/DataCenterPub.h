#pragma once

#include "ITablePub.h"

namespace Data
{
	enum E_MATCH_CASE
	{
		CASE_E_EQUAL,
		CASE_E_NOT_EQUAL,
		CASE_E_IS_NULL,
		CASE_E_NOT_NULL,
		CASE_E_LESS,
		CASE_E_GREATER,
		CASE_E_MODIFIED,
	};

	enum E_COMBINE_MODE
	{
		COMBINE_E_NULL,
		COMBINE_E_AND,
		COMBINE_E_OR,
	};

	typedef BOOL (__fastcall * MATCH_CALLBACK)(DWORD dwMatchDum, const void *pValue, DWORD dwOffsetBit);
	struct MATCH_CASE
	{
		MATCH_CALLBACK	pfnMatch;
		E_MATCH_CASE	eMatchCase;
		DWORD			dwColIndex;
		DWORD			dwOffsetBit;
		DWORD			dwDum;//放指针或者直接放值
	};

	struct MATCH_NODE
	{
		//仅叶子节点有效
		MATCH_CASE *	arMatchCase;
		DWORD			dwMatchCaseCount;		

		//非叶子节点有效
		E_COMBINE_MODE	eMode;
		MATCH_NODE*		arChild;
		DWORD			dwChildCount;		
	};

	struct ORDER_CASE
	{
		BOOL		bAscending;
		E_FIELD		eType;
		DWORD		dwColIndex;
		DWORD		dwOffsetBit;
	};

	struct TABLE_STRUCT
	{
		NAME_ID					tableName;	
		NAME_ID					indexColName;
		E_INDEX_TYPE			eIndexType;
		std::vector<COL_PROP>	vecCol;
	};

	struct VIEW_STRUCT
	{
		NAME_ID		viewName;
	};

	struct FIELDSET_STRUCT
	{
		NAME_ID					fieldSetName;	
		std::vector<COL_PROP>	vecCol;
	};

	enum E_ROW_EVENT
	{
		ROW_E_ADD,
		ROW_E_CHANGED,
		ROW_E_DEL,
		ROW_E_RESET,
	};

	namespace Error
	{
		void OutputWarning(const TCHAR* lpszFmt, ...);
		void OutputError(const TCHAR* lpszFmt, ...);
		void OutputDebug(const TCHAR* lpszFmt, ...);
	}
};