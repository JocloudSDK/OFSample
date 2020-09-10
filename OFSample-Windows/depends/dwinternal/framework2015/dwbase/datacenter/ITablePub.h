#pragma  once

#include "sysbase/detail/yycom.h"
#include "sysbase/tool/xsignals.h"
#include "StringTable.h"
#include "FilterAND.h"
#include "FilterOR.h"
#include "FilterOption.h"
#include "dwbase/Variant.h"

namespace Data
{
	enum E_FIELD
	{
		FIELD_E_NULL,
		FIELD_E_STR,
		FIELD_E_DWORD64,
		FIELD_E_DWORD,
		FIELD_E_WORD,
		FIELD_E_BYTE,
		FIELD_E_BOOL,
		FIELD_E_BLOB,
		FIELD_E_UNK,
		FIELD_E_TABLE,
	};

	struct COL_PROP
	{
		COL_PROP()
		{
			eColType = FIELD_E_NULL;
		}

		NAME_ID colName;
		E_FIELD eColType;
		BOOL	bVirtual;	//ÊÇ·ñÐéÄâ£¨¼ÆËã£©×Ö¶Î
	};

	enum E_INDEX_TYPE
	{
		INDEX_E_NULL,
		INDEX_E_MAP,
		INDEX_E_HASH,
		INDEX_E_TINY_MAP,
	};

	enum E_KEY_TYPE
	{
		KEY_E_NULL,
		KEY_E_DWORD,
		KEY_E_STR,
	};

	struct KEY
	{
		KEY()
		{
			unKey.dwKey = 0;
			eType = KEY_E_NULL;
		}

		KEY(DWORD dwKey) 
		{ 
			unKey.dwKey = dwKey;
			eType = KEY_E_DWORD;
		}

		KEY(LPCWSTR lpszKey) 
		{ 
			unKey.lpszKey = lpszKey; 
			eType = KEY_E_STR;
		}

		union 
		{
			DWORD	dwKey;
			LPCWSTR	lpszKey;
		}unKey;

		E_KEY_TYPE eType;
	};
};
