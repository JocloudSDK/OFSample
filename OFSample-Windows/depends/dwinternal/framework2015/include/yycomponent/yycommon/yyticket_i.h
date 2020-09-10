#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
use for getObject();

signals:
	void ticketAction(int scene, int actionType, const QString& data, const QString& description="");
	void ticketAction2(int scene, int actionType, const QString& data, const QString& description, int msgType);
*/

/* 说明文档:
	http://wiki.yypm.com/pages/viewpage.action?pageId=15894218
*/

class QImage;
class DuiTextCursor;
DWDEFINE_INTERFACE( IYYTicket ) : public IDWQComponent
{
	enum TextRole
	{
		NoText = 0,
		Description = 1,				// 飞机票的描述信息。
		ScecePlainText = 2,			//	插入纯文字( 已不用 )。
		CopyText = 3,					// 设置复制的串。
		DisableImageText = 4,		// 当禁止显示图片时，显示的字符串。
	};

	enum ImageRole
	{
		NoImage,
		Icon,					// 飞机票左边的小图标.
		PastedIcon,		// 飞机票粘贴到输入框时的图片.
		TicketImage,		// ImageStyle 下插入的图片.
		AsyncValidateImage
	};

	enum TicketAction
	{
		NoAction,
		InputAction	= 1,		// 
		ClickedAction	= 2,
		GetSendDescriptionAction = 3,
		CheckReceiveDescriptionAction = 4, // 已不用
		WebJoinAction = 5,		//网页跳转
		CheckReceiveValidateAction = 6, // 校验接收数据合法，如果不合法将不解析.
		TransferData = 7,
	};

	enum UseScene
	{
		NoScene,
		ChannelScene,
		ImScene,
		GroupScene,
		WebScene,
		ComponentScene,
		ChannelBulletinScene,
		GroupBulletinScene,
	};

	enum TicketStyle
	{
		SortStyle,		// 短飞机票样式.
		LongStyle,		// 长飞机票样式.
		ImageStyle,	// 普通图片样式.
		AsyncValidateStyle, //异步校验样式.
		CustomizedStyle, //业务自己插入自定义QTextFormat处理
	};

/**
   * @brief set text.
   * @param .
   * @param .
   */
	virtual void setText(TextRole scene, const QString& description ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setImage(ImageRole scene, const QImage& image ) = 0;
	
/**
   * @brief 转发给JS脚本.
   * @param .
   * @return .
   */
	virtual void setScriptTicketAction(const QString& key, TicketAction actionType, const QString& data) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */	
	virtual void setStyle(TicketStyle style) = 0;

/**
   * @brief 默认情况是可以复制的, 默认复制文本为整个飞机票.
   * 当需要更改复制文本调用 setText(CopyText, ”some text" );.
   * @param canCopy.
   */
	virtual void setEnableCopy( bool canCopy ) = 0;
	
/**
   * @brief 设置接收到的飞机票是否合法.
   * @param .
   * @return .
   */
	virtual void setIsValidate( bool validate ) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setImagePath(ImageRole role, const QString& imagePath ) = 0;

/**
   * @brief 更新异步校验状态，会重新走一次正常流程\n.
   * 在校验数据时刻，如果校验信息还没有准备好，可以先吧飞机票设置成 AsyncStyle\n.
   * 当数据准备好了，触发一下 updateAsyncTicket.
   * @param data data.
   */
	virtual void updateAsyncTicket(const QString& data) = 0;

	/**
   * @brief 插入对应图片的bytes.
   * @param imageBytes 对应图片的bytes.
   * @return
   */
	virtual void setImageBytes(ImageRole role, const QByteArray& imageBytes ) = 0;

	enum CurrentFormatPropertyIndex
	{
		KBlockFullText = 103,		//发送消息内容
		KSenderVipGrade = 104, //发送者vip等级
		KCurrentFrameId = 108, //发送消息当前所在frame的WId
		KMessageUidKey = 1007, //发送者uid
		KMessageEditKey = 1009 ////唯一标识每个textedit
	};
/**
   * @brief 获取当前的QTextFormat，可以获取消息属性.
   * @return 当前的QTextFormat.
   */
	virtual QTextFormat currentFormat() = 0;

	enum CustomizedFormatPropertyIndex
	{
		KImageUnCopyable = 102, //是否可以复制  true为不可以复制
		KFormatType = 801, //格式类型 0为文本(必须继承自QTextCharFormat),1为图片(必须继承自QTextImageFormat)
		KTextData = 802,	//文字数据, QSting
		KImageData = 803,	//图片数据, QByteArray
		KImageKey = 804, // 图片key
		KLinkValue = QTextFormat::UserProperty + 2, //点击链接的值
		KPasteTicketText = QTextCharFormat::UserProperty + 1002,  //粘贴时的文本
	};
/**
   * @brief 添加插入的自定义格式内容,必须从堆上分配,属性index为CustomizedFormatPropertyIndex.
   * @return
   */
	virtual void addCustomizedFormat(QTextFormat* format) = 0;

	/**
	* @brief 增加设置飞机票是否转文本是否显示（在拒收图片和表情的情况下，飞机票会转成文本）
	* @return
	*/
	virtual void setIsIgnoreTicketText(bool v) = 0;
};
