#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/propBag_i.h"
#include "biz/biz_types.h"

enum IM_STATUS;
enum LOGINTYPE;

enum LoginState
{
	Logout,
	Logining,
	Logined,
	Offline,
};

// same as PASSPORT_ERROR_CODE
enum LOGIN_ERROR_CODE
{
	LOGINERR_UNKNOWN_ERROR,
	LOGINERR_INITIATIVE_LOGOUT,
	LOGINERR_NETWORK_ERROR,
	LOGINERR_SERVER_ERROR,
	LOGINERR_PROTOCOL_OLD,
	LOGINERR_PASSPORT_ERROR,
	LOGINERR_GLOBAL_BAN,
	LOGINERR_FREENZEN_BAN,
	LOGINERR_KICKOFF,
	LOGINERR_MUL_KICK,
	LOGINERR_ATTEMPTING_SO_MUCH,
	LOGINERR_NEED_VALIDATE,
	LOGINERR_LOGINNING,
	LOGINERR_WAIT_RECONNECT,
	LOGINERR_TOKEN_NEEDED,
	LOGINERR_TOKER_ERROR,
	LOGINERR_WHITE_LIST_VERIFY_FAIL,
	LOGINERR_CREDIT_ERROR,
	LOGINERR_REAL_CREDIT_ERROR,
    LOGINERR_FORCEOUT,
	LOGINERR_UDB_CREDIT_INVALID,//��¼2.0��ƾ֤��Ч.
	LOGINERR_UDB_SWITCH_CLOSED,//��¼2.0�ӿڿ����Ѿ��ر�
};

struct LoginDynamicToken
{
    UINT8 token_type;							// ��������DYNAMIC_TOKEN_TYPE
    QString verification_selection_title;
    QString verification_title;
    QString token_data;
};

/**
  * @brief ��¼�ӿ�, ͨ��Root->getServiceInterface<IYYLogin>(KSERVICEMainframe)��ȡ.
  */
DWDEFINE_INTERFACE(IYYLogin) : public IUnk
{
/**
   * @brief login.
   * @param userName user name.
   * @param password password.
   */
	virtual void login( const std::wstring& userName, const std::wstring& password ) = 0;
	
/**
   * @brief �����ʵ�ǵ���ע��������ӿ����ֲ���.
   */
	virtual void logout() = 0;

	virtual void loginWithSha1(const QString& userName, const QString& passwordHC, IM_STATUS status) = 0;
	// �Ѿ����ú�passport�ĳ�ʼֵ
	virtual void loginWithoutInfo() = 0;

	virtual void loginBy3rd(const QString& passport, const QString& token, const QString& accountInfo, IM_STATUS status, LOGINTYPE type) = 0;
	// �Ѿ����ú�passport�ĳ�ʼֵ
	virtual void loginBy3rdWithoutInfo() = 0;

	virtual void passportLogout() = 0;

	virtual bool isAccountLogined(const QString& account) = 0;
	virtual bool isAccountStartLogin(const QString& account) = 0;

	virtual void setLoginState(LoginState s) = 0;
	virtual LoginState loginState() = 0;

	virtual void sendVerifyCode(const std::wstring &picId, const std::string &code) = 0; // �ӿ�����
	virtual void sendDynamicKeyLogin(UINT32 key) = 0; // �ӿ�����

	virtual BIZ_ISPType getIspTypeAll() = 0;

    virtual void makeAccountStartLogin(const QString& account) = 0;

	DWDECLARE_SIGNAL(SigLoginStateChanged, void(const LoginState oldState));
	DWDECLARE_SIGNAL(SigLogined, void (bool));
	DWDECLARE_SIGNAL(SigLogout, void (LOGIN_ERROR_CODE, IPropBagPtrCR));
	DWDECLARE_SIGNAL(SigPasswordTokenStat, void (LOGIN_ERROR_CODE, quint32)); // �ź�����
	DWDECLARE_SIGNAL(SigWaitReconnect, void (IPropBagPtrCR));
	DWDECLARE_SIGNAL(SigLoginFailStat, void (IPropBagPtrCR));
	DWDECLARE_SIGNAL(SigVerifyCodeRequired, void(const std::wstring &reason, const std::wstring &picId, const UINT32 resCode, const std::string &picData));
    DWDECLARE_SIGNAL(SigDynamicTokenWithContext, void(const std::vector<std::string>&, QList<LoginDynamicToken>&)); // �ź�����
    DWDECLARE_SIGNAL(SigDynamicTokenErr, void(const std::vector<std::string>&, const UINT8)); // �ź�����
    DWDECLARE_SIGNAL(SigRefreshSmsCode, void(IPropBagPtrCR));

	virtual void qrcodeLogin(const std::string& data,IM_STATUS status) = 0;
	virtual void creditLogin(UINT32 uid, const std::wstring& account, const std::string& credit, const std::string& token,IM_STATUS status) = 0;

	DWDECLARE_SIGNAL(SigDynamicTokenErr2, void(const std::vector<std::string>&, const UINT8, const std::string&));
	DWDECLARE_SIGNAL(SigDynamicTokenWithContext2, void(const std::vector<std::string>&, QList<LoginDynamicToken>&, const std::string&));
};

DWDEFINE_SIGNAL(IYYLogin::SigLoginStateChanged);
DWDEFINE_SIGNAL(IYYLogin::SigLogined);
DWDEFINE_SIGNAL(IYYLogin::SigLogout);
DWDEFINE_SIGNAL(IYYLogin::SigPasswordTokenStat);// �ź�����
DWDEFINE_SIGNAL(IYYLogin::SigWaitReconnect);
DWDEFINE_SIGNAL(IYYLogin::SigLoginFailStat);
DWDEFINE_SIGNAL(IYYLogin::SigVerifyCodeRequired);
DWDEFINE_SIGNAL(IYYLogin::SigDynamicTokenWithContext); // �ź�����
DWDEFINE_SIGNAL(IYYLogin::SigDynamicTokenErr); // �ź�����
DWDEFINE_SIGNAL(IYYLogin::SigRefreshSmsCode);
DWDEFINE_SIGNAL(IYYLogin::SigDynamicTokenErr2);
DWDEFINE_SIGNAL(IYYLogin::SigDynamicTokenWithContext2);
