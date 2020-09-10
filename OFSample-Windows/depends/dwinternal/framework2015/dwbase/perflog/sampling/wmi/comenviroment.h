/*! @file comenviroment.h 
 *  @brief 启用/禁用COM环境
 *
 *  @author wesleywu
 *  @version 1.0
 *  @data 2012-08-28
 */

#pragma once

#include <objbase.h>

namespace Perf
{
	// 启用COM支持
	class CComEnviroment
	{
		// 只能通过该入口进行COM环境的初始化
		friend HRESULT EnableComSupport(DWORD);
	protected:
		static CComEnviroment& GetInstance();
		HRESULT Initialize(DWORD initFlag = COINIT_MULTITHREADED);

		CComEnviroment();
		~CComEnviroment();

	private:
		bool m_bInitialized;
	};

	// 使用Perf::EnableComSupport()即可启用COM支持
	HRESULT EnableComSupport( DWORD initFlag = COINIT_MULTITHREADED );
}