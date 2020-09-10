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
  * @brief .�������ؼ���comָ��.\n
  * signal:void userClicked(UINT32 uid),�����ϵ��һ����ʱ�������ź�;
  */
DWDEFINE_INTERFACE(IChannelMessageEdit) : public IDWQComponent 
{
	/**
	* @brief �����ؼ�.
	* @return �����ؼ�widgetָ��.
	*/
	virtual DuiWidget*	widget() = 0;

/**
   * @brief ����������һ����Ϣ.
   * @param messageItem����Ϣ��ϸ����.
   */
	virtual void appendMessage( const MessageInfo& messageItem ) = 0;

	//����˽����Ϣ.
/**
   * @brief ����������һ��˽����Ϣ.
   * @param messageItem����Ϣ��ϸ����.
   */
	virtual void appendPrivateMessage( const MessageInfo& messageItem ) = 0;

/**
   * @brief �����Զ����ʽ.
   * @return �Զ����ʽ��ʽcomָ�룬�벻Ҫ����ָ������.
   */
	virtual ICompositeFormatPtr createCompositeFormat() = 0;

/**
   * @brief ���뵥���Զ�����Ϣ.
   * @param compositeFormat���Զ����ʽ.
   */
	virtual void appendSingleMessage(ICompositeFormatPtr compositeFormat ) = 0;

/**
   * @brief ����ϵͳ��Ϣ.
   * @param messageType����Ϣ����.
   * @param style����Ϣ��ʽ.
   */
	virtual void configSystemStyle( int messageType, const YYSystemMessageStyle& style ) = 0;

/**
   * @brief �����Ƿ����ͼƬ.
   * @return ����ͼƬ����true�����򷵻�false.
   */
	virtual bool isEnableImage() = 0;

/**
   * @brief �����Ƿ��������.
   * @return �������ַ���true�����򷵻�false.
   */
	virtual bool isEnableText() = 0;

	/**
	* @brief ��Ƶ����һ����Ϣ
	* @param data �������Ϣ������channellogic/iyychannelmessage_i.h��þ���ṹ
	*/
	virtual void appendMessageData(const YYMessageData &data) = 0;

        /**
    * @brief �ú�����������channel message editor
    * @param json�ַ���
    * @return .
    */
    virtual void setControl(const char* json) = 0;

    /**
    * @brief ����һ���������ͣ��������͵���Ϣ�����Զ����������ڶ�����Ϣ
    * @param data ��Ϣ����
    */
    virtual void addMessageTypeFilter(channelmsg::MessageType type) = 0;
    virtual void removeMessageTypeFilter(channelmsg::MessageType type) = 0;
    virtual void clearMessageTypeFilter() = 0;

    /**
    * @brief ����һ����Ϣ����������
    * @param handler ����������
    */
    virtual void addMessageAppenderHandler(IYYMessageAppenderHandler* hander) = 0;

    /**
    * @brief ����һ��������Ϣ����������
    * @param handler ����������
    */
    virtual void appendDefaultAppenderHandler() = 0;

    /**
    * @brief �����Ϣ�������Ĵ�����
    * @param remove_raw true��ʾ����YYԭ���Ĺ�������������Ҳ�����
    */
    virtual void clearMessageAppenderHandler(bool remove_default) = 0;
};


DWDEFINE_INTERFACE(IChannelMessageEditInner) : public IUnk
{
	enum MessageEditType{NoType = 0, PublicEditType = 1, PrivateEditType = 2};

	virtual void setEditType(MessageEditType type) = 0;
};