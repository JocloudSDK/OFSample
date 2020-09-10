#pragma once
#include "stable.h"
#include "biz/biz_types.h"
enum ResponseCode
{
	kRespOk               = 0,  // 服务正常
	kRespBusy             = 1,  // 服务忙，暂不处理客户端请求，客户端应暂停发送请求一段时间
	kRespSystemErr        = 2,  // 系统错误
	kRespSystemUpgrade    = 3,  // 系统升级
	kRespNoRoom           = 4,  // 没有视频空间
	kRespPermissionDenied = 5,  // 没有权限 
	kRespNotFound			= 6,  // 不存在
	kRespExist				= 7,  // 重复
};

struct Performer
{
	Performer():uid(0){}
	uint    uid;
	QString nick;
};

struct SimpleVodInfo
{
	QString program_id;                           // “视频标识”
	QString channel_name;                         // “频道名称” utf-8  
	QString uploader;                             // “上传者”昵称 utf-8
	uint	    app_id;                               //  “视频源”  
};

struct DetailedVodInfo
{
	DetailedVodInfo()
		:app_id(0),anchor_rank(0),is_open_camera(0)
	{}

	QString program_id;                           // “视频标识”
	QString channel_name;                         // “频道名称” utf-8  
	QString uploader;                             // “上传者”昵称 utf-8
	uint	    app_id;                               //  “视频源”  
	std::vector<Performer> performer_infos;       // “表演者信息”集合
	QString activity_name;                        // “节目名称” 只有娱乐有这个信息  utf-8
	QString type;                                 // “大类”
	QString two_level_type;                       // “二级分类”
	QString three_level_type;                     // “三级分类”  
	QString title;                                // “标题”
	std::vector<QString> tags;                    // “标签”
	uint     anchor_rank;                          // “主播等级”
	bool     is_open_camera;                         // “是否开启摄像头”
	QString expand;                               // “用于扩展” 
};

struct CollectInfo
{
	CollectInfo()
		:app_id(0),anchor_rank(0)
	{}

	QString collection_id;                         // “视频标识”
	QString channel_name;                         // “频道名称” utf-8  
	QString collector;                             // “收藏者”昵称 utf-8
	uint    app_id;                               //  “视频源”  
	std::vector<Performer> performer_infos;       // “表演者信息”集合
	QString activity_name;                        // “节目名称” 只有娱乐有这个信息  utf-8
	QString type;                                 // “大类”
	QString two_level_type;                       // “二级分类”
	QString three_level_type;                     // “三级分类”  
	QString title;                                // “标题”
	uint     anchor_rank;                          // “主播等级”
	std::string expand;                               // “用于扩展”  

};

DWDEFINE_INTERFACE(IYYVideoOnDemand):public IUnk
{
public:
	typedef std::vector<Performer> Performers;
	typedef std::vector<QString> Tags;
	typedef xsignals::signal<void (int res)> _SIG_RECORD_RULE_; // ResponseCode

	virtual void load() = 0; // 初始化
	virtual QString programId(quint32 appid) = 0; // 生成videoSdk录制用的programId
	virtual QString backstageId(quint32 appid) = 0; // 生成后台录制用的programId
	virtual QString collectionId(quint32 appid) = 0; // 生成收藏用的colectionId
	virtual void checkPermission() = 0; //判断是否有录播权限 结果:见sigPermission();
	virtual void checkStorage() = 0; // 判断是否有足够空间 结果:见sigStorage()
	virtual void recordStarted(const SimpleVodInfo &info) = 0; // 开始录制时调用
	virtual void recordStoped(const DetailedVodInfo &info) = 0; // 结束录制时调用
	virtual void collectVideo(const CollectInfo& info) = 0;	// 收藏视频
	virtual _SIG_RECORD_RULE_ &sigPermission() = 0;
	virtual _SIG_RECORD_RULE_ &sigStorage() = 0;
	virtual _SIG_RECORD_RULE_ &sigCollectVideo() = 0; // 收藏结果
};
