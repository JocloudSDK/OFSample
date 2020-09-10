/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

//////////////////////////////////////////////////////////////////////////
//´íÎó´¦Àíºê
#define CHECK_RET(hr)       {{HRESULT _hr = (hr); if (FAILED(_hr)) return _hr;}}

#include <assert.h>
#ifndef ASSERT
        #define ASSERT(f)   assert(f)
#endif

//////////////////////////////////////////////////////////////////////////
//»ù´¡ÀàÐÍ
#pragma warning(disable : 4005)

#include "detail/yycom.h"
