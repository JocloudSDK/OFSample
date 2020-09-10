#pragma once

#include <vector>
#include "biz/biz_types.h"
#include "dwbase/dwcomex.h"
#include "yycomponent/yycommon/yyminicardindicatorhandle_i.h"
#include "yycomponent/yycommon/yyminicardexpandhandle_i.h"
#include "yycomponent/yycommon/yyminicardheaderareahandle_i.h"

namespace minicard
{
	enum From 
	{  
		From_Null,
		From_MainFriendList,
		From_ImChatHeader,
		From_GroupMemberList,
		From_MainFrameHead,
		From_ChannelUserTree,
	};

    enum ShowPolicy
    {
        SP_None = 0,

        SP_WidgetLeftAligned = 0x1,
        SP_WidgetSceneLeftAligned = 0x2,    // ������scene�������ʾ
        SP_GeometryLeftAligned = 0x3,
        SP_LeftAlignedMask = 0xf,
        
        SP_WidgetRightAligned = 0x10,
        SP_WidgetSceneRightAligned = 0x20,   // ������scene���ұ���ʾ
        SP_GeometryRightAligned = 0x30,
        SP_RightAlignedMask = 0xf0,

        SP_WidgetBottomAligned = 0x100,
        SP_BottomAlignedMask = 0xf00,

        SP_WidgetTopAligned = 0x1000,
        SP_TopAlignedMask = 0xf000,

        SP_ScreenRectAdjust = 0x10000,          // ��Ӧ��Ļ
    };

	static const char* KAttachWidget = "AttachWidget";     // DuiWidget*
	static const char* KWidgetGeometry = "WidgetGeometry"; // geometry rect of attach widget
	static const char* KItemData = "ItemData";     // YYImTreeItemData*
	static const char* KWidgetPos = "WidgetPos";   // QPoint
	static const char* KFrom = "From"; // From
    static const char* KShowPolicy = "ShowPolicy"; // ShowPolicy
    static const char* KShowDelayTimeMsec = "ShowDelayMsec";        // unsigned int, default is 300ms
    static const char* KAttachGraphicsView = "AttachGraphicsView";  // QGraphicsView*, set the value if can't not get graphics view from KAttachWidget
    static const char* KItemDataUid = "ItemDataUid";        // unsigned int
    static const char* KAttachHwnd = "AttachHwnd"; // window from another process
    static const char* KInfoPropbag = "InfoPropbag";    // expand, IPropBagWrapperPtr

    // KInfoPropbag
    static const wchar_t KUinfoLevel[] = L"dwUinfoLevel";
    static const wchar_t KUinfoJifen[] = L"dwUinfoJifen";
    static const wchar_t KRole[] = L"dwRole";
    static const wchar_t KIsSettle[] = L"bIsSettle";
    static const wchar_t KIsEnableTextInChannel[] = L"bIsEnableTextInChannel";
    static const wchar_t KIsEnableVoiceInChannel[] = L"bInableVoiceInChannel";
    static const wchar_t KSmemberJifen[] = L"dwSmemberJifen";
	static const wchar_t KIsAnonymousUser[] = L"bIsAnonymousUser";
}

class DuiWidget;
struct YYImTreeItemData;
define_interface(IPassport);

/**
 * @brief ÿ��ҵ���ע����registerIndecator��unregisterIndecator�ӿڣ�
 * �Լ�����׳��� iconClicked( const QString& handleName, quint32 uid ) �źţ����ڴ���ҵ��ͼ��������¼�.
 */
DWDEFINE_INTERFACE(IYYMiniCard) : public IDWQComponent
{
	/**
	* @brief ע��Indecator�ӿ�.
	* @param handler Indecator�ӿ�.
	* @return.
	*/
	virtual void registerIndecator(IYYMiniCardIndicatorHandle *handler) = 0;

	/**
	* @brief ע��Indecator�ӿ�.
	* @param handler Indecator�ӿ�.
	* @return.
	*/
	virtual void unregisterIndecator(IYYMiniCardIndicatorHandle *handler) = 0;
	/**
	* @brief . ҵ�������첽����ͨ���÷���������ҵ��icon
	* @param . 
	* @return.
	*/
	virtual void updateIconList() = 0;

	/**
	* @brief . Ԥ���ط��������ݵ����ػ��棬ֻ�Ժ��ѵ������������
	* @param . uids �û�uid�б�
	* @return.
	*/
	virtual void preFetchData(QVector<quint32> uids) = 0;

	/**
	* @brief . �������ṩʹ��
	* @param .
	* @return.
	*/
	virtual void showCard( DuiWidget* item, const QRect& rcItem, YYImTreeItemData* itemData, IPassportPtr spPassport ) = 0;
	/**
	* @brief . �������ṩʹ��
	* @param .
	* @return .
	*/
	virtual void showCard( const QPoint& pt, YYImTreeItemData* itemData, IPassportPtr spPassport ) = 0;

	/**
	* @brief . �������ṩʹ��
	* @param .
	* @return .
	*/
	virtual void closeCard( ) = 0;

	/**
	* @brief . �������ṩʹ��
	* @param .
	* @return .
	*/
	virtual void showCard2(const QMap<QString, QVariant>& args, IPassportPtr spPassport) = 0;

	/**
	* @brief ��ʾ�Է���Ϸ��Ƶ����Ϣ����ʽ���磺&�Ƿָ���.activityStr://sid=400000&asid=2080&childsid=456&aid=1234&name=ʮһ���������� �����ַ���&imageurl=iowqruiowqur&.
	* @param gameName ������Ϸ����.
	* @param channelName ����Ƶ������.
	* @param channelID ����Ƶ��id.
	* @param activityStr �ɻ�Ʊ�������������Ƶ������.
	*/
	virtual void setStatus(const QString& gameName, const QString& channelName, const quint32 channelID, const QString& activityStr) = 0;

	/**
	* @brief ��ʾ�������Ͽ�.
	* @param item �����������Ͽ���ʾ��widget.
	* @param rcItem ��ʾ���Ͽ���λ��.
	* @param uid �û�uid.
	*/
	virtual void showCard3( DuiWidget* item, const QRect& rcItem, quint32 uid ) = 0;

	/**
	* @brief ��ʾ�������Ͽ�.
	* @param pt ��ʾλ��.
	* @param uid �û�uid.
	*/
	virtual void showCard4( const QPoint& pt, quint32 uid ) = 0;

	virtual void registerExpandHandle(IYYMiniCardExpandHandlePtr handler) = 0;
	virtual void unRegisterExpandHandle(IYYMiniCardExpandHandlePtr handler) = 0;
	virtual void updateExpandArea() = 0;

	/**
	* @brief Ƶ������ʾ�������Ͽ�.
	* @param pt ���λ��.
	* @param uid �û�uid.
	* @param widget ������ʾ�������Ͽ���widget.
	* @return.
	*/
	virtual void showCardFromChannel( const QPoint& pt, quint32 uid, DuiWidget* widget ) = 0;

	virtual void registerHeaderAreaHandle(IYYMiniCardHeaderAreaHandle* handler) = 0;
	virtual void unRegisterHeaderAreaHandle(IYYMiniCardHeaderAreaHandle* handler) = 0;
	virtual void updateHeaderArea() = 0;

	/**
	signals:
	void iconClicked( const QString& handleName, quint32 uid );
	*/

	/**
	* @brief ���ô����Ͽ�Ƥ��.
	* @param uid �û�uid.
	* @param imagePath ͼƬ����·��
	* @return.
	*/
	virtual void setInfocardImage(quint32 uid, const QString &imagePath) = 0;
};
