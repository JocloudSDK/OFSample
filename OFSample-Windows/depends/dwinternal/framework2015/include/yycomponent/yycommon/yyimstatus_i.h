#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"
#include "propbagsetwapper_i.h"

/**
* @brief IM status interface.
IYYImQueryUserInfoPtr minCardPtr = m_comRoot->getServiceInterface<IYYImQueryUserInfo>( KSERVICECommon );

*/	
class QImage;
DWDEFINE_INTERFACE( IYYIMStatus ) : public IUnk
{
	/**
	* @brief 批量查询用户的在线状态.
	* @param uidList 用户uid列表.
	* @return .
	*/	
	virtual void queryImStatus(const std::vector<UINT32> &uidList) = 0;

	typedef _def_boost_signals2_signal_type<void (const std::map<UINT32, IMSTATUS>& status, const std::map<UINT32, std::string>& context, const UINT32 interval)>::type	_SIG_IM_STATUS;
	virtual _SIG_IM_STATUS* getImStatusResult() = 0;
};


DWDEFINE_INTERFACE( IYYImQueryUserInfo ) : public IDWQComponent
{
	struct UI_SImUinfoSimpleEx
	{
		UINT32 m_uId;
		UINT32 m_imId;
		std::wstring m_nickname;
		GENDER m_sex;
		IMSTATUS m_status;
		UINT32 m_photoIndex;
		std::wstring m_sign;
	};

	struct UI_SPhotoPub
	{
		UINT32 m_uId;
		UINT32 m_uFormat;
		UINT32 m_uPhotoIndex;
		std::wstring m_strPhotoUrl;
	};

	enum UI_EHDLogoType
	{
		IM_HD_60_60 = 0,
		IM_HD_100_100 ,
		IM_HD_144_144,
		IM_HD_640_640, // webYY头像
		IM_HD_UNKNOWN = 0xff
	};

	/**
	* @brief 批量查询用户simple user info，直接调用protocol接口.
	* @param vecUid 用户uid列表.
	* @param mapContext 与uid关联的context.
	* @return .
	*/	
	virtual void querySimpleUinfoEx(const std::vector<UINT32>& vecUid, const std::map<UINT32, std::string>& mapContext) = 0;

	/**
	* @brief 批量查询用户simple user info, 优化querySimpleUinfoEx.
	* @param vecUid 用户uid列表.
	* @param mapContext 与uid关联的context.
	* @return .
	*/
	virtual void getUserInfo( const UINT32 userId, const std::string& context ) = 0;

	/**
	* @brief 批量取得头像图片url等信息.
	* @param vecUid 用户uid列表.
	* @param mapContext 与uid关联的context.
	* @return .
	*/
	virtual void getPhotoListPub(
		const std::vector<UINT32>& vecUid, 
		UINT32 uFormat, 
		const std::map<UINT32, std::string>& mapContext
		) = 0;

	/**
	* @brief 得到自定义头像的象素数据，如果缓存中有，则直接返回图片，如果不存在，则下载成功后以getcustomHeadImageDownloadCompileSignal事件通知.
	* @param uBuddyID 用户uid.
	* @param url 自定义头像的web路径.
	* @param bOnline 是否要在线状态，如果为false, 则会自动给灰图.
	* @return 自定义头像.
	*/
	virtual QImage getcustomHeadImageData(UINT32 uBuddyID, const QString url, bool bOnline) = 0;

	/**
	* @brief 得到系统头像的象素数据.
	* @param index 系统头像index.
	* @param bOnline 是否要在线状态，如果为false, 则会自动给灰图.
	* @return 系统头像.
	*/
	virtual QImage getsystemHeadImageData(UINT32 index, bool bOnline) = 0;

	/**
	* @brief 得到动态头像的象素数据，如果缓存中有，则直接返回图片，如果不存在，则下载成功后以getDynamicHeadImageDownloadCompileSignal事件通知.
	* @param uBuddyID 用户uid.
	* @param url 自定义头像的web路径.
	* @return 动态头像二进制数据.
	*/
	virtual QByteArray getDynamicHeadImageData(UINT32 uBuddyID, const QString url) = 0;

	/**
	* @brief 批量查询用户simple user info服务器返回结果.
	* @param vecData 用户信息返回结果列表.
	* @param mapContext 用户信息相关的上下文.
	* @return 事件对象.
	*/
	typedef _def_boost_signals2_signal_type<void (const std::vector<UI_SImUinfoSimpleEx>& vecData, const std::map<UINT32, std::string>& mapContext)>::type	_SIG_SIMPLE_USERINFOEX;
	virtual _SIG_SIMPLE_USERINFOEX* getSimpleUserInfoExSignal() = 0;

	/**
	* @brief 下载自定义头像返回结果.
	* @param userId 用户uid.
	* @return 事件对象.
	*/
	typedef _def_boost_signals2_signal_type<void (const UINT32 userId)>::type	_SIG_DOWNLOAD_COMPILE;
	virtual _SIG_DOWNLOAD_COMPILE* getcustomHeadImageDownloadCompileSignal() = 0;

	/**
	* @brief 下载动态头像返回结果.
	* @return 事件对象.
	*/
	virtual _SIG_DOWNLOAD_COMPILE* getDynamicHeadImageDownloadCompileSignal() = 0;

	/**
	* @brief  批量查询用户simple user info服务器返回结果.
	* @param vecData 用户信息返回结果列表.
	* @param mapContext 用户信息相关的上下文.
	* @return 事件对象.
	*/
	typedef _def_boost_signals2_signal_type<void (const std::vector<UI_SPhotoPub>& vecData, const std::map<UINT32, std::string>& mapContext)>::type	_SIG_GETPHOTOLISTPUBRES;
	virtual _SIG_GETPHOTOLISTPUBRES* getPhotoListPubResSignal() = 0; 


    enum USER_INFO_MASK
    {
        UI_MASK_Nick        = 1,
        UI_MASK_Sex         = (1 << 1),
        UI_MASK_Sign        = (1 << 2),
        UI_MASK_Intro       = (1 << 3),
        UI_MASK_Resume      = (1 << 4),
        UI_MASK_Birth       = (1 << 5),
        UI_MASK_Area        = (1 << 6),
        UI_MASK_Province    = (1 << 7),
        UI_MASK_City        = (1 << 8),
        UI_MASK_Jifen       = (1 << 9),
        UI_MASK_Imid        = (1 << 10),
        UI_MASK_State       = (1 << 11),
        UI_MASK_Photoindex  = (1 << 12),
    };

    /**
    * @brief 查询用户信息，请求会缓存，一定时间后批量查询；或者当请求数超过一定量会立即查询.
    * @param uAppId 组件id.
    * @param uUserId 用户im id.
    * @param uMask USER_INFO_MASK字段的集合，表示需要取哪方面的信息.
    * @return 
    */
    virtual void getUserInfo2(UINT32 uAppId, UINT32 uUserId, UINT32 uMask) = 0;

    /**
    * @brief 批量查询用户信息，请求会缓存，一定时间后批量查询；或者当请求数超过一定量会立即查询.
    * @param uAppId 组件id.
    * @param vedIds 用户im id集合.
    * @param uMask USER_INFO_MASK字段的集合，表示需要取哪方面的信息.
    * @return 
    */
    virtual void getUserInfos(UINT32 uAppId, const std::vector<UINT32>& vecIds, UINT32 uMask) = 0;

    /**
    * @brief 接口getUserInfo2/getUserInfos返回的信号.
    * @param 应用ID.
    * @param 用户信息数据集, 以用户id(uid)作为ID.
    * @param USER_INFO_MASK字段的集合，表示数据集中包含哪方面的信息.
    * @return 
    */
    typedef _def_boost_signals2_signal_type<void (UINT32, IPropBagSetWrapperPtrCR, UINT32)>::type _SIG_USER_INFO_RES;
    virtual _SIG_USER_INFO_RES* getUserInfoResSignal() = 0;

    /**
    * @brief 查询用户头像，请求会缓存，一定时间后批量查询；或者当请求数超过一定量会立即查询.
    * @param uAppId 组件id.
    * @param uUserId 用户im id.
    * @param eType 头像类型.
    * @return 
    */
    virtual void getHeadSculpture(UINT32 uAppId, UINT32 uUserId, UI_EHDLogoType eType) = 0;
    
    /**
    * @brief 批量查询用户头像，请求会缓存，一定时间后批量查询；或者当请求数超过一定量会立即查询.
    * @param uAppId 组件id.
    * @param vedIds 用户im id集合.
    * @param eType 头像类型.
    * @return 
    */
    virtual void getHeadSculptures(UINT32 uAppId, const std::vector<UINT32>& vecIds, UI_EHDLogoType eType) = 0;

    /**
    * @brief 接口getHeadSculpture/getHeadSculptures返回的信号.
    * @param 应用ID.
    * @param 用户信息数据集, 以用户id(uid)作为ID.
    * @return 
    */
    typedef _def_boost_signals2_signal_type<void (UINT32, IPropBagSetWrapperPtrCR)>::type _SIG_HEAD_SCULPTURE_RES;
    virtual _SIG_HEAD_SCULPTURE_RES* getHeadSculptureResSignal() = 0;

	/**
	* @brief 批量查询用户simple user info服务器返回结果(对比getSimpleUserInfoExSignal，增加艺名信息).
	* @param IPropBagSetWrapperPtrCR 用户信息返回结果列表,以用户id(uid)作为ID.
	* @param mapContext 用户信息相关的上下文.
	* @return 事件对象.
	*/
	typedef _def_boost_signals2_signal_type<void(IPropBagSetWrapperPtrCR, const std::map<UINT32, std::string>& mapContext)>::type	_SIG_SIMPLE_USERINFOEX2;
	virtual _SIG_SIMPLE_USERINFOEX2* getSimpleUserInfoExSignal2() = 0;

	/**
	* @brief 通过Webdb查询支持大量触发，通过批量查询用户信息（区别于querySimpleUinfoEx/getSimpleUserInfoExSignal2向im服务查询）
	* !!!!!!!!一次请求不能超过400个uid,否则直接返回一个空的回复!!!!!!!!!!
	* @param uidList ：批量用户uid集合.
	* @param isGetAll：是否取所有信息.
	* @param props   ：需要获取的属性标签.
	****props包含的string属性内容，以下为（isGetAll=true）***
	"area"：国家id
	"birthday"：生日(19900101)
	"city"：城市
	"custom_logo"：60*60
	"hd_logo_100"：100*100
	"hd_logo_144"：144*144
	"hd_logo_640"：640*640，webyy头像
	"id"：用户uid
	"intro"：个人说明
	"jifen"：积分
	"logo_index"：系统头像index
	"nick"：昵称
	"provice"：省份
	"sex"：性别
	"sign"：个性签名
	"stage_name"：艺名
	"yyno"：用户YY号
	* @return ：回调信息getUsersInfosByProtocolSignal
	*/
	virtual void queryUsersInfosByProtocol(const std::vector<UINT32>& uidList, bool isGetAll, const std::vector<std::string>& props) = 0;

	/**
	* @brief 批量查询用户queryUsersInfosByProtocolo服务器返回结果.
	* @param UINT32 返回结果, 0为成功，非0为失败.
	* @param const std::string& 用户信息json串，
	eg:{"uinfos":
	["{\"area\":\"0\",\"birthday\":\"20101128\",\"city\":\"-1\",\"custom_logo\":\"xxx.png\",\"hd_logo_100\":\"http:\\\/\\\/downhdlogo.yy.com\\\/hdlogo\\\/100100\\\/31\\\/69\\\/25\\\/0002256931\\\/u2256931I2uBGl-NVW.png\",\"hd_logo_144\":\"xxx.png\",\"hd_logo_640\":\"xxx.png\",\"id\":\"2256931\",\"intro\":\"\",\"jifen\":\"875711\",\"logo_index\":\"0\",\"nick\":\"LEE",\"province\":\"-1\",\"sex\":\"1\",\"sign\":\"\",\"stage_name\":\"\",\"yyno\":\"305549\"}",
	"{\"area\":\"352\",\"birthday\":\"19990808\",\"city\":\"-1\",\"id\":\"3660707\",\"intro\":\"\\n xxx\",\"jifen\":\"4488539\",\"logo_index\":\"0\",\"nick\":\"锕忋倿鐒伴緧銈沑",\"province\":\"-1\",\"sex\":\"1\",\"sign\":\"xxx\",\"stage_name\":\"\",\"yyno\":\"101338\"}"
	]}
	* @return 事件对象.
	*/
	typedef _def_boost_signals2_signal_type<void(UINT32, const std::string&)>::type	_SIG_GET_USERSINFOS_BYPROTOCOL;
	virtual _SIG_GET_USERSINFOS_BYPROTOCOL* getUsersInfosByProtocolSignal() = 0;
};
