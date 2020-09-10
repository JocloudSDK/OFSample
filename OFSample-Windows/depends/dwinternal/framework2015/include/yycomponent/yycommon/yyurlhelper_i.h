#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYUrlhelper):public IUnk
{
	/**
	* @brief 打开创建频道页面.
	* @param .
	* @return .
	*/
	virtual void goToGuildCreate() = 0;

	/**
	* @brief 打开公会管理页面.
	* @param .
	* @return .
	*/
	virtual void goToGuildManage(unsigned int sid) = 0;

	/**
	* @brief 打开频道日志页面.
	* @param .
	* @return .
	*/
	virtual void goToSessionLog(unsigned int sid) = 0;

	/**
	* @brief 打开举报页面.
	* @param .
	* @return .
	*/
	virtual void goToInform(unsigned int sid) = 0;

	/**
	* @brief 直接获取用于登录的key.
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
	* @brief 专为陆铭组提供的，得到去商城的url的后缀.
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
	* @brief 打开YY成就.
	* @param .
	* @return .
	*/
	virtual void gotoYYAchievement (unsigned int uid ) = 0; 

	/**
	* @brief 以安全中心方式打开的URL.已废弃. 
	* @param .
	* @return.
	*/
	virtual std::wstring securityUrl( const std::wstring& url ) = 0;

	/**
	* @brief 获取登陆信息。打开应用中心及师徒系统的页面时需加上登陆信息.
	* @param .
	* @return .
	*/
	virtual std::wstring getLoginInfo(unsigned int channelSID) = 0;

	/**
	* @brief 新游推荐.
	* @param .
	* @return .
	*/
	virtual std::wstring getKeyForNewGameRecommend() = 0;

	/**
	* @brief . 新的udb自动登录网页,支持ticket机制
	* @param . url 打开的url
	* @param . busiId UDB分配给busiURL业务系统的appid。注：关于busiId，①要求新接入系统按真实值填写；②已接入系统可以忽略该参数，但期望有发版计划时升级，如实填写真实值；
	* @param . direct  是否只写busiURL所在顶级域的COOKIE标识；(可选)
	具体说明
	①若不要求只写busiURL所在顶级域的COOKIE，可忽略此参数；

	②若要求只写busiURL所在顶级域的COOKIE，则direct=1，此时，需要注意以下两点：
	a.http://udb.duowan.com需要替换为http://udb.$domain
	$domain视业务系统而定，参考业务系统的回调busiURL，$domain取busiURL所属的顶级域，如：
	若busiURL=http://m.yy.com/?7090，则$domain=yy.com，
	以此类推，
	若busiURL=http://gh.yy.duowan.com/?89009，则$domain=duowan.com
	注：属于公司有效域名$domain有duowan.com，yy.com，bengou.com，kuaikuai.cn，以及sc2.com.cn； 
	b.在direct=1，只写单域COOKIE情况下是否需要回调busiURL，取决于业务系统需求，如：
	若业务系统需要回调，则填充busiURL的相应值；
	若业务系统不需要回调，则可忽略该参数；
	* @return. 
	*/
	virtual void openSecurityUrl(const std::wstring& url, const int busiId, const int direct ) = 0;

	/**
	* @brief . YY客户端8.1版本新增的udb自动登录网页,支持ticket机制
	* @param . url 打开的url
	* @param . busiId UDB分配给busiURL业务系统的appid。注：关于busiId，①要求新接入系统按真实值填写；②已接入系统可以忽略该参数，但期望有发版计划时升级，如实填写真实值；
	* @param . direct  是否只写busiURL所在顶级域的COOKIE标识；(可选)
	* @param . reqDomainList  业务系统需要些Cookie的域名集合（注：优先级高的域放前面，以此类推）；(可选)
	参数补充说明——
	①业务系统可通过reqDomainList来通知要写那些域的Cookie，通过半角的逗号分隔，如：reqDomainList为lgn.yy.com,lgn.duowan.com表示要写yy.com和duowan.com这两个域的Cookie.
	注：
	②属于公司有效域名且可写Cookie的域名包括（具体值由SDK中CookieDomainEnum类的final static属性定义，以下仅供参考范围）：
	lgn.duowan.com，lgn.yy.com，lgn.kuaikuai.cn，lgn.5253.com，lgn.100.com，lgn.1931.com，lgn.5153.com， lgn.yy.tv，lgn.yyclouds.com； 

	写Cookie的域名范围的规则：
	①先判断direct是否等于1，是则只写当前域的Cookie(默认值写yy.com域的Cookie)
	②再判断reqDomainList:是否有域名，有则写指定的域名Cookie
	③如果上面两种情况都不满足，则写默认域：yy.com,duowan.com,kuaikuai.cn

	写Cookie的例子：
	①业务系统请求lgn.yy.com,
	入参：direct=1
	则只写yy.com域的Cookie
	②业务系统请求lgn.yy.com
	入参：direct为非1或null，reqDomainList包括lgn.duowan.com,lgn.1931.com,
	则：写duowan.com和1931.com的Cookie
	③业务系统请求lgn.yy.com，direct和reqDomainList都不传，则默认写yy.com,duowan.com,kuaikuai.cn这三个域的Cookie

	具体说明——
	①若不要求只写busiURL所在顶级域的COOKIE，可忽略此参数；

	②若要求只写busiURL所在顶级域的COOKIE，则direct=1，此时，需要注意以下两点：
	a.http://udb.duowan.com需要替换为http://udb.$domain
	$domain视业务系统而定，参考业务系统的回调busiURL，$domain取busiURL所属的顶级域，如：
	若busiURL=http://m.yy.com/?7090，则$domain=yy.com，
	以此类推，
	若busiURL=http://gh.yy.duowan.com/?89009，则$domain=duowan.com
	注：属于公司有效域名$domain有duowan.com，yy.com，bengou.com，kuaikuai.cn，以及sc2.com.cn； 
	b.在direct=1，只写单域COOKIE情况下是否需要回调busiURL，取决于业务系统需求，如：
	若业务系统需要回调，则填充busiURL的相应值；
	若业务系统不需要回调，则可忽略该参数；
	* @return. 
	*/
	// 注意！！！
	// 8.38新增：使用时打开url必须带上非0值的busiId，如不知道，请找url业务方询问接udb的appid
	virtual void openSecurityUrl2(const std::wstring& url, const int busiId, const int direct, const std::wstring& reqDomainList ) = 0;
};