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
        SP_WidgetSceneLeftAligned = 0x2,    // 优先于scene的左边显示
        SP_GeometryLeftAligned = 0x3,
        SP_LeftAlignedMask = 0xf,
        
        SP_WidgetRightAligned = 0x10,
        SP_WidgetSceneRightAligned = 0x20,   // 优先于scene的右边显示
        SP_GeometryRightAligned = 0x30,
        SP_RightAlignedMask = 0xf0,

        SP_WidgetBottomAligned = 0x100,
        SP_BottomAlignedMask = 0xf00,

        SP_WidgetTopAligned = 0x1000,
        SP_TopAlignedMask = 0xf000,

        SP_ScreenRectAdjust = 0x10000,          // 适应屏幕
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
 * @brief 每个业务关注的是registerIndecator和unregisterIndecator接口，
 * 以及组件抛出的 iconClicked( const QString& handleName, quint32 uid ) 信号，用于处理业务图标点击后的事件.
 */
DWDEFINE_INTERFACE(IYYMiniCard) : public IDWQComponent
{
	/**
	* @brief 注册Indecator接口.
	* @param handler Indecator接口.
	* @return.
	*/
	virtual void registerIndecator(IYYMiniCardIndicatorHandle *handler) = 0;

	/**
	* @brief 注销Indecator接口.
	* @param handler Indecator接口.
	* @return.
	*/
	virtual void unregisterIndecator(IYYMiniCardIndicatorHandle *handler) = 0;
	/**
	* @brief . 业务数据异步到达通过该方法来更新业务icon
	* @param . 
	* @return.
	*/
	virtual void updateIconList() = 0;

	/**
	* @brief . 预加载服务器数据到本地缓存，只对好友的做了这个操作
	* @param . uids 用户uid列表
	* @return.
	*/
	virtual void preFetchData(QVector<quint32> uids) = 0;

	/**
	* @brief . 不对外提供使用
	* @param .
	* @return.
	*/
	virtual void showCard( DuiWidget* item, const QRect& rcItem, YYImTreeItemData* itemData, IPassportPtr spPassport ) = 0;
	/**
	* @brief . 不对外提供使用
	* @param .
	* @return .
	*/
	virtual void showCard( const QPoint& pt, YYImTreeItemData* itemData, IPassportPtr spPassport ) = 0;

	/**
	* @brief . 不对外提供使用
	* @param .
	* @return .
	*/
	virtual void closeCard( ) = 0;

	/**
	* @brief . 不对外提供使用
	* @param .
	* @return .
	*/
	virtual void showCard2(const QMap<QString, QVariant>& args, IPassportPtr spPassport) = 0;

	/**
	* @brief 显示对方游戏与频道信息，格式例如：&是分隔符.activityStr://sid=400000&asid=2080&childsid=456&aid=1234&name=十一国庆七天乐 精彩乐翻天&imageurl=iowqruiowqur&.
	* @param gameName 所玩游戏名字.
	* @param channelName 所在频道名字.
	* @param channelID 所在频道id.
	* @param activityStr 飞机票参数，弹出做活动频道窗口.
	*/
	virtual void setStatus(const QString& gameName, const QString& channelName, const quint32 channelID, const QString& activityStr) = 0;

	/**
	* @brief 显示迷你资料卡.
	* @param item 触发迷你资料卡显示的widget.
	* @param rcItem 显示资料卡的位置.
	* @param uid 用户uid.
	*/
	virtual void showCard3( DuiWidget* item, const QRect& rcItem, quint32 uid ) = 0;

	/**
	* @brief 显示迷你资料卡.
	* @param pt 显示位置.
	* @param uid 用户uid.
	*/
	virtual void showCard4( const QPoint& pt, quint32 uid ) = 0;

	virtual void registerExpandHandle(IYYMiniCardExpandHandlePtr handler) = 0;
	virtual void unRegisterExpandHandle(IYYMiniCardExpandHandlePtr handler) = 0;
	virtual void updateExpandArea() = 0;

	/**
	* @brief 频道树显示迷你资料卡.
	* @param pt 鼠标位置.
	* @param uid 用户uid.
	* @param widget 触发显示迷你资料卡的widget.
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
	* @brief 设置大资料卡皮肤.
	* @param uid 用户uid.
	* @param imagePath 图片本地路径
	* @return.
	*/
	virtual void setInfocardImage(quint32 uid, const QString &imagePath) = 0;
};
