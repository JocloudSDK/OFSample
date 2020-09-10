/*! @file comenviroment.h 
 *  @brief ����/����COM����
 *
 *  @author wesleywu
 *  @version 1.0
 *  @data 2012-08-28
 */

#pragma once

#include <objbase.h>

namespace Perf
{
	// ����COM֧��
	class CComEnviroment
	{
		// ֻ��ͨ������ڽ���COM�����ĳ�ʼ��
		friend HRESULT EnableComSupport(DWORD);
	protected:
		static CComEnviroment& GetInstance();
		HRESULT Initialize(DWORD initFlag = COINIT_MULTITHREADED);

		CComEnviroment();
		~CComEnviroment();

	private:
		bool m_bInitialized;
	};

	// ʹ��Perf::EnableComSupport()��������COM֧��
	HRESULT EnableComSupport( DWORD initFlag = COINIT_MULTITHREADED );
}