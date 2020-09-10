#pragma once
#include <string>

namespace ImP2PVideoInfo
{
	enum VideoMsgStatus
	{
		VideoStatus_None, VideoStatus_Accepted, VideoStatus_Rejected, VideoStatus_Canceled,VideoStatus_PeerOffLine
	};

	enum Status
	{
		None	= 0x0,
		Invite	= 0x1,		// 发起邀请方状态
		Invited = 0x2,		// 被邀请，还没有接受
		Accepted = 0x4,		// 被邀请方接受了邀请
		Metting	 = 0x8		// 正在视频
	};

	enum VideoFrameType
	{
		I_FRAME,
		BP_FRAME,
	};

	enum StopVideoStatus
	{
		STOP_FROM_USER,
		STOP_AUTO,
		STOP_NO_RESPONE,
		STOP_ONLY_ONE_INSTANCE,
	};

	enum eVideoChanMethod
	{
		E_P2P_VIDEO_CHAN_METHOD_XAN = 0,
		E_P2P_VIDEO_CHAN_METHOD_BRIDGE,
		E_P2P_VIDEO_CHAN_METHOD_LINKD,

		E_P2P_VIDEO_CHAN_METHOD_UNKNOWN = 0xff			
	};


	// same as eP2PTransmitAction in common4/common/protocol/const.h
	enum P2PVideoAction
	{
		P2P_VIDEO_ACTION_ACCEPTED = 0,          // 对方已经接受
		P2P_VIDEO_ACTION_REFUSED,				// 对方已经拒绝
		P2P_VIDEO_ACTION_VIDEOING,				//视频进行中
		P2P_VIDEO_ACTION_START_CAMERA,//通知本方视频数据可以开始采集
		P2P_VIDEO_ACTION_STOP_CAMERA, //通知奔放视频数据需要停止采集
		P2P_VIDEO_ACTION_START_VIDEO, //底层传输准备就绪,请求开始采集I帧视频数据,每次任务只请求一次.
		P2P_VIDEO_ACTION_INIT_VIDEO, //通知应用层初始化以接收视频数据.
		P2P_VIDEO_CALL_CONN_SELF_IS_CONTROL,       //你为工信部管制用户
		P2P_VIDEO_CALL_CONN_PEER_IS_CONTROL,        //对方为工信部管制用户
		P2P_VIDEO_ACTION_NOT_SUPPORT = 0xff,     //对方版本不支持
	};

	//the same as protocol::p2p::eP2PAppAction
	enum eP2PUnionAppAction
	{
		E_P2P_ACTION_ACCEPT = 0,
		E_P2P_ACTION_CANCEL,
		E_P2P_ACTION_REJECT,
		E_P2P_ACTION_REMOVE,
		E_P2P_ACTION_RETRANSMIT,
		E_P2P_ACTION_DEVICE_STATE,
		E_P2P_ACTION_FLUSH_SENDQ,
		E_P2P_ACTION_CREATE_BACKUP_CHAN,
		E_P2P_ACTION_SPEED_TEST,
		E_P2P_ACTION_SWITCH_MAIN,
		E_P2P_ACTION_REPORT_STATICS,
		E_P2P_ACTION_NOTIFY_INIT_VIDEO,

		E_P2P_ACTION_UNKNOWN = 0xFFFFFFFF
	};

	// same as P2PTransmitStatus in common4/common/protocol/const.h
	enum P2PVideoStatus
	{
		P2P_VIDEO_STATUS_WAITING = 0,       //正在建立连接
		P2P_VIDEO_STATUS_READY,             //双方连接已建立，开始传输
		P2P_VIDEO_STATUS_FINISH,            //传输完成
		P2P_VIDEO_STATUS_IO_ERROR,          //文件系统错误			
		P2P_VIDEO_STATUS_NETWORK_ERROR,     //传输出现错误，任务已中断
		P2P_VIDEO_STATUS_FAILED,            //连接建立失败
		P2P_VIDEO_STATUS_CANCEL,            //连接被取消
		P2P_VIDEO_STATUS_PCONN_READY,       //预连接建立已建立.
		P2P_VIDEO_STATUS_PCONN_NOTREADY,    //预连接未建立.
		P2P_VIDEO_STATUS_DRIVER_ERROR,		//文件盘符异常.
		P2P_VIDEO_STATUS_TTL_WARN,			//发送队列杜塞严重
		P2P_VIDEO_TRANSMIT_BANDWIDTH,		//传输带宽通知.
		P2P_VIDEO_TRANSMIT_PUNCH_READY,      //打洞连接建立
		P2P_VIDEO_TRANSMIT_SERVER_READY,      //服务器中转连接建立
		P2P_VIDEO_TRANSMIT_SPEED_MONITOR,    //链路测试结果.
		P2P_VIDEO_TRANSMIT_SERVER_CONN_RES,  //服务器备用链路(通道1,2)建立请求的响应.
		P2P_VIDEO_TRANSMIT_NEW_MAINCHAN,		//通道切换主链路更新通知.
		P2P_VIDEO_STATUS_CHAN_READY,		//某传输通道已建立成功.

		P2PVIDEO_CAMERA_IN = 50,
		P2PVIDEO_CAMERA_OUT,
		P2PVIDEO_RES_UPDATE,
		P2PVIDEO_RES_PEER_UP,
		P2PVIDEO_RES_PEER_DN,

		P2P_VIDEO_STATUS_UNKNOW = 0xff
	};

	enum P2PVideoCameraStatus
	{
		P2P_VIDEO_CAMERA_IN,
		P2P_VIDEO_CAMERA_OUT,
		P2P_VIDEO_RESOLUTION_CHANGE,
		P2P_VIDEO_RESOLUTION_CHANGE_UP_PEER,
		P2P_VIDEO_RESOLUTION_CHANGE_DOWN_PEER,
		P2P_VIDEO_TTL_CHANGE,
	};

	enum eVideoQuality
	{
		videoDistinct,
		videoFluency
	};

	enum eExternReason
	{
		P2P_VIDEO_USER,
		P2P_VIDEO_NORESPONSE,
		P2P_VIDEO_AUTO,
		P2P_VIDEO_ONLY_ONE_INSTANCE
	};

	enum eVideoCustomValue
	{
		P2P_NONE = 0x0, // 未初始化
		P2P_SUPPORT_NEW_VIDEO_CODEC = 0x1, // 普通imp2p视频(注意！！！！后面新增类型值都必须 &0x1 != 0)
		P2P_KEFU_NEW_VIDEO_CODE = 0x3      // 客服发起imp2p视频(0x2&0x1保持第一位的值)
	};

	// same as P2PVideoTransmitInfo in common4/common/protocol/IImP2PVideo.h
	struct P2P_VIDEO_INFORMATION
	{
		//对端信息
		std::string m_strGUID;				//传输任务ID
		unsigned int m_uMyId;               //本方ID
		unsigned int m_uPeerID;				//对方ID
		//传输信息
		bool m_bMeCamera;                 	//是否有Camera
		bool m_bPeerCamera;
		bool m_bRequest;
		unsigned int m_uMyWidth;
		unsigned int m_uMyHeigh;
		unsigned int m_uPeerWidth;
		unsigned int m_uPeerHeigh;
		unsigned int m_uTTL;
		unsigned short m_wFrameRate;

		//not marshal.
		unsigned int m_uBandwidth;
		unsigned int m_uTransmitRtt;
		bool m_isPeerSuppUnionChan;
		unsigned short m_wChanNum;//Just for ui,no need to marshal. best xchan selection.
		unsigned short m_wServerHops;//record how many servers on the whole way,as servers forward.
		std::string m_strServerip;//access server ip address.
		eVideoChanMethod m_eChanMethod;
		unsigned int m_uAppCustomValue;

		P2P_VIDEO_INFORMATION()
		{
			m_uMyId = 0;
			m_uPeerID = 0;
			m_bMeCamera = false;
			m_bPeerCamera = false;
			m_bRequest = false;
			m_uTTL = (unsigned int)-1;
			m_uMyWidth = m_uMyHeigh = m_uPeerWidth = m_uPeerHeigh = 0;
			m_uBandwidth = 0;
			m_uTransmitRtt = 0;
			m_isPeerSuppUnionChan = false;
			m_wChanNum = 0xffff;
			m_wServerHops = 0;
			m_strServerip = "";
			m_wFrameRate = 0;
			m_eChanMethod = E_P2P_VIDEO_CHAN_METHOD_UNKNOWN;
			m_uAppCustomValue = 0;
		}
	};



	struct reoprtChannelInfo
	{
		short serverNum;
		bool bPunch;
		eVideoChanMethod chanMethod;
		short serverhots;

		reoprtChannelInfo()
		{
			serverNum = 0;
			bPunch = 0;
			chanMethod = E_P2P_VIDEO_CHAN_METHOD_UNKNOWN;
			serverhots = 0;
		}
	};

	struct videoResolution
	{
		unsigned short width;
		unsigned short height;
		unsigned short sampleTime; //定时器

		videoResolution()
		{
			width = 0;
			height = 0;
			sampleTime = 0;
		}

		bool operator == (const videoResolution &right) const
		{
			return height == right.height && width == right.width && sampleTime == right.sampleTime;
		}

		bool operator != (const videoResolution &right) const
		{
			return !(operator == (right));
		}

		videoResolution & operator = (const videoResolution &right)
		{
			if ( this == &right)
			{
				return *this;
			}
			width = right.width;
			height = right.height;
			sampleTime = right.sampleTime;
			return *this;
		}
	};

}

