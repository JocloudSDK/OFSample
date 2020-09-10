#pragma  once
	
#include "dwbase/dwcomex.h"

const long MIN_VOLUME = -10000;
const long MAX_VOLUME = 0;

enum PlaybackState
{
	STATE_RUNNING,
	STATE_PAUSED,
	STATE_STOPPED,
	STATE_CLOSED
};

struct IGrabberCallback
{
	virtual HRESULT BufferCB(double dblSampleTime, BYTE * pBuffer, long lBufferSize)=NULL;
};

//ÊÓÆµ²¶×½
DWDEFINE_INTERFACE(IVideoPlayer) : public IUnk
{
	virtual PlaybackState State() const =0;

	virtual HRESULT OpenFile(const WCHAR* sFileName)=0;

	// Streaming
	virtual HRESULT Play()=0;
	virtual HRESULT Pause()=0;
	virtual HRESULT Stop()=0;

	// seeking
	virtual BOOL	CanSeek() const =0;
	virtual HRESULT SetPosition(LONGLONG pos)=0;
	virtual HRESULT GetDuration(LONGLONG *pDuration)=0;
	virtual HRESULT GetCurrentPosition(LONGLONG *pTimeNow)=0;

	// Audio
	virtual HRESULT	Mute(BOOL bMute)=0;
	virtual BOOL	IsMuted() const =0;
	virtual HRESULT	SetVolume(long lVolume)=0;
	virtual long	GetVolume() const =0; 

	//capture
	virtual HRESULT TakeSnapshot( LPBYTE pBuf,long *pBufSize )=0;
	virtual SIZE GetVideoSize() const =0;
	virtual void SetGrabberCallback(IGrabberCallback *pNotify)=0;
};

//ÆÁÄ»²¶×½
DWDEFINE_INTERFACE(IScreenCapture): public IUnk
{
	virtual HRESULT TakeSnapshot ( LPBYTE pBuf,long *pBufSize )=0;
	virtual SIZE GetTargetSize()=0;
	virtual void CaptureWindow(HWND hWnd)=0;
	virtual void CaptureRectangle(LPCRECT pRcTarget)=0;
};

