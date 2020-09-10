#pragma once
#include "dwbase/dwcomex.h"
#include "dwbase/dwcomcreator.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yychannel/yycompositeformat_i.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"

class DuiTextEdit;
class DuiWidget;
struct YYMessageData;

struct YYSystemMessageStyle
{
	QString	IconPath;
	QColor	TextColor;
	QFont	Font;
};

struct MessageInfo
{
	MessageInfo()
	{
		coCreateComInstance(KSERVICEPropbag,spSenderInfo);
	}
	quint32	uid;
	QString	message;
	QFont	font;
	QColor	color;
	IPropBagWrapperPtr spSenderInfo;
};

struct IYYMessageAppender
{
public:
    virtual ~IYYMessageAppender() { }
    virtual bool isRefusable() const = 0;
    virtual void append(DuiTextEdit* editor) = 0;
};

struct IYYMessageAppenderHandler
{
public:
    virtual ~IYYMessageAppenderHandler() { }
    virtual IYYMessageAppender* createAppender(const YYMessageData& messageData) = 0;
};

/**
  * @brief .代表公屏控件的com指针.\n
  * signal:void userClicked(UINT32 uid),公屏上点击一个人时发出的信号;
  */
DWDEFINE_INTERFACE(IChannelMessageEdit) : public IDWQComponent 
{
	/**
	* @brief 公屏控件.
	* @return 公屏控件widget指针.
	*/
	virtual DuiWidget*	widget() = 0;

/**
   * @brief 往公屏插入一条消息.
   * @param messageItem，消息详细内容.
   */
	virtual void appendMessage( const MessageInfo& messageItem ) = 0;

	//插入私聊消息.
/**
   * @brief 往公屏插入一条私聊消息.
   * @param messageItem，消息详细内容.
   */
	virtual void appendPrivateMessage( const MessageInfo& messageItem ) = 0;

/**
   * @brief 创建自定义格式.
   * @return 自定义格式格式com指针，请不要用裸指针拿着.
   */
	virtual ICompositeFormatPtr createCompositeFormat() = 0;

/**
   * @brief 插入单行自定义消息.
   * @param compositeFormat，自定义格式.
   */
	virtual void appendSingleMessage(ICompositeFormatPtr compositeFormat ) = 0;

/**
   * @brief 配置系统消息.
   * @param messageType，消息类型.
   * @param style，消息样式.
   */
	virtual void configSystemStyle( int messageType, const YYSystemMessageStyle& style ) = 0;

/**
   * @brief 公屏是否接受图片.
   * @return 接受图片返回true，否则返回false.
   */
	virtual bool isEnableImage() = 0;

/**
   * @brief 公屏是否接受文字.
   * @return 接受文字返回true，否则返回false.
   */
	virtual bool isEnableText() = 0;

	/**
	* @brief 向公频插入一条消息
	* @param data 插入的消息，引用channellogic/iyychannelmessage_i.h获得具体结构
	*/
	virtual void appendMessageData(const YYMessageData &data) = 0;

        /**
    * @brief 该函数用于配置channel message editor
    * @param json字符串
    * @return .
    */
    virtual void setControl(const char* json) = 0;

    /**
    * @brief 增加一个过滤类型，这种类型的消息不会自动上屏，用于定制消息
    * @param data 消息类型
    */
    virtual void addMessageTypeFilter(channelmsg::MessageType type) = 0;
    virtual void removeMessageTypeFilter(channelmsg::MessageType type) = 0;
    virtual void clearMessageTypeFilter() = 0;

    /**
    * @brief 增加一个消息上屏处理器
    * @param handler 上屏处理器
    */
    virtual void addMessageAppenderHandler(IYYMessageAppenderHandler* hander) = 0;

    /**
    * @brief 增加一个内置消息上屏处理器
    * @param handler 上屏处理器
    */
    virtual void appendDefaultAppenderHandler() = 0;

    /**
    * @brief 清空消息上屏器的处理器
    * @param remove_raw true表示包括YY原本的公屏上屏处理器也清理掉
    */
    virtual void clearMessageAppenderHandler(bool remove_default) = 0;
};


DWDEFINE_INTERFACE(IChannelMessageEditInner) : public IUnk
{
	enum MessageEditType{NoType = 0, PublicEditType = 1, PrivateEditType = 2};

	virtual void setEditType(MessageEditType type) = 0;
};