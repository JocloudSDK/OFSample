#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
/**
* @brief 在频道公屏,频道公告,IM,IM群,IM群公告,IM历史记录添加聊天内容及公告内容的解析实例
*/

/**
* @brief 智能解析接口，由组件实现.
*/
DWDEFINE_INTERFACE(IYYInputIntellisense) : public IUnk
{
	/**
   * @brief 解析text字符串.
   * @param text 将要被解析的字符串. 
   * @param startPos　返回匹配到的字符串的起始位置(以双字节wchar_t为单位的字符索引).
   * @param matchLength　返回匹配到的字符串的长度(以双字节wchar_t为单位的字符长度).
   * @return 解析成功 true，否则返回false.
   */
	virtual bool intellisenseText( const std::wstring& text, int& startPos, int& matchLength ) = 0; 
};


/**
* @brief 组件翻译接口，通过root->getServiceInterface<IYYInputIntellisenseContainer>(KSERVICECommonComponentLevel)来获取.
*/
DWDEFINE_INTERFACE(IYYInputIntellisenseContainer): public IUnk
{
	//解析的位置
	enum WhereIntellisense
	{
		IM = 1,				//IM及群聊窗口
		IMHistroy = 1 << 1,	//IM历史记录
		IMBulletin = 1 << 2,//群公告
		Channel = 1 << 3,   //频道公屏
		ChannelBulletin = 1 << 4,//频道公告
		ErrorPosition,			 //错误的位置,新增位置一定要放在这个的前面定义				 
		ChatContent = IM | Channel,
		Bulletin = IMBulletin | ChannelBulletin,
		ALL = ChatContent | IMHistroy | Bulletin,
	};

	//解析的时机
	enum WhenIntellisense 
	{
		Default,			//默认时机的解析对象，直接加到智能解析链的尾部		
		BeforeWebLink,		//在webLink解析对象之前做智能解析[http://www.???]
		BeforeComponentLink,//在组件链接之前解析[yy://???]
		UrlLinkClicked,		//Url链接被点击的时候［如果在这个时候解析，只有内容text全量被匹配的时候才会响应，即text.length == matchLength, 见IYYInputIntellisense::intellisenseText］
	};

	enum Priority
	{
		PRIORITY_HIGHEST = 0x00,
		PRIORITY_NORMAL_BEFORE = 0x100,
		PRIORITY_NORMAL = 0x200,
		PRIORITY_NORMAL_AFTER = 0x400,
		PRIORITY_NORMAL_LOWEST = 0x800,
	};

/**
   * @brief 添加智能解析对象.
   * @param intellisense 智能解析对象指针. 
   * @param priority　优先级--在同一个WhenIntellisense上,高优先级的优先解析 见Priority定义.
   * @param pos　解析对象在整个解析链中的位置，见WhenIntellisense定义　.
   * @param whereNeedIntellisense 在哪些地方需要解析(见WhereIntellisense），是WhereIntellisense的一个并集.
   * @return 如果intellisense对象已存在，返回false.
   */
	virtual bool addIntellisense( IYYInputIntellisensePtr intellisense, int priority, WhenIntellisense when, int whereNeedIntellisense ) = 0;

	/**
   * @brief 添加智能解析对象.
   * @param regexp 智能解析正则表达式. 
   * @return 如果正则达式已存在该位置返回NULL, 否则返回智能解析对象指针.
   */
	virtual IYYInputIntellisensePtr addIntellisense2( const std::wstring& regexp, int priority, WhenIntellisense when, int whereNeedIntellisense ) = 0;

/**
   * @brief 移除智能解析对象.
   * @param .
   * @return .
   */
	virtual void removeIntellisense( IYYInputIntellisensePtr intellisense ) = 0;
};