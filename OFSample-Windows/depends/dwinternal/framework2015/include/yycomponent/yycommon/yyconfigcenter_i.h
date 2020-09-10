#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
signals:
	void configReadyNotify(const std::map<std::string,std::string>& configMap);
*/
//����key��ֵ���ο����ú�̨��http://publess.yy.com/config-mgr?bssid=1029&token=&appid=1001
__declspec(selectany) extern const char KNetBarProcessConfigKey[] = "netbarprocesslist";
__declspec(selectany) extern const char KPreloadTemplateIdsConfigKey[]  = "preloadtemplateids";
__declspec(selectany) extern const char KHttpReportConfigKey[] = "httpreport";//http���سɹ����ϱ�
__declspec(selectany) extern const char* KMetricsReportConfigKey = "metricsreportswitch";//ӥ���ϱ����ؿ��ƽ���ͳһ��Դ��������
__declspec(selectany) extern const char KPersonInfoPageTabs[] = "personInfoPageTabs";
__declspec(selectany) extern const char KSearchWordsTimeConfigKey[] = "searchwordstime";//�������ֲ�ʱ�估���ø���ʱ��
__declspec(selectany) extern const char KWonderentranceConfigKey[] = "wonderentrance";//������������ͼƬ����
__declspec(selectany) extern const char KHotIconConfigKey[] = "hoticonswitch";//Ƶ���ȶ�ֵ����
__declspec(selectany) extern const char KBindPhoneConfigKey[] = "bindphoneswitch";//��¼�ɹ�����ʾδʵ���û����ֻ�����
DWDEFINE_INTERFACE( IYYConfigCenter ) : public IDWQComponent
{
/**
   * @brief ��ʼ��ȡ��������. ע��YY�����ڲ�ʹ��.
   */
	virtual void start(void) = 0;
	virtual bool isReady(void) = 0;
	virtual bool getVaule(const std::string& strKey, std::string& strValue) = 0;
	virtual std::string getSourceJsData(void) = 0;
};
