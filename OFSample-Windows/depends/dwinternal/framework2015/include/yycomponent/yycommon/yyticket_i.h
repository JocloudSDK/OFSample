#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
use for getObject();

signals:
	void ticketAction(int scene, int actionType, const QString& data, const QString& description="");
	void ticketAction2(int scene, int actionType, const QString& data, const QString& description, int msgType);
*/

/* ˵���ĵ�:
	http://wiki.yypm.com/pages/viewpage.action?pageId=15894218
*/

class QImage;
class DuiTextCursor;
DWDEFINE_INTERFACE( IYYTicket ) : public IDWQComponent
{
	enum TextRole
	{
		NoText = 0,
		Description = 1,				// �ɻ�Ʊ��������Ϣ��
		ScecePlainText = 2,			//	���봿����( �Ѳ��� )��
		CopyText = 3,					// ���ø��ƵĴ���
		DisableImageText = 4,		// ����ֹ��ʾͼƬʱ����ʾ���ַ�����
	};

	enum ImageRole
	{
		NoImage,
		Icon,					// �ɻ�Ʊ��ߵ�Сͼ��.
		PastedIcon,		// �ɻ�Ʊճ���������ʱ��ͼƬ.
		TicketImage,		// ImageStyle �²����ͼƬ.
		AsyncValidateImage
	};

	enum TicketAction
	{
		NoAction,
		InputAction	= 1,		// 
		ClickedAction	= 2,
		GetSendDescriptionAction = 3,
		CheckReceiveDescriptionAction = 4, // �Ѳ���
		WebJoinAction = 5,		//��ҳ��ת
		CheckReceiveValidateAction = 6, // У��������ݺϷ���������Ϸ���������.
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
		SortStyle,		// �̷ɻ�Ʊ��ʽ.
		LongStyle,		// ���ɻ�Ʊ��ʽ.
		ImageStyle,	// ��ͨͼƬ��ʽ.
		AsyncValidateStyle, //�첽У����ʽ.
		CustomizedStyle, //ҵ���Լ������Զ���QTextFormat����
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
   * @brief ת����JS�ű�.
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
   * @brief Ĭ������ǿ��Ը��Ƶ�, Ĭ�ϸ����ı�Ϊ�����ɻ�Ʊ.
   * ����Ҫ���ĸ����ı����� setText(CopyText, ��some text" );.
   * @param canCopy.
   */
	virtual void setEnableCopy( bool canCopy ) = 0;
	
/**
   * @brief ���ý��յ��ķɻ�Ʊ�Ƿ�Ϸ�.
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
   * @brief �����첽У��״̬����������һ����������\n.
   * ��У������ʱ�̣����У����Ϣ��û��׼���ã������Ȱɷɻ�Ʊ���ó� AsyncStyle\n.
   * ������׼�����ˣ�����һ�� updateAsyncTicket.
   * @param data data.
   */
	virtual void updateAsyncTicket(const QString& data) = 0;

	/**
   * @brief �����ӦͼƬ��bytes.
   * @param imageBytes ��ӦͼƬ��bytes.
   * @return
   */
	virtual void setImageBytes(ImageRole role, const QByteArray& imageBytes ) = 0;

	enum CurrentFormatPropertyIndex
	{
		KBlockFullText = 103,		//������Ϣ����
		KSenderVipGrade = 104, //������vip�ȼ�
		KCurrentFrameId = 108, //������Ϣ��ǰ����frame��WId
		KMessageUidKey = 1007, //������uid
		KMessageEditKey = 1009 ////Ψһ��ʶÿ��textedit
	};
/**
   * @brief ��ȡ��ǰ��QTextFormat�����Ի�ȡ��Ϣ����.
   * @return ��ǰ��QTextFormat.
   */
	virtual QTextFormat currentFormat() = 0;

	enum CustomizedFormatPropertyIndex
	{
		KImageUnCopyable = 102, //�Ƿ���Ը���  trueΪ�����Ը���
		KFormatType = 801, //��ʽ���� 0Ϊ�ı�(����̳���QTextCharFormat),1ΪͼƬ(����̳���QTextImageFormat)
		KTextData = 802,	//��������, QSting
		KImageData = 803,	//ͼƬ����, QByteArray
		KImageKey = 804, // ͼƬkey
		KLinkValue = QTextFormat::UserProperty + 2, //������ӵ�ֵ
		KPasteTicketText = QTextCharFormat::UserProperty + 1002,  //ճ��ʱ���ı�
	};
/**
   * @brief ��Ӳ�����Զ����ʽ����,����Ӷ��Ϸ���,����indexΪCustomizedFormatPropertyIndex.
   * @return
   */
	virtual void addCustomizedFormat(QTextFormat* format) = 0;

	/**
	* @brief �������÷ɻ�Ʊ�Ƿ�ת�ı��Ƿ���ʾ���ھ���ͼƬ�ͱ��������£��ɻ�Ʊ��ת���ı���
	* @return
	*/
	virtual void setIsIgnoreTicketText(bool v) = 0;
};
