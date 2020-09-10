/** 
@file
@brief 内存池的头文件
@version 2011-9-26 bobdeng
*/
#pragma once

#include "dwglobal.h"

DWBASE_EXPORTS void *		mp_alloc(UINT uSize);
DWBASE_EXPORTS void *		mp_realloc(void *p, UINT uNewSize); 
DWBASE_EXPORTS void			mp_free(void *p);
DWBASE_EXPORTS UINT			mp_size(const void *p);
