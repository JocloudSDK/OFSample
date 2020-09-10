#pragma once

#include "sysbase/tool/xsignals.h"

class DWBASE_EXPORTS DwNetworkDetector
	: xsignals::trackable
{
public:
	DwNetworkDetector();
	~DwNetworkDetector();

private:
	DwNetworkDetector(const DwNetworkDetector &);
	DwNetworkDetector &operator=(const DwNetworkDetector &);

public:
	bool start();
	void stop();

public:
	typedef xsignals::signal<void ( bool isNetAvailable )> _SIG_NETSTATUS;
	_SIG_NETSTATUS sigNetStatus;

private:
	static UINT WINAPI _threadStub(PVOID param);
	UINT _threadProc();

	bool _triggerLayer();
	bool _logicLayer();
	bool _reportLayer();

private:
	HANDLE m_quitEvent;
	HANDLE m_hWorkThread;
};
