#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
signals:
	void configReadyNotify(const std::map<std::string,std::string>& configMap);
*/
//以下key键值，参考配置后台：http://publess.yy.com/config-mgr?bssid=1029&token=&appid=1001
__declspec(selectany) extern const char KNetBarProcessConfigKey[] = "netbarprocesslist";
__declspec(selectany) extern const char KPreloadTemplateIdsConfigKey[]  = "preloadtemplateids";
__declspec(selectany) extern const char KHttpReportConfigKey[] = "httpreport";//http下载成功率上报
__declspec(selectany) extern const char* KMetricsReportConfigKey = "metricsreportswitch";//鹰眼上报开关控制接入统一资源配置中心
__declspec(selectany) extern const char KPersonInfoPageTabs[] = "personInfoPageTabs";
__declspec(selectany) extern const char KSearchWordsTimeConfigKey[] = "searchwordstime";//搜索框轮播时间及配置更新时间
__declspec(selectany) extern const char KWonderentranceConfigKey[] = "wonderentrance";//主面板搜索入口图片配置
__declspec(selectany) extern const char KHotIconConfigKey[] = "hoticonswitch";//频道热度值开关
__declspec(selectany) extern const char KBindPhoneConfigKey[] = "bindphoneswitch";//登录成功后提示未实名用户绑定手机开关
DWDEFINE_INTERFACE( IYYConfigCenter ) : public IDWQComponent
{
/**
   * @brief 开始获取配置数据. 注：YY基础内部使用.
   */
	virtual void start(void) = 0;
	virtual bool isReady(void) = 0;
	virtual bool getVaule(const std::string& strKey, std::string& strValue) = 0;
	virtual std::string getSourceJsData(void) = 0;
};
