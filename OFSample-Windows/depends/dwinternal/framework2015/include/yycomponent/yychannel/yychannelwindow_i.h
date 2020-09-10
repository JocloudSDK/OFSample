#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yychannel/yychannelmessageedit_i.h"
#include "yycomponent/yychannel/yychannelinputedit_i.h"
#include "yycomponent/yychannel/yychannelusertreewidget_i.h"
#include "yycomponent/yychannel/yychannelmanagerwidget_i.h"
#include "yycomponent/yychannel/yychannelmaixuwidget_i.h"
#include "yycomponent/yychannel/yychannelsearchwidget_i.h"
#include "yycomponent/yychannel/yychannelprivatechatautoreply_i.h"
#include "yycomponent/yychannel/yychannelchatquickreply_i.h"

class DuiFrameWindow;
class DuiMenu;

namespace subWin
{
	typedef  QMap<QString, QVariant> Args;
	enum SubWindow
	{
		AudioSetting,
		KalaokSetting,
		FavorWindow,
		RecordFinish, // arg KRecordPath
        F2EventGetFailed,//no administator login windows get Key or mouse event failed
		ChangeTemplate,//���ܷ���NULL[�ڲ���ȡ����ģ���б�ʱ]
        LaunchSelectorWindow,// ����ѡ��window
	};
	const char KRecordPath[] = "recordPath";
}

namespace popWin
{
	typedef  QMap<QString, QVariant> Args;
	enum PopWindow
	{
		ResetPlayer,
		ChannelInfo,
		VoiceWizard,
		KickOff,//T�� args.insert(menuData::KUid, uid);
		Ban//���� args.insert(menuData::KUid, uid);
	};
}

namespace menuData // ����menuʱ����Ҫ������.
{
	const char KUid[] = "uid";
	const char KSid[] = "sid";
	const char KUserName[] = "userName";
	const char KItemIndex[] = "itemIndex";
	const char KPoint[] = "point";
    const char KUserRole[] = "userRole";        // �û��˳�Ƶ����������Ϣ���Ҽ��˵���Ҫ�õ���������
}

namespace Decorator
{
	typedef enum
	{
		Maixu,
		ManagerList,
		ChannelTree,
        MessageList
	} Scenes;
	
	typedef enum
	{
		DecoratorBegin = 0,

		/**
		* @brief ����ض��û����ǳ���ɫʱ���ô˺���
		* @param scene ������ֵΪScenesö��֮һ
		* @param uid �û�uid
		* @param color ���ص���ɫ
		* @param handled �Ƿ��Ѿ����������߲���Ҫ��ӣ��Ѵ�����д��0������Ϊ0
		*/
		// void nickColor(quint32 scene, quint32 uid, QRgb *color, qint32 *handled)";
		NickColor = DecoratorBegin,
		
		DecoratorEnd,
	} Method;
}
/*
    use getObject function, have to release the smart pointer when component onUnload
*/
DWDEFINE_INTERFACE(IYYChannelWindow) : public IUnk
{
	typedef QMap<QString, QVariant> Args;

    /**
	* @brief Ƶ������,�������ڴ�����󻯣���С�������أ���ʾ����ԭ�����ñ���ͼ������titlebar�ȣ������������ô��ڴ�С��ֱ�Ӳ�������HWND���������׵���UI���쳣.
	* @return Ƶ������.
	*/
    virtual DuiFrameWindow* channelWindow() = 0;

   /**
   * @brief ���������ؼ��ӿڡ�����Ȩ������ߣ����ó�Ա�����ȷ������������������ü���Ϊ��ʱ��������.
   * @param parent�����ָ��׽ڵ�.
   * @return �����ؼ��ӿ�ָ��.
   */
	virtual IChannelMessageEditPtr createMessageWidget(QGraphicsItem* parent) = 0;

   /**
   * @brief ���������ӿڡ�����Ȩ������ߣ����ó�Ա�����ȷ������������������ü���Ϊ��ʱ��������.
   * @param parent�����ָ��׽ڵ�.
   * @return �����ӿ�ָ��.
   */
	virtual IChannelInputEditPtr createInputWidget(QGraphicsItem* parent) = 0;

	/**
	* @brief �����û����ؼ��ӿڡ�����Ȩ������ߣ����ó�Ա�����ȷ������������������ü���Ϊ��ʱ��������.
	* @param parent�����ָ��ڵ�.
	* @return �û����ؼ��ӿ�ָ��.
	*/
	virtual IChannelUserTreeWidgetPtr createUserTreeWidget(DuiWidget* parent) = 0;

	/**
	* @brief ��������Ա�б�ؼ��ӿڡ�����Ȩ������ߣ����ó�Ա�����ȷ������������������ü���Ϊ��ʱ��������.
	* @param parent�����ָ��ڵ�.
	* @return ����Ա�б�ؼ��ӿ�ָ��.
	*/
	virtual IChannelManagerWidgetPtr createManagerWidgt(DuiWidget* parent) = 0;


	/**
	* @brief ��������ؼ��ӿڡ�����Ȩ������ߣ����ó�Ա�����ȷ������������������ü���Ϊ��ʱ��������.
	* @param parent�����ָ��ڵ�.
	* @return ����ؼ��ӿ�ָ��.
	*/
	virtual IChannelMaixuWidgetPtr createMaixuWidget(DuiWidget* parent) = 0;


	/**
	* @brief ����Ƶ���û������ؼ��ӿڡ�����Ȩ������ߣ����ó�Ա�����ȷ������������������ü���Ϊ��ʱ��������.
	* @param parent�����ָ��ڵ�.
	* @return ����ؼ��ӿ�ָ��.
	*/
	virtual IChannelSearchWidgetPtr createSearchWidget(DuiWidget* parent) = 0;


	/**
	* @brief ��ȡƵ�����в˵�����������˵�����Ӧ������ҵ��������ʾ,�ֵ�����.

	* @param menuName���˵�����
			UserContextMenu			:Ƶ�����û�item�͹����û�item
			TreeFolderContextMenu	:Ƶ����Folder
			HeaderContextMenu		:Ƶ��ͼ��
			TextEditContextMenu		:������˽��
			EditContextMenu			:Ƶ�����Ϳ�

	* @param QMap<QString, QVariant> &param��param ����˵�����Ҫ�Ĳ���
			��Ƶ���й���		param.insert(menuData::KSid, channelId);
			���û��й���		param.insert(menuData::KUid, userId);
			��item�����й���	param.insert(menuData::KItemIndex, itemIndex);
			��char��ʽ�й���	param.insert(menuData::KPoint, event->pos());
	* @return �˵�ָ��.
	*/
	virtual DuiMenu* getMenu(const QString& menuName, const Args &param) = 0;

	/**
	* @brief ��ȡƵ��һЩ���е���������ʽ���ڣ������߼�������ҵ��������ʾ.���ղء��˵�����ģ��˵��ĵ���ʽ���ڣ�.
	* @param subWindow���Ӵ�������
				AudioSetting	(��˷����ã�
				FavorWindow		(Ƶ���ղش���)
				KalaokSetting	(�������ã�
				RecordFinish	(¼����ɴ���)
				ChangeTemplate  (�л�ģ�洰��)(���ܷ���NULL[�ڲ���ȡ����ģ���б�ʱ])
			param,	���� 
				����
				subWin::Args args;
				args.insert(subWin::KRecordPath, path);
				
	* @return ����ָ��.
	*/
	virtual DuiFrameWindow* getSubWindow(subWin::SubWindow subWindow, const subWin::Args &param) = 0;

	/**
	* @brief ����Ƶ��һЩ���е���������ʽ����,�����߼�����.
	* @param popWin, ��Ҫ������Ƶ���Ӵ�������
				ResetPlayer		(���ò���������)
				ChannelInfo		(Ƶ����Ϣ����)
				VoiceWizard		(����Ԥ��)
			param,	���� 
				����
				popWin::Args param;
				param.insert(popWin::KSid, channelId);
				param.insert(popWin::KUid, userId);
	* @return void.
	*/

	virtual void popSubWindow(popWin::PopWindow popWindow, const popWin::Args &param) = 0;
	/**
	* @brief �����Ƿ�Ĭ��UI.
	* @param defaultChannel��true,Ĭ��UI�� flase����ʾȫ����UI��ͨ��fullCustomContainer()��ȡȫ����UI����
	*/
	virtual void showDefaultChannel(bool defaultChannel) = 0;

	/**
	* @brief ��ȡȫ����UI����.
	* @return ȫ����UI����.
	*/
	virtual DuiWidget* fullCustomContainer() = 0;
	    
	/**
	* @brief ��ȡƵ���ض��ؼ��ӿ�.
	* @param �ɻ�ȡ�ؼ���ö��ֵ
	* @return ��Ӧ�ؼ���ָ��.
	*/
    enum AvailableWidgetName
    {
        YYGameButton = 0,
		DefaultPublicMessageEdit = 1,
		DefaultPrivateMessageEdit = 2,
    };
	virtual DuiWidget* getWidget(AvailableWidgetName name) = 0;

	/**
	* @brief ����˽���Զ��ظ�����.
	* @param ���ָ��ڵ�.
	* @return �Զ��ظ����ڵ�ָ��.
	*/
	virtual IChannelPrivateChatAutoReplyPtr createPrviateChatAutoReplyWindow(DuiWidget* parent) = 0;

	/**
	* @brief ������ݻظ�����.
	* @param ���ָ��ڵ�.
	* @return �Զ��ظ����ڵ�ָ��.
	*/
	virtual IChannelChatQuickReplyPtr createChatQuickReplyWindow(DuiWidget* parent) = 0;

	/**
	* @brief ����Ƶ���������洰��.
	* @param ���ָ��ڵ�.
	* @return Ƶ���������洰�ڵ�ָ��.
	*/
	virtual DuiWidget* createChannelBaseBulletinWidget(DuiWidget* parent) = 0;

	/**
	* @brief ע�����������
	* @param priority ���ȼ�
	* @param func ע��ĺ���������ֵ�μ�namespace decoratorMethod
	* @param decorator ������ָ�룬Ҫ����������invoke�������Ե��õ���ע���func������
	* @return �Ƿ�ע��ɹ�
	*/
	virtual bool registerDrawDecorator(const qint32 priority, const Decorator::Method method, IDWQComponentPtr decorator) = 0;

	/**
	* @brief ��ע�����������
	* @param func ��ע��ĺ���������ֵ�μ�namespace decoratorMethod
	* @param decorator ������ָ��
	* @return �Ƿ�ע��ɹ�
	*/
	virtual bool unregisterDrawDecorator(const Decorator::Method method, IDWQComponentPtr decorator) = 0;

	/**
	* @brief ��������������
	* @param func ����������������ֵ�μ�namespace decoratorMethod
	* @param args �����б�˳����ݺ���ȷ��
	* @return �Ƿ���óɹ�
	*/
	virtual bool invokeDecorator(const Decorator::Method method, QVariantList &args) = 0;

	/*
	* @brief channelBottomBar λ�ò���ҵ��widget������λ���� expand��ť(�ر���̨)��࣬����ģ�岻��ʾexpand��ť
	* @param widget Ҫ�����widget
	* @param index ����λ��
	* @return 0=�ɹ�����0=ʧ��
	*/
	virtual int insertBussWidgetToChannelBtmBar(DuiWidget* widget, int index = -1) = 0;

	/*
	* @brief channelBottomBar λ�ò���ҵ��widget�Ƴ�����
	* @param widget �Ѳ����widget
	* @return 0=�ɹ�����0=ʧ��
	*/
	virtual int removeBussWidgetFromChannelBtmBar(DuiWidget* widget) = 0;

	/*
	*@brief PUBG+�������߿�����ã�Ƶ���������ʱԤ���Ҳ�λ����ʾ��Ƶ�����ڼ���󻯣�
	*@param width �������ߴ��ڿ��
	*/
	virtual void setPubgWidth(int width) = 0;

	/*
	*@brief ��ȡƵ���û��ٱ�����
	*@param uid ���ٱ��û�uid
	*/
	virtual DuiFrameWindow* getInfoFrame(quint32 uid) = 0;
};
DWBIND_INTERFACE(popWin::Args, DWMAKE_INTERFACE_UUID_NAME(QMap<QString_QVariant>))
