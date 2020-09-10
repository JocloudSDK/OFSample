/** 
@file
@brief 采集控制选项
@version 2011-9-16 bobdeng
         2012-8-29 wesleywu    Add "Sampling Disk/Network" Option
*/


#pragma once

#include "stlalloc.h"

namespace Perf
{
	class CControl
	{
	public:
		static CControl *GetInstance();
		
		BOOL IsSamplingCallStack(){ return m_bSamplingCallStack; }
		BOOL IsSamplingResponse() { return m_bSamplingResponse; }
		BOOL IsSamplingCPUMem() { return m_bSamplingCPUMem; }

		BOOL IsSamplingWMI() { return m_bSamplingWMI; }

		DWORD GetSamplingLogLevel() { return m_dwSamplingLogLevel; }
		void SetSamplingLogLevel(DWORD dwLevel) { m_dwSamplingLogLevel = dwLevel; }

		void EnablePerfStub(BOOL bEnable) {m_bEnablePerfStub = bEnable;}
		BOOL isPerfStubEnabled(){ return m_bEnablePerfStub;}

		BOOL isMultiPerfStubEnabled(){ return m_bEnableMultiPerlStub;}

		void OpenFullOption();
		void CloseAllOption();

		void SetAPIHook(BOOL bEnable);
		BOOL IsAPIHook();

	private:
		std::wstring GetValue(const std::wstring& strOption, const std::wstring& strKey);
		void PaserCommandLine(LPCTSTR pCommandLine);
		CControl();

	private:

		BOOL				m_bSamplingCallStack;
		BOOL				m_bSamplingResponse;
		BOOL				m_bSamplingCPUMem;
		BOOL				m_bSamplingWMI;		// New
		BOOL				m_bEnablePerfStub;
		BOOL				m_bEnableMultiPerlStub;
		DWORD				m_dwSamplingLogLevel;

		CRITICAL_SECTION	m_cri;
	};
}
