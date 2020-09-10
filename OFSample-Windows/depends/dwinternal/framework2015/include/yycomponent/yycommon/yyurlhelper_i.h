#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYUrlhelper):public IUnk
{
	/**
	* @brief �򿪴���Ƶ��ҳ��.
	* @param .
	* @return .
	*/
	virtual void goToGuildCreate() = 0;

	/**
	* @brief �򿪹������ҳ��.
	* @param .
	* @return .
	*/
	virtual void goToGuildManage(unsigned int sid) = 0;

	/**
	* @brief ��Ƶ����־ҳ��.
	* @param .
	* @return .
	*/
	virtual void goToSessionLog(unsigned int sid) = 0;

	/**
	* @brief �򿪾ٱ�ҳ��.
	* @param .
	* @return .
	*/
	virtual void goToInform(unsigned int sid) = 0;

	/**
	* @brief ֱ�ӻ�ȡ���ڵ�¼��key.
	* @param .
	* @return .
	*/
	virtual std::wstring getKeyForWebLogin() = 0;

	/**
	* @brief .
	* @return .
	*/
	virtual void goToShop() = 0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void gotoYZone(const std::wstring & urlZone, const std::wstring& data) = 0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void gotoPayCenter(const std::wstring &preFix, const std::wstring &postFix) = 0;

	/**
	* @brief .
	* @return .
	*/
	virtual void gotoOpenId() = 0;
	/**
	* @brief רΪ½�����ṩ�ģ��õ�ȥ�̳ǵ�url�ĺ�׺.
	* @param .
	* @return .
	*/
	virtual std::wstring getShopKey() = 0;

	/**
	* @brief .
	* @return .
	*/
	virtual std::wstring getZoneKey() = 0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring guildManageUrl(unsigned int sid) = 0;

	/**
	* @brief ��YY�ɾ�.
	* @param .
	* @return .
	*/
	virtual void gotoYYAchievement (unsigned int uid ) = 0; 

	/**
	* @brief �԰�ȫ���ķ�ʽ�򿪵�URL.�ѷ���. 
	* @param .
	* @return.
	*/
	virtual std::wstring securityUrl( const std::wstring& url ) = 0;

	/**
	* @brief ��ȡ��½��Ϣ����Ӧ�����ļ�ʦͽϵͳ��ҳ��ʱ����ϵ�½��Ϣ.
	* @param .
	* @return .
	*/
	virtual std::wstring getLoginInfo(unsigned int channelSID) = 0;

	/**
	* @brief �����Ƽ�.
	* @param .
	* @return .
	*/
	virtual std::wstring getKeyForNewGameRecommend() = 0;

	/**
	* @brief . �µ�udb�Զ���¼��ҳ,֧��ticket����
	* @param . url �򿪵�url
	* @param . busiId UDB�����busiURLҵ��ϵͳ��appid��ע������busiId����Ҫ���½���ϵͳ����ʵֵ��д�����ѽ���ϵͳ���Ժ��Ըò������������з���ƻ�ʱ��������ʵ��д��ʵֵ��
	* @param . direct  �Ƿ�ֻдbusiURL���ڶ������COOKIE��ʶ��(��ѡ)
	����˵��
	������Ҫ��ֻдbusiURL���ڶ������COOKIE���ɺ��Դ˲�����

	����Ҫ��ֻдbusiURL���ڶ������COOKIE����direct=1����ʱ����Ҫע���������㣺
	a.http://udb.duowan.com��Ҫ�滻Ϊhttp://udb.$domain
	$domain��ҵ��ϵͳ�������ο�ҵ��ϵͳ�Ļص�busiURL��$domainȡbusiURL�����Ķ������磺
	��busiURL=http://m.yy.com/?7090����$domain=yy.com��
	�Դ����ƣ�
	��busiURL=http://gh.yy.duowan.com/?89009����$domain=duowan.com
	ע�����ڹ�˾��Ч����$domain��duowan.com��yy.com��bengou.com��kuaikuai.cn���Լ�sc2.com.cn�� 
	b.��direct=1��ֻд����COOKIE������Ƿ���Ҫ�ص�busiURL��ȡ����ҵ��ϵͳ�����磺
	��ҵ��ϵͳ��Ҫ�ص��������busiURL����Ӧֵ��
	��ҵ��ϵͳ����Ҫ�ص�����ɺ��Ըò�����
	* @return. 
	*/
	virtual void openSecurityUrl(const std::wstring& url, const int busiId, const int direct ) = 0;

	/**
	* @brief . YY�ͻ���8.1�汾������udb�Զ���¼��ҳ,֧��ticket����
	* @param . url �򿪵�url
	* @param . busiId UDB�����busiURLҵ��ϵͳ��appid��ע������busiId����Ҫ���½���ϵͳ����ʵֵ��д�����ѽ���ϵͳ���Ժ��Ըò������������з���ƻ�ʱ��������ʵ��д��ʵֵ��
	* @param . direct  �Ƿ�ֻдbusiURL���ڶ������COOKIE��ʶ��(��ѡ)
	* @param . reqDomainList  ҵ��ϵͳ��ҪЩCookie���������ϣ�ע�����ȼ��ߵ����ǰ�棬�Դ����ƣ���(��ѡ)
	��������˵������
	��ҵ��ϵͳ��ͨ��reqDomainList��֪ͨҪд��Щ���Cookie��ͨ����ǵĶ��ŷָ����磺reqDomainListΪlgn.yy.com,lgn.duowan.com��ʾҪдyy.com��duowan.com���������Cookie.
	ע��
	�����ڹ�˾��Ч�����ҿ�дCookie����������������ֵ��SDK��CookieDomainEnum���final static���Զ��壬���½����ο���Χ����
	lgn.duowan.com��lgn.yy.com��lgn.kuaikuai.cn��lgn.5253.com��lgn.100.com��lgn.1931.com��lgn.5153.com�� lgn.yy.tv��lgn.yyclouds.com�� 

	дCookie��������Χ�Ĺ���
	�����ж�direct�Ƿ����1������ֻд��ǰ���Cookie(Ĭ��ֵдyy.com���Cookie)
	�����ж�reqDomainList:�Ƿ�������������дָ��������Cookie
	�����������������������㣬��дĬ����yy.com,duowan.com,kuaikuai.cn

	дCookie�����ӣ�
	��ҵ��ϵͳ����lgn.yy.com,
	��Σ�direct=1
	��ֻдyy.com���Cookie
	��ҵ��ϵͳ����lgn.yy.com
	��Σ�directΪ��1��null��reqDomainList����lgn.duowan.com,lgn.1931.com,
	��дduowan.com��1931.com��Cookie
	��ҵ��ϵͳ����lgn.yy.com��direct��reqDomainList����������Ĭ��дyy.com,duowan.com,kuaikuai.cn���������Cookie

	����˵������
	������Ҫ��ֻдbusiURL���ڶ������COOKIE���ɺ��Դ˲�����

	����Ҫ��ֻдbusiURL���ڶ������COOKIE����direct=1����ʱ����Ҫע���������㣺
	a.http://udb.duowan.com��Ҫ�滻Ϊhttp://udb.$domain
	$domain��ҵ��ϵͳ�������ο�ҵ��ϵͳ�Ļص�busiURL��$domainȡbusiURL�����Ķ������磺
	��busiURL=http://m.yy.com/?7090����$domain=yy.com��
	�Դ����ƣ�
	��busiURL=http://gh.yy.duowan.com/?89009����$domain=duowan.com
	ע�����ڹ�˾��Ч����$domain��duowan.com��yy.com��bengou.com��kuaikuai.cn���Լ�sc2.com.cn�� 
	b.��direct=1��ֻд����COOKIE������Ƿ���Ҫ�ص�busiURL��ȡ����ҵ��ϵͳ�����磺
	��ҵ��ϵͳ��Ҫ�ص��������busiURL����Ӧֵ��
	��ҵ��ϵͳ����Ҫ�ص�����ɺ��Ըò�����
	* @return. 
	*/
	// ע�⣡����
	// 8.38������ʹ��ʱ��url������Ϸ�0ֵ��busiId���粻֪��������urlҵ��ѯ�ʽ�udb��appid
	virtual void openSecurityUrl2(const std::wstring& url, const int busiId, const int direct, const std::wstring& reqDomainList ) = 0;
};