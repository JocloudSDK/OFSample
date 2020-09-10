#pragma once
#include "stable.h"
#include "biz/biz_types.h"
enum ResponseCode
{
	kRespOk               = 0,  // ��������
	kRespBusy             = 1,  // ����æ���ݲ�����ͻ������󣬿ͻ���Ӧ��ͣ��������һ��ʱ��
	kRespSystemErr        = 2,  // ϵͳ����
	kRespSystemUpgrade    = 3,  // ϵͳ����
	kRespNoRoom           = 4,  // û����Ƶ�ռ�
	kRespPermissionDenied = 5,  // û��Ȩ�� 
	kRespNotFound			= 6,  // ������
	kRespExist				= 7,  // �ظ�
};

struct Performer
{
	Performer():uid(0){}
	uint    uid;
	QString nick;
};

struct SimpleVodInfo
{
	QString program_id;                           // ����Ƶ��ʶ��
	QString channel_name;                         // ��Ƶ�����ơ� utf-8  
	QString uploader;                             // ���ϴ��ߡ��ǳ� utf-8
	uint	    app_id;                               //  ����ƵԴ��  
};

struct DetailedVodInfo
{
	DetailedVodInfo()
		:app_id(0),anchor_rank(0),is_open_camera(0)
	{}

	QString program_id;                           // ����Ƶ��ʶ��
	QString channel_name;                         // ��Ƶ�����ơ� utf-8  
	QString uploader;                             // ���ϴ��ߡ��ǳ� utf-8
	uint	    app_id;                               //  ����ƵԴ��  
	std::vector<Performer> performer_infos;       // ����������Ϣ������
	QString activity_name;                        // ����Ŀ���ơ� ֻ�������������Ϣ  utf-8
	QString type;                                 // �����ࡱ
	QString two_level_type;                       // ���������ࡱ
	QString three_level_type;                     // ���������ࡱ  
	QString title;                                // �����⡱
	std::vector<QString> tags;                    // ����ǩ��
	uint     anchor_rank;                          // �������ȼ���
	bool     is_open_camera;                         // ���Ƿ�������ͷ��
	QString expand;                               // ��������չ�� 
};

struct CollectInfo
{
	CollectInfo()
		:app_id(0),anchor_rank(0)
	{}

	QString collection_id;                         // ����Ƶ��ʶ��
	QString channel_name;                         // ��Ƶ�����ơ� utf-8  
	QString collector;                             // ���ղ��ߡ��ǳ� utf-8
	uint    app_id;                               //  ����ƵԴ��  
	std::vector<Performer> performer_infos;       // ����������Ϣ������
	QString activity_name;                        // ����Ŀ���ơ� ֻ�������������Ϣ  utf-8
	QString type;                                 // �����ࡱ
	QString two_level_type;                       // ���������ࡱ
	QString three_level_type;                     // ���������ࡱ  
	QString title;                                // �����⡱
	uint     anchor_rank;                          // �������ȼ���
	std::string expand;                               // ��������չ��  

};

DWDEFINE_INTERFACE(IYYVideoOnDemand):public IUnk
{
public:
	typedef std::vector<Performer> Performers;
	typedef std::vector<QString> Tags;
	typedef xsignals::signal<void (int res)> _SIG_RECORD_RULE_; // ResponseCode

	virtual void load() = 0; // ��ʼ��
	virtual QString programId(quint32 appid) = 0; // ����videoSdk¼���õ�programId
	virtual QString backstageId(quint32 appid) = 0; // ���ɺ�̨¼���õ�programId
	virtual QString collectionId(quint32 appid) = 0; // �����ղ��õ�colectionId
	virtual void checkPermission() = 0; //�ж��Ƿ���¼��Ȩ�� ���:��sigPermission();
	virtual void checkStorage() = 0; // �ж��Ƿ����㹻�ռ� ���:��sigStorage()
	virtual void recordStarted(const SimpleVodInfo &info) = 0; // ��ʼ¼��ʱ����
	virtual void recordStoped(const DetailedVodInfo &info) = 0; // ����¼��ʱ����
	virtual void collectVideo(const CollectInfo& info) = 0;	// �ղ���Ƶ
	virtual _SIG_RECORD_RULE_ &sigPermission() = 0;
	virtual _SIG_RECORD_RULE_ &sigStorage() = 0;
	virtual _SIG_RECORD_RULE_ &sigCollectVideo() = 0; // �ղؽ��
};
