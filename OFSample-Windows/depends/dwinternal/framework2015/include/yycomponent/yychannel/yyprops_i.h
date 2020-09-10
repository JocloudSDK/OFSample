#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"
#include "yycomponent/yycomclsid.h"

// 道具信息接口.
DWDEFINE_INTERFACE(IPropsInfo) : public IUnk
{
	virtual quint32 getId(void) = 0;
	
	virtual QString getName(void) = 0;
	
	virtual QString	 getIconUrl(void) = 0;
	
	virtual quint32	 getAppId(void) = 0;
	
	//获取价列表.
	virtual const QMap<quint32,quint32>& getPriceList(void) = 0;
	
	//获取描述信息.
	virtual const QMap<QString,QString>& getDescriptionList(void) = 0;
};


// 道具系统接口.
DWDEFINE_INTERFACE( IYYProps ) : public IUnk
{
	//返回码定义说明.
	enum ResponseCode
	{
		kRespOK = 1, //成功.
		kRespIllegalAccount = -1,//账户非法（封禁之类的）.
		kRespUnknownAppID = -2,//不存在的业务(不存在的appId).
		kRespInSufficientBalance = -3,//该账户余额不足.
		kRespUnknownPropsID = -4,//不存在的propsId或道具已下架.
		kRespCurrencyAccountRestrictions = -5,//不能使用、增值的货币账户.
		kRespNotExistOrSufficientSum = -6,//没有这个道具或数量不足.
		KRespRechargeActivityCurrencyLimit = -7,//活动货币自动增值达到每日上限.
		KRespTimeAuthenticationFailed = -8,//时间验证失败，只有充值接口返回这个错误号.
		KRespNeedConfirm = -10,//用rmb购买需要确认
		kRespParamError = -400,//参数错误，特指参数缺失.
		kRespAuthenticationFailed = -401,//身份鉴权失败.
		kRespNoAuthority = -403,//没有权限（暂无用，保留）.
		kRespUnknowCmd = -404,//未知cmd.
		kRespSystemError = -500//服务端错误.
	};

	// 货币类型枚举.
	enum CurrencyType
	{
		CT_Unknow = 0,
		Virtual=1, // 虚拟货币，紫水晶.
		Real, // 提现货币，黄水晶.
		Activity, // 活动货币，白水晶.
		Yb,   // Y币，系统会自动根据汇率转换为紫水晶货币.
		Time  // 时长也作为一种货币，单位秒，自动根据汇率转换为白水晶货币.
	};

	// 道具类型枚举.
	enum PropsType
	{
		PT_Unknow = 0,
		Free,// 免费道具.
		Premium // 收费道具.
	};

	//道具用户账户信息.
	struct Account
	{
		quint32 uid;
		CurrencyType currencyType;
		quint32 amount;
		quint32 freezed;
		Account() : uid(0), currencyType(CT_Unknow), amount(0), freezed(0){}
	};

	// 道具信息汇总.
	struct Props
	{
		quint32				id;
		QString				name;
		QString				iconUrl;// 图标地址.
		quint32				appid; // 业务方标识.
		QMap<CurrencyType,quint32>			priceList;//不同货币类型的定价列表.
		Props() : id(0), appid(0){}
	};

	// 用户持有道具.
	struct UserProps
	{
		quint32	uid;
		quint32	propsId;
		quint32	propsCount;
		UserProps() : uid(0), propsCount(0), propsId(0){}
	};

	//用户道具信息所有汇总对象.
	struct User
	{
		quint32		uid;
		QList<Account> accounts;
		QList<UserProps> userPropsList;	
		User() : uid(0){}
	};

	//道具用户信息.
	struct PropsUserInfo
	{
		quint32 uid;
		QString imId;
		QString nickName;
		PropsUserInfo() : uid(0){}
	};

	//购买道具信息.
	struct ConsumePropsInfo
	{
		quint32 propsId;
		quint32 count; //购买道具数量.
		CurrencyType currencyType;//购买道具所使用的货币.
		double costMoney;//购买道具总共用了多少钱，货币是什么类型，就什么为单位.
	};

public:

	//先设置业务ID.
	//财经是1，交友是2，以一敌百是3.
	//注意：此接口除了(财经是，交友是，以一敌百是)外其他模版不可以使用.
	virtual void setAppId(quint32 nAppId) = 0;

	//通过指定uid，获取已获得赠送的道具数量.
	virtual void asyncGetUserGiftPropsByUid(quint32 uid) = 0;

	//获取我自己所有道具信息.
	virtual void asyncGetMyAllProps(void) = 0;

	//获取某个业务的所有道具信息和定价.
	//业务在setAppId接口被指定.
	virtual void asyncGetAllProps(void) = 0;

	// 购买道具
	//expandContent 扩展内容，作透传用，不用的话就填空.
	virtual void asyncConsumeProps(quint32 propsId, quint32 count,CurrencyType currencyType,const QString& expandContent) = 0;

	//充值（活动货币外部增值）.
	//currencyType: 货币类型:1:虚拟 2:提现 3:活动 4:Y币 5:时间货币.
	//amount: 数量.
	//比如财经的褔豆增加asyncReChargeProps(IYYProps::Activity,20).
	virtual void asyncReChargeProps(CurrencyType currencyType, quint32 amount) = 0;

	//使用道具(业务服务器service通道).
	//这接口是通过service通道访问业务服务器实现.
	//expandContent 扩展内容，各业务可根据业务增加内容，比如说赠送道具的留言.
	virtual void asyncUseProps(quint32 recvUid,quint32 propsId, quint32 count,const QString& expandContent) = 0;

	//指定uid已赠送的道具数量返回通知信号.
	//参数1，指返回错误码.
	//参数2，指定要查找的用户id.
	//参数3，收到的道具列表.
	typedef _def_boost_signals2_signal_type<void (qint32, quint32,const QMap<quint32,quint32>&)>::type _SIG_GET_USER_GIFT_PROPS_BY_UID;
	virtual _SIG_GET_USER_GIFT_PROPS_BY_UID* getUserGiftPropsByUidSignal() = 0;

	//获取所有道具信息结果返回通知信号.
	//参数1, 指返回错误码.
	//参数2, 用户持有的道具信息.
	typedef _def_boost_signals2_signal_type<void (qint32, const IYYProps::User&)>::type _SIG_GET_MY_ALL_PROPS;
	virtual _SIG_GET_MY_ALL_PROPS* getMyAllPropsSignal() = 0;

	//获取某个业务的所有道具信息和定价通知信号.
	//参数1, 指返回错误码.
	typedef _def_boost_signals2_signal_type<void (qint32, const QList<IYYProps::Props>&)>::type _SIG_GET_ALL_PROPS;
	virtual _SIG_GET_ALL_PROPS* getAllPropsSignal() = 0;

	//购买道具确认通知信号.
	//当用户的购买设置了购买二次选项时，调用购买道具接口，会触发此信号，当用户确认后会触发getConsumePropsSignal().
	//如果没有这个设置直接触发getConsumePropsSignal().
	//参数1, 指返回错误码,错误码=-10时，需要用户确认弹出confirmUrl.
	//参数2, 确认url.
	typedef _def_boost_signals2_signal_type<void (qint32,const QString&)>::type _SIG_CONSUME_PROPS_CONFIRM;
	virtual _SIG_CONSUME_PROPS_CONFIRM* getConsumePropsConfirmSignal() = 0;

	//购买道具成功通知信号.
	//返回购买结果，1表示购买成功
	//参数1, 指返回错误码,会出现的业务错误码：-1、-3、-4、-5.
	//参数2, 道具id.
	//参数3, 道具数量.
	//参数4, 扩展，默认为空.
	typedef _def_boost_signals2_signal_type<void (qint32,quint32,qint32,const QString&)>::type _SIG_CONSUME_PROPS;
	virtual _SIG_CONSUME_PROPS* getConsumePropsSignal() = 0;

	//充值（活动货币外部增值）通知信号.
	//参数1, 指返回错误码.
	//参数2, 货币类型:1:虚拟 2:提现 3:活动 4:Y币 5:时间货币.
	//参数3, 数量.
	typedef _def_boost_signals2_signal_type<void (qint32,IYYProps::CurrencyType,quint32)>::type _SIG_RECHARGE_PROPS;
	virtual _SIG_RECHARGE_PROPS* getReChargePropsSignal() = 0;

	//使用道具通知信号.
	//参数1, 指返回错误码，返回1指成功，返回0指失败，返回-6指没有这个道具或数量不足.
	typedef _def_boost_signals2_signal_type<void (qint32)>::type _SIG_USE_PROPS;
	virtual _SIG_USE_PROPS* getUsePropsSignal() = 0;

	//接收道具通知信号，只有接收首方才会收到这个通知.
	//参数1, 送出道具用户的id.
	//参数2, 道具id.
	//参数3, 道具数量.
	//参数4, 扩展内容.
	typedef _def_boost_signals2_signal_type<void (quint32,qint32,qint32,const QString&)>::type _SIG_USE_PROPS_NOTICE;
	virtual _SIG_USE_PROPS_NOTICE* getUsePropsNoticeSignal() = 0;

	//送出道具广播通知信号.
	//参数1，道具使用者用户信息.
	//参数2，道具接收者用户信息.
	//参数3，道具id.
	//参数4，道具数量.
	//参数5 , 扩展内容.
	typedef _def_boost_signals2_signal_type<void (const IYYProps::PropsUserInfo&, const IYYProps::PropsUserInfo&, quint32, quint32,const QString&)>::type _SIG_USE_PROPS_BROADCAST;
	virtual _SIG_USE_PROPS_BROADCAST* getUsePropsBroadcastSignal() = 0;

	//初始化.
	//propsAppId 道具系统分配的id.
	//serviceId	业务服务器service通道id,如果不需要业务服务器，可以传0.
	virtual void create(quint32 propsAppId,quint32 serviceId) = 0;

	//直接使用道具(im通道).
	//这接口是直接通过im通道访问道具系统服务器实现.
	//expandContent 扩展内容，各业务可根据业务增加内容，比如说赠送道具的留言.
	virtual void asyncDirectUseProps(quint32 recvUid,quint32 propsId, quint32 count,const QString& expandContent) = 0;

	//直接使用道具结果返回信号.
	//参数1，指返回错误码.
	//参数2，道具使用者用户信息.
	//参数3，道具接收者用户信息.
	//参数4，道具id.
	//参数5，道具数量.
	//参数6 , 扩展内容.
	typedef _def_boost_signals2_signal_type<void (qint32,const IYYProps::PropsUserInfo&, const IYYProps::PropsUserInfo&, quint32, quint32,const QString&)>::type _SIG_DIRECT_USE_PROPS;
	virtual _SIG_DIRECT_USE_PROPS* getDirectUsePropsSignal() = 0;

	/****以下是道具二期接口*****/
	//道具二期后，客户端不再与业务服务器通信，由道具服务器与业务服务器通信.

	//获取某个业务的所有道具信息和定价.
	//调用此接口会有_SIG_GET_ALL_PROPS_EX和SigGetAllProps3信号回调.
	virtual void asyncGetAllPropsEx(void) = 0;

	//获取当前频道主播签约状态.
	//uid 指定要获取签约状态的主播的uid.
	virtual void asyncGetUserSigningStatus(quint32 uid) = 0;

	//购买并使用道具.
	//这个接口是购买和使用合并，将购买的逻辑放到后台实现.
	//如果你的包裹里已经有，就不会再购买，如果没有后台会自动选择合适的货币类型购买.
	//expandContent 扩展内容，各业务可根据业务增加内容，比如说赠送道具的留言.
	virtual void asyncConsumeAndUseProps(quint32 recvUid,quint32 propsId, quint32 count,const QString& expandContent) = 0;

	//获取某个业务的所有道具信息和定价通知信号.
	//参数1, 指返回错误码.
	typedef _def_boost_signals2_signal_type<void (qint32, const QList<IPropsInfoPtr>&)>::type _SIG_GET_ALL_PROPS_EX;
	virtual _SIG_GET_ALL_PROPS_EX* getAllPropsExSignal() = 0;

	//获取当前频道主播签约状态返回信号.
	//参数1，指返回错误码.
	//参数2，主播uid
	//参数3，频道号，如果为0，表示没有签约.
	typedef _def_boost_signals2_signal_type<void (qint32,quint32,quint32)>::type _SIG_GET_USER_SINGNING_STATUS_PROPS;
	virtual _SIG_GET_USER_SINGNING_STATUS_PROPS* getGetUserSigningStatusSignal() = 0;

	//购买并使用道具返回信号.
	//参数1，指返回错误码.
	//参数2，道具使用者用户信息.
	//参数3，道具接收者用户信息.
	//参数4，道具id.
	//参数5，使用道具数量.
	//参数6，购买信息（主要包括"购买道具数量""购买道具所使用的货币""购买道具总共用了多少钱"）.
	//参数7 , 扩展内容.
	typedef _def_boost_signals2_signal_type<void (qint32,const IYYProps::PropsUserInfo&, const IYYProps::PropsUserInfo&, quint32, quint32,
		const IYYProps::ConsumePropsInfo&,const QString&)>::type _SIG_CONSUME_AND_USE_PROPS;
	virtual _SIG_CONSUME_AND_USE_PROPS* getConsumeAndUsePropsSignal() = 0;

	/**
	  * @brief 购买道具，同时转移给其他用户（加入包裹）
	  * @param transfers 转移给的用户<uid, count>（uid，数量）集合
	  * @param propsId 道具id
	  * @param buyOnly true表示不使用自身包裹道具，只购买新道具
	  * @param expandContent 扩展字段
	  */
	virtual void asyncConsumeAndTransProps(const QVector<QPair<quint32, quint32>> &transfers, const quint32 propsId, const bool buyOnly, const QString &expandContent) = 0;

	/**
	  * @brief 购买道具，同时转移给其他用户（加入包裹）返回信号，单播，只有发送者收到
	  * @param result 返回代码
	  * @param transfers 转移给的用户<PropsUserInfo, count>（用户信息，数量）集合
	  * @param propsId 道具id
	  * @param count 数量
	  * @param buyOnly true表示不使用自身包裹道具，只购买新道具
	  * @param consumePropsInfo 购买的道具信息
	  * @param expandContent 扩展字段
	  */
	DWDECLARE_SIGNAL(SigConsumeAndTransProps, void(const qint32 result, const QVector<QPair<IYYProps::PropsUserInfo, quint32>> &transfers, const quint32 propsId, const quint32 count, const bool buyOnly, const IYYProps::ConsumePropsInfo &consumePropsInfo, const QString &expandContent));

	/**
	  * @brief 转移道具给其他用户广播
	  * @param sendUser 送道具用户信息
	  * @param transfers 转移给的用户<PropsUserInfo, count>（用户信息，数量）集合
	  * @param propsId 道具id
	  * @param count 数量
	  * @param expandContent 扩展字段
	  */
	DWDECLARE_SIGNAL(SigTransPropsBroadcast, void(const IYYProps::PropsUserInfo &sendUser, const QVector<QPair<IYYProps::PropsUserInfo, quint32>> &transfers, const quint32 propsId, const quint32 count, const QString &expandContent));

	/**
	  * @brief 转移个人账户货币给其他用户
	  * @param transfers 转移给的用户<uid, amount>（uid，数量）集合
	  * @param currencyType 货币类型，只支持非YB挂钩货币（白水晶/宝贝大乱斗）
	  * @param expandContent 扩展字段
	  */
	virtual void asyncTransUserAccountAmount(const QVector<QPair<quint32, quint32>> &transfers, const CurrencyType currencyType, const QString &expandContent) = 0;

	/**
	  * @brief 转移个人账户货币给其他用户返回信号
	  * @param result 返回代码，可能值：-1、-3、-5、-8
	  * @param transfers 转移给的用户<PropsUserInfo, amount>（用户信息，数量）集合
	  * @param currencyType 货币类型
	  * @param amount 数量
	  * @param expandContent 扩展字段
	  */
	DWDECLARE_SIGNAL(SigTransUserAccountAmount, void(const quint32 result, const QVector<QPair<IYYProps::PropsUserInfo, quint32>> &transfers, const CurrencyType currencyType, const quint32 amount, const QString &expandContent));

	/**
	  * @brief 转移个人账户货币给其他用户广播
	  * @param sendUser 送货币用户信息
	  * @param transfers 转移给的用户<PropsUserInfo, amount>（用户信息，数量）集合
	  * @param currencyType 货币类型
	  * @param amount 数量
	  * @param expandContent 扩展字段
	  */
	DWDECLARE_SIGNAL(SigTransUserAccountAmountBroadcast, void(const IYYProps::PropsUserInfo &sendUser, const QVector<QPair<IYYProps::PropsUserInfo, quint32>> &transfers, const CurrencyType currencyType, const quint32 amount, const QString &expandContent));


	/**
	  * @brief 获得指定用户被赠送道具数量，这里获得的是用户作为非主播时得到的道具，与asyncGetUserGiftPropsByUid不一样
	  * @param uid 用户uid
	  */
	virtual void asyncGetUserTakePropsByUid(const quint32 uid) = 0;

	/**
	  * @brief 获得指定用户被赠送的道具数量，asyncGetUserTakePropsByUid返回信号
	  * @param result 返回代码
	  * @param uid 用户uid
	  * @param props 道具列表
	  */
	DWDECLARE_SIGNAL(SigGetUserTakePropsByUid, void(const qint32 result, const quint32 uid, const QMap<quint32, quint32> &props));

	/**
	  * @brief 赠送道具，同时指定接收方和分成方
	  * @param dividedUid 分成的Uid，等价于asyncDirectUseProps的recvUid，一般是主播Uid
	  * @param takerUid 受赠的Uid，一般是嘉宾Uid
	  * @param propsId 礼物Id
	  * @param count 数量
	  * @param expandContent 扩展字段
	  */
	virtual void asyncUsePropsWithTaker(const quint32 dividedUid, const quint32 takerUid, const quint32 propsId, const quint32 count, const QString &expandContent) = 0;

	/**
	  * @brief 赠送道具，asyncUsePropsWithTaker返回信号
	  * @param result 返回代码
	  * @param senderInfo 赠送者信息
	  * @param devidedInfo 分成者信息，等价于asyncDirectUseProps返回的受赠者信息，一般是主播
	  * @param takerInfo 受赠者信息，一般是嘉宾
	  * @param propsId 礼物Id
	  * @param count 数量
	  * @param expandContent 扩展字段
	  */
	DWDECLARE_SIGNAL(SigUsePropsWithTaker, void(const qint32 result, const IYYProps::PropsUserInfo &senderInfo, const IYYProps::PropsUserInfo &devidedInfo, const IYYProps::PropsUserInfo &takerInfo, const quint32 propsId, const quint32 count, const QString &expandContent));
	
	/**
	  * @brief 购买并赠送道具，同时指定接收方和分成方广播
	  * @param senderInfo 赠送者信息
	  * @param devidedInfo 分成者信息，等价于asyncDirectUseProps返回的受赠者信息，一般是主播
	  * @param takerInfo 受赠者信息，一般是嘉宾
	  * @param propsId 礼物Id
	  * @param count 数量
	  * @param expandContent 扩展字段
	  */
	DWDECLARE_SIGNAL(SigUsePropsWithTakerBroadcast, void(const IYYProps::PropsUserInfo &senderInfo, const IYYProps::PropsUserInfo &devidedInfo, const IYYProps::PropsUserInfo &takerInfo, const quint32 propsId, const quint32 count, const QString &expandContent));

	/**
	  * @brief 购买并使用道具，同时指定接收方和分成方
	  * @param dividedUid 分成的Uid，等价于asyncDirectUseProps的recvUid，一般是主播Uid
	  * @param takerUid 受赠的Uid，一般是嘉宾Uid
	  * @param propsId 礼物Id
	  * @param count 数量
	  * @param expandContent 扩展字段
	  */
	virtual void asyncConsumeAndUsePropsWithTaker(const quint32 dividedUid, const quint32 takerUid, const quint32 propsId, const quint32 count, const QString& expandContent) = 0;

	/**
	  * @brief 购买并赠送道具，asyncConsumeAndUsePropsWithTaker返回信号
	  * @param result 返回代码
	  * @param senderInfo 赠送者信息
	  * @param devidedInfo 分成者信息，等价于asyncDirectUseProps返回的受赠者信息，一般是主播
	  * @param takerInfo 受赠者信息，一般是嘉宾
	  * @param propsId 礼物Id
	  * @param count 数量
	  * @param consumePropsInfo 购买的道具信息
	  * @param expandContent 扩展字段
	  */
	DWDECLARE_SIGNAL(SigConsumeAndUsePropsWithTaker, void(const qint32 result, const IYYProps::PropsUserInfo &senderInfo, const IYYProps::PropsUserInfo &devidedInfo, const IYYProps::PropsUserInfo &takerInfo, const quint32 propsId, const quint32 count, const IYYProps::ConsumePropsInfo &consumePropsInf, const QString &expandContent));

	/**
	  * @brief 获取某个业务的所有道具信息和定价，支持expand字段.调用此接口会有_SIG_GET_ALL_PROPS_EX和SigGetAllProps3信号回调.
	  * @param version 获取不同版本的道具信息，默认传0.
      * @param expandContent 扩展字段.
	  */
	virtual void asyncGetAllProps3(quint32 version, const QString& expandContent) = 0;

	//获取某个业务的所有道具信息和定价通知信号.
	//param result 指返回错误码.
	//param propsList 道具信息列表.
	//param expandContent 扩展字段.
	DWDECLARE_SIGNAL(SigGetAllProps3,void (const qint32 result, const QList<IPropsInfoPtr>& propsList, const QString& expandContent));

	//获取某个业务的所有道具信息和定价通知信号,原生json.
	//param result 指返回错误码.
	//param json 原生json信息.
	DWDECLARE_SIGNAL(SigGetAllProps4,void (const qint32 result, const QString& json));

	//模板业务与营收服务器（道具服务器）之间直接通信的统一请求接口
	virtual void asyncPropsUnifiedReq(const quint32 cmd, const QString& jsonData) = 0;

	//模板业务与营收服务之间直接通信统一请求接口返回信号，原生json
	//param result 指返回错误码.
	//param json 原生json信息.
	DWDECLARE_SIGNAL(SigPropsUnifiedRes,void (const qint32 result, const QString& json));
};

DWDEFINE_SIGNAL(IYYProps::SigConsumeAndTransProps);
DWDEFINE_SIGNAL(IYYProps::SigTransPropsBroadcast);
DWDEFINE_SIGNAL(IYYProps::SigTransUserAccountAmount);
DWDEFINE_SIGNAL(IYYProps::SigTransUserAccountAmountBroadcast);
DWDEFINE_SIGNAL(IYYProps::SigGetUserTakePropsByUid);
DWDEFINE_SIGNAL(IYYProps::SigUsePropsWithTaker);
DWDEFINE_SIGNAL(IYYProps::SigUsePropsWithTakerBroadcast);
DWDEFINE_SIGNAL(IYYProps::SigConsumeAndUsePropsWithTaker);
DWDEFINE_SIGNAL(IYYProps::SigGetAllProps3);
DWDEFINE_SIGNAL(IYYProps::SigGetAllProps4);
DWDEFINE_SIGNAL(IYYProps::SigPropsUnifiedRes);
