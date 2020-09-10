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
		Invite	= 0x1,		// �������뷽״̬
		Invited = 0x2,		// �����룬��û�н���
		Accepted = 0x4,		// �����뷽����������
		Metting	 = 0x8		// ������Ƶ
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
		P2P_VIDEO_ACTION_ACCEPTED = 0,          // �Է��Ѿ�����
		P2P_VIDEO_ACTION_REFUSED,				// �Է��Ѿ��ܾ�
		P2P_VIDEO_ACTION_VIDEOING,				//��Ƶ������
		P2P_VIDEO_ACTION_START_CAMERA,//֪ͨ������Ƶ���ݿ��Կ�ʼ�ɼ�
		P2P_VIDEO_ACTION_STOP_CAMERA, //֪ͨ������Ƶ������Ҫֹͣ�ɼ�
		P2P_VIDEO_ACTION_START_VIDEO, //�ײ㴫��׼������,����ʼ�ɼ�I֡��Ƶ����,ÿ������ֻ����һ��.
		P2P_VIDEO_ACTION_INIT_VIDEO, //֪ͨӦ�ò��ʼ���Խ�����Ƶ����.
		P2P_VIDEO_CALL_CONN_SELF_IS_CONTROL,       //��Ϊ���Ų������û�
		P2P_VIDEO_CALL_CONN_PEER_IS_CONTROL,        //�Է�Ϊ���Ų������û�
		P2P_VIDEO_ACTION_NOT_SUPPORT = 0xff,     //�Է��汾��֧��
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
		P2P_VIDEO_STATUS_WAITING = 0,       //���ڽ�������
		P2P_VIDEO_STATUS_READY,             //˫�������ѽ�������ʼ����
		P2P_VIDEO_STATUS_FINISH,            //�������
		P2P_VIDEO_STATUS_IO_ERROR,          //�ļ�ϵͳ����			
		P2P_VIDEO_STATUS_NETWORK_ERROR,     //������ִ����������ж�
		P2P_VIDEO_STATUS_FAILED,            //���ӽ���ʧ��
		P2P_VIDEO_STATUS_CANCEL,            //���ӱ�ȡ��
		P2P_VIDEO_STATUS_PCONN_READY,       //Ԥ���ӽ����ѽ���.
		P2P_VIDEO_STATUS_PCONN_NOTREADY,    //Ԥ����δ����.
		P2P_VIDEO_STATUS_DRIVER_ERROR,		//�ļ��̷��쳣.
		P2P_VIDEO_STATUS_TTL_WARN,			//���Ͷ��ж�������
		P2P_VIDEO_TRANSMIT_BANDWIDTH,		//�������֪ͨ.
		P2P_VIDEO_TRANSMIT_PUNCH_READY,      //�����ӽ���
		P2P_VIDEO_TRANSMIT_SERVER_READY,      //��������ת���ӽ���
		P2P_VIDEO_TRANSMIT_SPEED_MONITOR,    //��·���Խ��.
		P2P_VIDEO_TRANSMIT_SERVER_CONN_RES,  //������������·(ͨ��1,2)�����������Ӧ.
		P2P_VIDEO_TRANSMIT_NEW_MAINCHAN,		//ͨ���л�����·����֪ͨ.
		P2P_VIDEO_STATUS_CHAN_READY,		//ĳ����ͨ���ѽ����ɹ�.

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
		P2P_NONE = 0x0, // δ��ʼ��
		P2P_SUPPORT_NEW_VIDEO_CODEC = 0x1, // ��ͨimp2p��Ƶ(ע�⣡������������������ֵ������ &0x1 != 0)
		P2P_KEFU_NEW_VIDEO_CODE = 0x3      // �ͷ�����imp2p��Ƶ(0x2&0x1���ֵ�һλ��ֵ)
	};

	// same as P2PVideoTransmitInfo in common4/common/protocol/IImP2PVideo.h
	struct P2P_VIDEO_INFORMATION
	{
		//�Զ���Ϣ
		std::string m_strGUID;				//��������ID
		unsigned int m_uMyId;               //����ID
		unsigned int m_uPeerID;				//�Է�ID
		//������Ϣ
		bool m_bMeCamera;                 	//�Ƿ���Camera
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
		unsigned short sampleTime; //��ʱ��

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

