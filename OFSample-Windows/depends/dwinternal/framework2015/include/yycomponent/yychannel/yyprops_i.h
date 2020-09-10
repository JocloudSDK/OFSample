#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"
#include "yycomponent/yycomclsid.h"

// ������Ϣ�ӿ�.
DWDEFINE_INTERFACE(IPropsInfo) : public IUnk
{
	virtual quint32 getId(void) = 0;
	
	virtual QString getName(void) = 0;
	
	virtual QString	 getIconUrl(void) = 0;
	
	virtual quint32	 getAppId(void) = 0;
	
	//��ȡ���б�.
	virtual const QMap<quint32,quint32>& getPriceList(void) = 0;
	
	//��ȡ������Ϣ.
	virtual const QMap<QString,QString>& getDescriptionList(void) = 0;
};


// ����ϵͳ�ӿ�.
DWDEFINE_INTERFACE( IYYProps ) : public IUnk
{
	//�����붨��˵��.
	enum ResponseCode
	{
		kRespOK = 1, //�ɹ�.
		kRespIllegalAccount = -1,//�˻��Ƿ������֮��ģ�.
		kRespUnknownAppID = -2,//�����ڵ�ҵ��(�����ڵ�appId).
		kRespInSufficientBalance = -3,//���˻�����.
		kRespUnknownPropsID = -4,//�����ڵ�propsId��������¼�.
		kRespCurrencyAccountRestrictions = -5,//����ʹ�á���ֵ�Ļ����˻�.
		kRespNotExistOrSufficientSum = -6,//û��������߻���������.
		KRespRechargeActivityCurrencyLimit = -7,//������Զ���ֵ�ﵽÿ������.
		KRespTimeAuthenticationFailed = -8,//ʱ����֤ʧ�ܣ�ֻ�г�ֵ�ӿڷ�����������.
		KRespNeedConfirm = -10,//��rmb������Ҫȷ��
		kRespParamError = -400,//����������ָ����ȱʧ.
		kRespAuthenticationFailed = -401,//��ݼ�Ȩʧ��.
		kRespNoAuthority = -403,//û��Ȩ�ޣ������ã�������.
		kRespUnknowCmd = -404,//δ֪cmd.
		kRespSystemError = -500//����˴���.
	};

	// ��������ö��.
	enum CurrencyType
	{
		CT_Unknow = 0,
		Virtual=1, // ������ң���ˮ��.
		Real, // ���ֻ��ң���ˮ��.
		Activity, // ����ң���ˮ��.
		Yb,   // Y�ң�ϵͳ���Զ����ݻ���ת��Ϊ��ˮ������.
		Time  // ʱ��Ҳ��Ϊһ�ֻ��ң���λ�룬�Զ����ݻ���ת��Ϊ��ˮ������.
	};

	// ��������ö��.
	enum PropsType
	{
		PT_Unknow = 0,
		Free,// ��ѵ���.
		Premium // �շѵ���.
	};

	//�����û��˻���Ϣ.
	struct Account
	{
		quint32 uid;
		CurrencyType currencyType;
		quint32 amount;
		quint32 freezed;
		Account() : uid(0), currencyType(CT_Unknow), amount(0), freezed(0){}
	};

	// ������Ϣ����.
	struct Props
	{
		quint32				id;
		QString				name;
		QString				iconUrl;// ͼ���ַ.
		quint32				appid; // ҵ�񷽱�ʶ.
		QMap<CurrencyType,quint32>			priceList;//��ͬ�������͵Ķ����б�.
		Props() : id(0), appid(0){}
	};

	// �û����е���.
	struct UserProps
	{
		quint32	uid;
		quint32	propsId;
		quint32	propsCount;
		UserProps() : uid(0), propsCount(0), propsId(0){}
	};

	//�û�������Ϣ���л��ܶ���.
	struct User
	{
		quint32		uid;
		QList<Account> accounts;
		QList<UserProps> userPropsList;	
		User() : uid(0){}
	};

	//�����û���Ϣ.
	struct PropsUserInfo
	{
		quint32 uid;
		QString imId;
		QString nickName;
		PropsUserInfo() : uid(0){}
	};

	//���������Ϣ.
	struct ConsumePropsInfo
	{
		quint32 propsId;
		quint32 count; //�����������.
		CurrencyType currencyType;//���������ʹ�õĻ���.
		double costMoney;//��������ܹ����˶���Ǯ��������ʲô���ͣ���ʲôΪ��λ.
	};

public:

	//������ҵ��ID.
	//�ƾ���1��������2����һ�а���3.
	//ע�⣺�˽ӿڳ���(�ƾ��ǣ������ǣ���һ�а���)������ģ�治����ʹ��.
	virtual void setAppId(quint32 nAppId) = 0;

	//ͨ��ָ��uid����ȡ�ѻ�����͵ĵ�������.
	virtual void asyncGetUserGiftPropsByUid(quint32 uid) = 0;

	//��ȡ���Լ����е�����Ϣ.
	virtual void asyncGetMyAllProps(void) = 0;

	//��ȡĳ��ҵ������е�����Ϣ�Ͷ���.
	//ҵ����setAppId�ӿڱ�ָ��.
	virtual void asyncGetAllProps(void) = 0;

	// �������
	//expandContent ��չ���ݣ���͸���ã����õĻ������.
	virtual void asyncConsumeProps(quint32 propsId, quint32 count,CurrencyType currencyType,const QString& expandContent) = 0;

	//��ֵ��������ⲿ��ֵ��.
	//currencyType: ��������:1:���� 2:���� 3:� 4:Y�� 5:ʱ�����.
	//amount: ����.
	//����ƾ���ч������asyncReChargeProps(IYYProps::Activity,20).
	virtual void asyncReChargeProps(CurrencyType currencyType, quint32 amount) = 0;

	//ʹ�õ���(ҵ�������serviceͨ��).
	//��ӿ���ͨ��serviceͨ������ҵ�������ʵ��.
	//expandContent ��չ���ݣ���ҵ��ɸ���ҵ���������ݣ�����˵���͵��ߵ�����.
	virtual void asyncUseProps(quint32 recvUid,quint32 propsId, quint32 count,const QString& expandContent) = 0;

	//ָ��uid�����͵ĵ�����������֪ͨ�ź�.
	//����1��ָ���ش�����.
	//����2��ָ��Ҫ���ҵ��û�id.
	//����3���յ��ĵ����б�.
	typedef _def_boost_signals2_signal_type<void (qint32, quint32,const QMap<quint32,quint32>&)>::type _SIG_GET_USER_GIFT_PROPS_BY_UID;
	virtual _SIG_GET_USER_GIFT_PROPS_BY_UID* getUserGiftPropsByUidSignal() = 0;

	//��ȡ���е�����Ϣ�������֪ͨ�ź�.
	//����1, ָ���ش�����.
	//����2, �û����еĵ�����Ϣ.
	typedef _def_boost_signals2_signal_type<void (qint32, const IYYProps::User&)>::type _SIG_GET_MY_ALL_PROPS;
	virtual _SIG_GET_MY_ALL_PROPS* getMyAllPropsSignal() = 0;

	//��ȡĳ��ҵ������е�����Ϣ�Ͷ���֪ͨ�ź�.
	//����1, ָ���ش�����.
	typedef _def_boost_signals2_signal_type<void (qint32, const QList<IYYProps::Props>&)>::type _SIG_GET_ALL_PROPS;
	virtual _SIG_GET_ALL_PROPS* getAllPropsSignal() = 0;

	//�������ȷ��֪ͨ�ź�.
	//���û��Ĺ��������˹������ѡ��ʱ�����ù�����߽ӿڣ��ᴥ�����źţ����û�ȷ�Ϻ�ᴥ��getConsumePropsSignal().
	//���û���������ֱ�Ӵ���getConsumePropsSignal().
	//����1, ָ���ش�����,������=-10ʱ����Ҫ�û�ȷ�ϵ���confirmUrl.
	//����2, ȷ��url.
	typedef _def_boost_signals2_signal_type<void (qint32,const QString&)>::type _SIG_CONSUME_PROPS_CONFIRM;
	virtual _SIG_CONSUME_PROPS_CONFIRM* getConsumePropsConfirmSignal() = 0;

	//������߳ɹ�֪ͨ�ź�.
	//���ع�������1��ʾ����ɹ�
	//����1, ָ���ش�����,����ֵ�ҵ������룺-1��-3��-4��-5.
	//����2, ����id.
	//����3, ��������.
	//����4, ��չ��Ĭ��Ϊ��.
	typedef _def_boost_signals2_signal_type<void (qint32,quint32,qint32,const QString&)>::type _SIG_CONSUME_PROPS;
	virtual _SIG_CONSUME_PROPS* getConsumePropsSignal() = 0;

	//��ֵ��������ⲿ��ֵ��֪ͨ�ź�.
	//����1, ָ���ش�����.
	//����2, ��������:1:���� 2:���� 3:� 4:Y�� 5:ʱ�����.
	//����3, ����.
	typedef _def_boost_signals2_signal_type<void (qint32,IYYProps::CurrencyType,quint32)>::type _SIG_RECHARGE_PROPS;
	virtual _SIG_RECHARGE_PROPS* getReChargePropsSignal() = 0;

	//ʹ�õ���֪ͨ�ź�.
	//����1, ָ���ش����룬����1ָ�ɹ�������0ָʧ�ܣ�����-6ָû��������߻���������.
	typedef _def_boost_signals2_signal_type<void (qint32)>::type _SIG_USE_PROPS;
	virtual _SIG_USE_PROPS* getUsePropsSignal() = 0;

	//���յ���֪ͨ�źţ�ֻ�н����׷��Ż��յ����֪ͨ.
	//����1, �ͳ������û���id.
	//����2, ����id.
	//����3, ��������.
	//����4, ��չ����.
	typedef _def_boost_signals2_signal_type<void (quint32,qint32,qint32,const QString&)>::type _SIG_USE_PROPS_NOTICE;
	virtual _SIG_USE_PROPS_NOTICE* getUsePropsNoticeSignal() = 0;

	//�ͳ����߹㲥֪ͨ�ź�.
	//����1������ʹ�����û���Ϣ.
	//����2�����߽������û���Ϣ.
	//����3������id.
	//����4����������.
	//����5 , ��չ����.
	typedef _def_boost_signals2_signal_type<void (const IYYProps::PropsUserInfo&, const IYYProps::PropsUserInfo&, quint32, quint32,const QString&)>::type _SIG_USE_PROPS_BROADCAST;
	virtual _SIG_USE_PROPS_BROADCAST* getUsePropsBroadcastSignal() = 0;

	//��ʼ��.
	//propsAppId ����ϵͳ�����id.
	//serviceId	ҵ�������serviceͨ��id,�������Ҫҵ������������Դ�0.
	virtual void create(quint32 propsAppId,quint32 serviceId) = 0;

	//ֱ��ʹ�õ���(imͨ��).
	//��ӿ���ֱ��ͨ��imͨ�����ʵ���ϵͳ������ʵ��.
	//expandContent ��չ���ݣ���ҵ��ɸ���ҵ���������ݣ�����˵���͵��ߵ�����.
	virtual void asyncDirectUseProps(quint32 recvUid,quint32 propsId, quint32 count,const QString& expandContent) = 0;

	//ֱ��ʹ�õ��߽�������ź�.
	//����1��ָ���ش�����.
	//����2������ʹ�����û���Ϣ.
	//����3�����߽������û���Ϣ.
	//����4������id.
	//����5����������.
	//����6 , ��չ����.
	typedef _def_boost_signals2_signal_type<void (qint32,const IYYProps::PropsUserInfo&, const IYYProps::PropsUserInfo&, quint32, quint32,const QString&)>::type _SIG_DIRECT_USE_PROPS;
	virtual _SIG_DIRECT_USE_PROPS* getDirectUsePropsSignal() = 0;

	/****�����ǵ��߶��ڽӿ�*****/
	//���߶��ں󣬿ͻ��˲�����ҵ�������ͨ�ţ��ɵ��߷�������ҵ�������ͨ��.

	//��ȡĳ��ҵ������е�����Ϣ�Ͷ���.
	//���ô˽ӿڻ���_SIG_GET_ALL_PROPS_EX��SigGetAllProps3�źŻص�.
	virtual void asyncGetAllPropsEx(void) = 0;

	//��ȡ��ǰƵ������ǩԼ״̬.
	//uid ָ��Ҫ��ȡǩԼ״̬��������uid.
	virtual void asyncGetUserSigningStatus(quint32 uid) = 0;

	//����ʹ�õ���.
	//����ӿ��ǹ����ʹ�úϲ�����������߼��ŵ���̨ʵ��.
	//�����İ������Ѿ��У��Ͳ����ٹ������û�к�̨���Զ�ѡ����ʵĻ������͹���.
	//expandContent ��չ���ݣ���ҵ��ɸ���ҵ���������ݣ�����˵���͵��ߵ�����.
	virtual void asyncConsumeAndUseProps(quint32 recvUid,quint32 propsId, quint32 count,const QString& expandContent) = 0;

	//��ȡĳ��ҵ������е�����Ϣ�Ͷ���֪ͨ�ź�.
	//����1, ָ���ش�����.
	typedef _def_boost_signals2_signal_type<void (qint32, const QList<IPropsInfoPtr>&)>::type _SIG_GET_ALL_PROPS_EX;
	virtual _SIG_GET_ALL_PROPS_EX* getAllPropsExSignal() = 0;

	//��ȡ��ǰƵ������ǩԼ״̬�����ź�.
	//����1��ָ���ش�����.
	//����2������uid
	//����3��Ƶ���ţ����Ϊ0����ʾû��ǩԼ.
	typedef _def_boost_signals2_signal_type<void (qint32,quint32,quint32)>::type _SIG_GET_USER_SINGNING_STATUS_PROPS;
	virtual _SIG_GET_USER_SINGNING_STATUS_PROPS* getGetUserSigningStatusSignal() = 0;

	//����ʹ�õ��߷����ź�.
	//����1��ָ���ش�����.
	//����2������ʹ�����û���Ϣ.
	//����3�����߽������û���Ϣ.
	//����4������id.
	//����5��ʹ�õ�������.
	//����6��������Ϣ����Ҫ����"�����������""���������ʹ�õĻ���""��������ܹ����˶���Ǯ"��.
	//����7 , ��չ����.
	typedef _def_boost_signals2_signal_type<void (qint32,const IYYProps::PropsUserInfo&, const IYYProps::PropsUserInfo&, quint32, quint32,
		const IYYProps::ConsumePropsInfo&,const QString&)>::type _SIG_CONSUME_AND_USE_PROPS;
	virtual _SIG_CONSUME_AND_USE_PROPS* getConsumeAndUsePropsSignal() = 0;

	/**
	  * @brief ������ߣ�ͬʱת�Ƹ������û������������
	  * @param transfers ת�Ƹ����û�<uid, count>��uid������������
	  * @param propsId ����id
	  * @param buyOnly true��ʾ��ʹ������������ߣ�ֻ�����µ���
	  * @param expandContent ��չ�ֶ�
	  */
	virtual void asyncConsumeAndTransProps(const QVector<QPair<quint32, quint32>> &transfers, const quint32 propsId, const bool buyOnly, const QString &expandContent) = 0;

	/**
	  * @brief ������ߣ�ͬʱת�Ƹ������û�����������������źţ�������ֻ�з������յ�
	  * @param result ���ش���
	  * @param transfers ת�Ƹ����û�<PropsUserInfo, count>���û���Ϣ������������
	  * @param propsId ����id
	  * @param count ����
	  * @param buyOnly true��ʾ��ʹ������������ߣ�ֻ�����µ���
	  * @param consumePropsInfo ����ĵ�����Ϣ
	  * @param expandContent ��չ�ֶ�
	  */
	DWDECLARE_SIGNAL(SigConsumeAndTransProps, void(const qint32 result, const QVector<QPair<IYYProps::PropsUserInfo, quint32>> &transfers, const quint32 propsId, const quint32 count, const bool buyOnly, const IYYProps::ConsumePropsInfo &consumePropsInfo, const QString &expandContent));

	/**
	  * @brief ת�Ƶ��߸������û��㲥
	  * @param sendUser �͵����û���Ϣ
	  * @param transfers ת�Ƹ����û�<PropsUserInfo, count>���û���Ϣ������������
	  * @param propsId ����id
	  * @param count ����
	  * @param expandContent ��չ�ֶ�
	  */
	DWDECLARE_SIGNAL(SigTransPropsBroadcast, void(const IYYProps::PropsUserInfo &sendUser, const QVector<QPair<IYYProps::PropsUserInfo, quint32>> &transfers, const quint32 propsId, const quint32 count, const QString &expandContent));

	/**
	  * @brief ת�Ƹ����˻����Ҹ������û�
	  * @param transfers ת�Ƹ����û�<uid, amount>��uid������������
	  * @param currencyType �������ͣ�ֻ֧�ַ�YB�ҹ����ң���ˮ��/�������Ҷ���
	  * @param expandContent ��չ�ֶ�
	  */
	virtual void asyncTransUserAccountAmount(const QVector<QPair<quint32, quint32>> &transfers, const CurrencyType currencyType, const QString &expandContent) = 0;

	/**
	  * @brief ת�Ƹ����˻����Ҹ������û������ź�
	  * @param result ���ش��룬����ֵ��-1��-3��-5��-8
	  * @param transfers ת�Ƹ����û�<PropsUserInfo, amount>���û���Ϣ������������
	  * @param currencyType ��������
	  * @param amount ����
	  * @param expandContent ��չ�ֶ�
	  */
	DWDECLARE_SIGNAL(SigTransUserAccountAmount, void(const quint32 result, const QVector<QPair<IYYProps::PropsUserInfo, quint32>> &transfers, const CurrencyType currencyType, const quint32 amount, const QString &expandContent));

	/**
	  * @brief ת�Ƹ����˻����Ҹ������û��㲥
	  * @param sendUser �ͻ����û���Ϣ
	  * @param transfers ת�Ƹ����û�<PropsUserInfo, amount>���û���Ϣ������������
	  * @param currencyType ��������
	  * @param amount ����
	  * @param expandContent ��չ�ֶ�
	  */
	DWDECLARE_SIGNAL(SigTransUserAccountAmountBroadcast, void(const IYYProps::PropsUserInfo &sendUser, const QVector<QPair<IYYProps::PropsUserInfo, quint32>> &transfers, const CurrencyType currencyType, const quint32 amount, const QString &expandContent));


	/**
	  * @brief ���ָ���û������͵��������������õ����û���Ϊ������ʱ�õ��ĵ��ߣ���asyncGetUserGiftPropsByUid��һ��
	  * @param uid �û�uid
	  */
	virtual void asyncGetUserTakePropsByUid(const quint32 uid) = 0;

	/**
	  * @brief ���ָ���û������͵ĵ���������asyncGetUserTakePropsByUid�����ź�
	  * @param result ���ش���
	  * @param uid �û�uid
	  * @param props �����б�
	  */
	DWDECLARE_SIGNAL(SigGetUserTakePropsByUid, void(const qint32 result, const quint32 uid, const QMap<quint32, quint32> &props));

	/**
	  * @brief ���͵��ߣ�ͬʱָ�����շ��ͷֳɷ�
	  * @param dividedUid �ֳɵ�Uid���ȼ���asyncDirectUseProps��recvUid��һ��������Uid
	  * @param takerUid ������Uid��һ���Ǽα�Uid
	  * @param propsId ����Id
	  * @param count ����
	  * @param expandContent ��չ�ֶ�
	  */
	virtual void asyncUsePropsWithTaker(const quint32 dividedUid, const quint32 takerUid, const quint32 propsId, const quint32 count, const QString &expandContent) = 0;

	/**
	  * @brief ���͵��ߣ�asyncUsePropsWithTaker�����ź�
	  * @param result ���ش���
	  * @param senderInfo ��������Ϣ
	  * @param devidedInfo �ֳ�����Ϣ���ȼ���asyncDirectUseProps���ص���������Ϣ��һ��������
	  * @param takerInfo ��������Ϣ��һ���Ǽα�
	  * @param propsId ����Id
	  * @param count ����
	  * @param expandContent ��չ�ֶ�
	  */
	DWDECLARE_SIGNAL(SigUsePropsWithTaker, void(const qint32 result, const IYYProps::PropsUserInfo &senderInfo, const IYYProps::PropsUserInfo &devidedInfo, const IYYProps::PropsUserInfo &takerInfo, const quint32 propsId, const quint32 count, const QString &expandContent));
	
	/**
	  * @brief �������͵��ߣ�ͬʱָ�����շ��ͷֳɷ��㲥
	  * @param senderInfo ��������Ϣ
	  * @param devidedInfo �ֳ�����Ϣ���ȼ���asyncDirectUseProps���ص���������Ϣ��һ��������
	  * @param takerInfo ��������Ϣ��һ���Ǽα�
	  * @param propsId ����Id
	  * @param count ����
	  * @param expandContent ��չ�ֶ�
	  */
	DWDECLARE_SIGNAL(SigUsePropsWithTakerBroadcast, void(const IYYProps::PropsUserInfo &senderInfo, const IYYProps::PropsUserInfo &devidedInfo, const IYYProps::PropsUserInfo &takerInfo, const quint32 propsId, const quint32 count, const QString &expandContent));

	/**
	  * @brief ����ʹ�õ��ߣ�ͬʱָ�����շ��ͷֳɷ�
	  * @param dividedUid �ֳɵ�Uid���ȼ���asyncDirectUseProps��recvUid��һ��������Uid
	  * @param takerUid ������Uid��һ���Ǽα�Uid
	  * @param propsId ����Id
	  * @param count ����
	  * @param expandContent ��չ�ֶ�
	  */
	virtual void asyncConsumeAndUsePropsWithTaker(const quint32 dividedUid, const quint32 takerUid, const quint32 propsId, const quint32 count, const QString& expandContent) = 0;

	/**
	  * @brief �������͵��ߣ�asyncConsumeAndUsePropsWithTaker�����ź�
	  * @param result ���ش���
	  * @param senderInfo ��������Ϣ
	  * @param devidedInfo �ֳ�����Ϣ���ȼ���asyncDirectUseProps���ص���������Ϣ��һ��������
	  * @param takerInfo ��������Ϣ��һ���Ǽα�
	  * @param propsId ����Id
	  * @param count ����
	  * @param consumePropsInfo ����ĵ�����Ϣ
	  * @param expandContent ��չ�ֶ�
	  */
	DWDECLARE_SIGNAL(SigConsumeAndUsePropsWithTaker, void(const qint32 result, const IYYProps::PropsUserInfo &senderInfo, const IYYProps::PropsUserInfo &devidedInfo, const IYYProps::PropsUserInfo &takerInfo, const quint32 propsId, const quint32 count, const IYYProps::ConsumePropsInfo &consumePropsInf, const QString &expandContent));

	/**
	  * @brief ��ȡĳ��ҵ������е�����Ϣ�Ͷ��ۣ�֧��expand�ֶ�.���ô˽ӿڻ���_SIG_GET_ALL_PROPS_EX��SigGetAllProps3�źŻص�.
	  * @param version ��ȡ��ͬ�汾�ĵ�����Ϣ��Ĭ�ϴ�0.
      * @param expandContent ��չ�ֶ�.
	  */
	virtual void asyncGetAllProps3(quint32 version, const QString& expandContent) = 0;

	//��ȡĳ��ҵ������е�����Ϣ�Ͷ���֪ͨ�ź�.
	//param result ָ���ش�����.
	//param propsList ������Ϣ�б�.
	//param expandContent ��չ�ֶ�.
	DWDECLARE_SIGNAL(SigGetAllProps3,void (const qint32 result, const QList<IPropsInfoPtr>& propsList, const QString& expandContent));

	//��ȡĳ��ҵ������е�����Ϣ�Ͷ���֪ͨ�ź�,ԭ��json.
	//param result ָ���ش�����.
	//param json ԭ��json��Ϣ.
	DWDECLARE_SIGNAL(SigGetAllProps4,void (const qint32 result, const QString& json));

	//ģ��ҵ����Ӫ�շ����������߷�������֮��ֱ��ͨ�ŵ�ͳһ����ӿ�
	virtual void asyncPropsUnifiedReq(const quint32 cmd, const QString& jsonData) = 0;

	//ģ��ҵ����Ӫ�շ���֮��ֱ��ͨ��ͳһ����ӿڷ����źţ�ԭ��json
	//param result ָ���ش�����.
	//param json ԭ��json��Ϣ.
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
