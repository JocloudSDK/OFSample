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
	* @brief .幕后动作发一个https请求后端abtest接口，原则上应用启动时调用一次即可，如有业务需要也可多次调用
	* @param wStrAppId业务id，上abtest管理后台申请
	* @param wStrAppVersion业务版本号，格式x.x.x或者x.x.x.x
	* @param nUserId用户UID
	* @param wStrfilterJson业务拓展过滤参数，json格式
	* @return .
	*/
	virtual bool initABTest(const std::wstring &wStrAppId,const std::wstring &wStrAppVersion,const unsigned int &nUserId,const std::wstring &wStrfilterJson) = 0;

	typedef _def_boost_signals2_signal_type<void (const std::wstring& , bool )>::type	_SIG_ABTEST_ARRIVE;
	/**
	* @brief .获取上面InitABTest网络请求返回结果成功或者失败的信号
	* @param .wStrAppId自己业务的appid
	* @return .
	*/
	virtual _SIG_ABTEST_ARRIVE *getABTestArriveSignal(const std::wstring &wStrAppId) = 0;

	/**
	* @brief 获取业务数据是否准备好了
	* @param wStrAppId自己业务的appid
	* @return 
	*/
	virtual bool isReady(const std::wstring &wStrAppId) = 0;

	/**
	* @brief 查询具体实验值，此函数需要isReady返回true或者getABTestArriveSignal信号对饮的槽函数返回true才可以进行调用
	* @param wStrLayerid abtest管理后台配置的实验层名称
	* @param bReport	是否上报海度此次实验命中
	* @param nValuesize 默认实验值json字符串最大wchar_t数组大小，超过该大小自动截断
	* @return 该层对应的实验值json格式或者空
	*/
	virtual std::wstring queryABTest(const std::wstring &wStrLayerid,bool bReport=true,unsigned int nValuesize=1024) = 0;
	
};

//Example
//业务初始化的地方调用一次
//#define APPID L"pc-yy"
//if (IYYABTESTPtr spABTest = comMgr()->getServiceInterface<IYYABTEST>(KSERVICECommon))
//{
//	spABTest->initABTest(APPID,L"8.51.0.0",bizGetPassportSingleton()->getMyUid(),L"");
//}

//取实验数据可多次调用
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
