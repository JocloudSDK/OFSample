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
	* @brief ������ѯ�û�������״̬.
	* @param uidList �û�uid�б�.
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
		IM_HD_640_640, // webYYͷ��
		IM_HD_UNKNOWN = 0xff
	};

	/**
	* @brief ������ѯ�û�simple user info��ֱ�ӵ���protocol�ӿ�.
	* @param vecUid �û�uid�б�.
	* @param mapContext ��uid������context.
	* @return .
	*/	
	virtual void querySimpleUinfoEx(const std::vector<UINT32>& vecUid, const std::map<UINT32, std::string>& mapContext) = 0;

	/**
	* @brief ������ѯ�û�simple user info, �Ż�querySimpleUinfoEx.
	* @param vecUid �û�uid�б�.
	* @param mapContext ��uid������context.
	* @return .
	*/
	virtual void getUserInfo( const UINT32 userId, const std::string& context ) = 0;

	/**
	* @brief ����ȡ��ͷ��ͼƬurl����Ϣ.
	* @param vecUid �û�uid�б�.
	* @param mapContext ��uid������context.
	* @return .
	*/
	virtual void getPhotoListPub(
		const std::vector<UINT32>& vecUid, 
		UINT32 uFormat, 
		const std::map<UINT32, std::string>& mapContext
		) = 0;

	/**
	* @brief �õ��Զ���ͷ����������ݣ�����������У���ֱ�ӷ���ͼƬ����������ڣ������سɹ�����getcustomHeadImageDownloadCompileSignal�¼�֪ͨ.
	* @param uBuddyID �û�uid.
	* @param url �Զ���ͷ���web·��.
	* @param bOnline �Ƿ�Ҫ����״̬�����Ϊfalse, ����Զ�����ͼ.
	* @return �Զ���ͷ��.
	*/
	virtual QImage getcustomHeadImageData(UINT32 uBuddyID, const QString url, bool bOnline) = 0;

	/**
	* @brief �õ�ϵͳͷ�����������.
	* @param index ϵͳͷ��index.
	* @param bOnline �Ƿ�Ҫ����״̬�����Ϊfalse, ����Զ�����ͼ.
	* @return ϵͳͷ��.
	*/
	virtual QImage getsystemHeadImageData(UINT32 index, bool bOnline) = 0;

	/**
	* @brief �õ���̬ͷ����������ݣ�����������У���ֱ�ӷ���ͼƬ����������ڣ������سɹ�����getDynamicHeadImageDownloadCompileSignal�¼�֪ͨ.
	* @param uBuddyID �û�uid.
	* @param url �Զ���ͷ���web·��.
	* @return ��̬ͷ�����������.
	*/
	virtual QByteArray getDynamicHeadImageData(UINT32 uBuddyID, const QString url) = 0;

	/**
	* @brief ������ѯ�û�simple user info���������ؽ��.
	* @param vecData �û���Ϣ���ؽ���б�.
	* @param mapContext �û���Ϣ��ص�������.
	* @return �¼�����.
	*/
	typedef _def_boost_signals2_signal_type<void (const std::vector<UI_SImUinfoSimpleEx>& vecData, const std::map<UINT32, std::string>& mapContext)>::type	_SIG_SIMPLE_USERINFOEX;
	virtual _SIG_SIMPLE_USERINFOEX* getSimpleUserInfoExSignal() = 0;

	/**
	* @brief �����Զ���ͷ�񷵻ؽ��.
	* @param userId �û�uid.
	* @return �¼�����.
	*/
	typedef _def_boost_signals2_signal_type<void (const UINT32 userId)>::type	_SIG_DOWNLOAD_COMPILE;
	virtual _SIG_DOWNLOAD_COMPILE* getcustomHeadImageDownloadCompileSignal() = 0;

	/**
	* @brief ���ض�̬ͷ�񷵻ؽ��.
	* @return �¼�����.
	*/
	virtual _SIG_DOWNLOAD_COMPILE* getDynamicHeadImageDownloadCompileSignal() = 0;

	/**
	* @brief  ������ѯ�û�simple user info���������ؽ��.
	* @param vecData �û���Ϣ���ؽ���б�.
	* @param mapContext �û���Ϣ��ص�������.
	* @return �¼�����.
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
    * @brief ��ѯ�û���Ϣ������Ỻ�棬һ��ʱ���������ѯ�����ߵ�����������һ������������ѯ.
    * @param uAppId ���id.
    * @param uUserId �û�im id.
    * @param uMask USER_INFO_MASK�ֶεļ��ϣ���ʾ��Ҫȡ�ķ������Ϣ.
    * @return 
    */
    virtual void getUserInfo2(UINT32 uAppId, UINT32 uUserId, UINT32 uMask) = 0;

    /**
    * @brief ������ѯ�û���Ϣ������Ỻ�棬һ��ʱ���������ѯ�����ߵ�����������һ������������ѯ.
    * @param uAppId ���id.
    * @param vedIds �û�im id����.
    * @param uMask USER_INFO_MASK�ֶεļ��ϣ���ʾ��Ҫȡ�ķ������Ϣ.
    * @return 
    */
    virtual void getUserInfos(UINT32 uAppId, const std::vector<UINT32>& vecIds, UINT32 uMask) = 0;

    /**
    * @brief �ӿ�getUserInfo2/getUserInfos���ص��ź�.
    * @param Ӧ��ID.
    * @param �û���Ϣ���ݼ�, ���û�id(uid)��ΪID.
    * @param USER_INFO_MASK�ֶεļ��ϣ���ʾ���ݼ��а����ķ������Ϣ.
    * @return 
    */
    typedef _def_boost_signals2_signal_type<void (UINT32, IPropBagSetWrapperPtrCR, UINT32)>::type _SIG_USER_INFO_RES;
    virtual _SIG_USER_INFO_RES* getUserInfoResSignal() = 0;

    /**
    * @brief ��ѯ�û�ͷ������Ỻ�棬һ��ʱ���������ѯ�����ߵ�����������һ������������ѯ.
    * @param uAppId ���id.
    * @param uUserId �û�im id.
    * @param eType ͷ������.
    * @return 
    */
    virtual void getHeadSculpture(UINT32 uAppId, UINT32 uUserId, UI_EHDLogoType eType) = 0;
    
    /**
    * @brief ������ѯ�û�ͷ������Ỻ�棬һ��ʱ���������ѯ�����ߵ�����������һ������������ѯ.
    * @param uAppId ���id.
    * @param vedIds �û�im id����.
    * @param eType ͷ������.
    * @return 
    */
    virtual void getHeadSculptures(UINT32 uAppId, const std::vector<UINT32>& vecIds, UI_EHDLogoType eType) = 0;

    /**
    * @brief �ӿ�getHeadSculpture/getHeadSculptures���ص��ź�.
    * @param Ӧ��ID.
    * @param �û���Ϣ���ݼ�, ���û�id(uid)��ΪID.
    * @return 
    */
    typedef _def_boost_signals2_signal_type<void (UINT32, IPropBagSetWrapperPtrCR)>::type _SIG_HEAD_SCULPTURE_RES;
    virtual _SIG_HEAD_SCULPTURE_RES* getHeadSculptureResSignal() = 0;

	/**
	* @brief ������ѯ�û�simple user info���������ؽ��(�Ա�getSimpleUserInfoExSignal������������Ϣ).
	* @param IPropBagSetWrapperPtrCR �û���Ϣ���ؽ���б�,���û�id(uid)��ΪID.
	* @param mapContext �û���Ϣ��ص�������.
	* @return �¼�����.
	*/
	typedef _def_boost_signals2_signal_type<void(IPropBagSetWrapperPtrCR, const std::map<UINT32, std::string>& mapContext)>::type	_SIG_SIMPLE_USERINFOEX2;
	virtual _SIG_SIMPLE_USERINFOEX2* getSimpleUserInfoExSignal2() = 0;

	/**
	* @brief ͨ��Webdb��ѯ֧�ִ���������ͨ��������ѯ�û���Ϣ��������querySimpleUinfoEx/getSimpleUserInfoExSignal2��im�����ѯ��
	* !!!!!!!!һ�������ܳ���400��uid,����ֱ�ӷ���һ���յĻظ�!!!!!!!!!!
	* @param uidList �������û�uid����.
	* @param isGetAll���Ƿ�ȡ������Ϣ.
	* @param props   ����Ҫ��ȡ�����Ա�ǩ.
	****props������string�������ݣ�����Ϊ��isGetAll=true��***
	"area"������id
	"birthday"������(19900101)
	"city"������
	"custom_logo"��60*60
	"hd_logo_100"��100*100
	"hd_logo_144"��144*144
	"hd_logo_640"��640*640��webyyͷ��
	"id"���û�uid
	"intro"������˵��
	"jifen"������
	"logo_index"��ϵͳͷ��index
	"nick"���ǳ�
	"provice"��ʡ��
	"sex"���Ա�
	"sign"������ǩ��
	"stage_name"������
	"yyno"���û�YY��
	* @return ���ص���ϢgetUsersInfosByProtocolSignal
	*/
	virtual void queryUsersInfosByProtocol(const std::vector<UINT32>& uidList, bool isGetAll, const std::vector<std::string>& props) = 0;

	/**
	* @brief ������ѯ�û�queryUsersInfosByProtocolo���������ؽ��.
	* @param UINT32 ���ؽ��, 0Ϊ�ɹ�����0Ϊʧ��.
	* @param const std::string& �û���Ϣjson����
	eg:{"uinfos":
	["{\"area\":\"0\",\"birthday\":\"20101128\",\"city\":\"-1\",\"custom_logo\":\"xxx.png\",\"hd_logo_100\":\"http:\\\/\\\/downhdlogo.yy.com\\\/hdlogo\\\/100100\\\/31\\\/69\\\/25\\\/0002256931\\\/u2256931I2uBGl-NVW.png\",\"hd_logo_144\":\"xxx.png\",\"hd_logo_640\":\"xxx.png\",\"id\":\"2256931\",\"intro\":\"\",\"jifen\":\"875711\",\"logo_index\":\"0\",\"nick\":\"LEE",\"province\":\"-1\",\"sex\":\"1\",\"sign\":\"\",\"stage_name\":\"\",\"yyno\":\"305549\"}",
	"{\"area\":\"352\",\"birthday\":\"19990808\",\"city\":\"-1\",\"id\":\"3660707\",\"intro\":\"\\n xxx\",\"jifen\":\"4488539\",\"logo_index\":\"0\",\"nick\":\"﹏ゞ焰龍゛\",\"province\":\"-1\",\"sex\":\"1\",\"sign\":\"xxx\",\"stage_name\":\"\",\"yyno\":\"101338\"}"
	]}
	* @return �¼�����.
	*/
	typedef _def_boost_signals2_signal_type<void(UINT32, const std::string&)>::type	_SIG_GET_USERSINFOS_BYPROTOCOL;
	virtual _SIG_GET_USERSINFOS_BYPROTOCOL* getUsersInfosByProtocolSignal() = 0;
};
