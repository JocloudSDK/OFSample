#pragma once
#include "videoinfo.h"
#include "yyvideochatproxy_i.h"
#include "yycomponent/propBag_i.h"

typedef xsignals::signal<void (const ImP2PVideoInfo::P2P_VIDEO_INFORMATION& vInfo, bool bAccepted, ImP2PVideoInfo::VideoMsgStatus videoStatus,bool noRespone)> _SIG_IM_VIDEO_REQUEST;
typedef xsignals::signal<void (const ImP2PVideoInfo::P2P_VIDEO_INFORMATION& vInfo)> _SIG_MAGICAL_VIDEO_BOX_REQUEST;
typedef xsignals::signal<void ()> _SIG_IM_VIDEO_STATE;
typedef xsignals::signal<void (const ImP2PVideoInfo::P2P_VIDEO_INFORMATION& vInfo, bool bAccepted, 
							   ImP2PVideoInfo::VideoMsgStatus videoStatus,bool noRespone,IPropBagPtr spData)> _SIG_IM_VIDEO_REQUEST_EX;

DWDEFINE_INTERFACE(IImVideoChatMgrProxy) : IDWComponent
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual IImVideoChatProxyPtr initVideo(unsigned int peerId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual IImVideoChatProxyPtr lookUpVideoChat(unsigned int peerId) = 0;

	virtual ImP2PVideoInfo::Status getVideoState() = 0; 
};