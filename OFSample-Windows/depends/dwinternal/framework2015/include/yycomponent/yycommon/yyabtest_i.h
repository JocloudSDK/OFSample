#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"

/**
   * http://doc.yy.com/pages/viewpage.action?pageId=12322839
   * @brief YY abtest interface.
   */
DWDEFINE_INTERFACE(IYYABTEST) : public IUnk
{
	/**
	* @brief .Ļ������һ��https������abtest�ӿڣ�ԭ����Ӧ������ʱ����һ�μ��ɣ�����ҵ����ҪҲ�ɶ�ε���
	* @param wStrAppIdҵ��id����abtest�����̨����
	* @param wStrAppVersionҵ��汾�ţ���ʽx.x.x����x.x.x.x
	* @param nUserId�û�UID
	* @param wStrfilterJsonҵ����չ���˲�����json��ʽ
	* @return .
	*/
	virtual bool initABTest(const std::wstring &wStrAppId,const std::wstring &wStrAppVersion,const unsigned int &nUserId,const std::wstring &wStrfilterJson) = 0;

	typedef _def_boost_signals2_signal_type<void (const std::wstring& , bool )>::type	_SIG_ABTEST_ARRIVE;
	/**
	* @brief .��ȡ����InitABTest�������󷵻ؽ���ɹ�����ʧ�ܵ��ź�
	* @param .wStrAppId�Լ�ҵ���appid
	* @return .
	*/
	virtual _SIG_ABTEST_ARRIVE *getABTestArriveSignal(const std::wstring &wStrAppId) = 0;

	/**
	* @brief ��ȡҵ�������Ƿ�׼������
	* @param wStrAppId�Լ�ҵ���appid
	* @return 
	*/
	virtual bool isReady(const std::wstring &wStrAppId) = 0;

	/**
	* @brief ��ѯ����ʵ��ֵ���˺�����ҪisReady����true����getABTestArriveSignal�źŶ����Ĳۺ�������true�ſ��Խ��е���
	* @param wStrLayerid abtest�����̨���õ�ʵ�������
	* @param bReport	�Ƿ��ϱ����ȴ˴�ʵ������
	* @param nValuesize Ĭ��ʵ��ֵjson�ַ������wchar_t�����С�������ô�С�Զ��ض�
	* @return �ò��Ӧ��ʵ��ֵjson��ʽ���߿�
	*/
	virtual std::wstring queryABTest(const std::wstring &wStrLayerid,bool bReport=true,unsigned int nValuesize=1024) = 0;
	
};

//Example
//ҵ���ʼ���ĵط�����һ��
//#define APPID L"pc-yy"
//if (IYYABTESTPtr spABTest = comMgr()->getServiceInterface<IYYABTEST>(KSERVICECommon))
//{
//	spABTest->initABTest(APPID,L"8.51.0.0",bizGetPassportSingleton()->getMyUid(),L"");
//}

//ȡʵ�����ݿɶ�ε���
//if (IYYABTESTPtr spABTest = comMgr()->getServiceInterface<IYYABTEST>(KSERVICECommon))
//{
//	if(spABTest->isReady(APPID))
//	{
//		on_abtestReady(APPID,true);
//	}
//	else
//	{
//		xsignals_helper::sig_connect(*(spABTest->getABTestArriveSignal(APPID)),this,&ClassName::on_abtestReady);
//	}
//}
//void on_abtestReady(const std::wstring& appId, bool bSuccess)
//{
//	if(appId.compare(L"pc-yy")==0)
//	{
//		if(bSuccess)
//		{
//			if (IYYABTESTPtr spABTest = comMgr()->getServiceInterface<IYYABTEST>(KSERVICECommon))
//			{
//				QString layerValue = QString::fromStdWString(spABTest->queryABTest(APPID,true));
//			}
//		}
//	}
//}
//
