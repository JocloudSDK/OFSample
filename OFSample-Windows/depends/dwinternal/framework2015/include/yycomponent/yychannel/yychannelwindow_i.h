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
		ChangeTemplate,//可能返回NULL[内部获取不到模版列表时]
        LaunchSelectorWindow,// 开播选择window
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
		KickOff,//T人 args.insert(menuData::KUid, uid);
		Ban//封人 args.insert(menuData::KUid, uid);
	};
}

namespace menuData // 创建menu时所需要的数据.
{
	const char KUid[] = "uid";
	const char KSid[] = "sid";
	const char KUserName[] = "userName";
	const char KItemIndex[] = "itemIndex";
	const char KPoint[] = "point";
    const char KUserRole[] = "userRole";        // 用户退出频道产生的消息的右键菜单需要用到的数据项
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
		* @brief 获得特定用户的昵称颜色时调用此函数
		* @param scene 场景，值为Scenes枚举之一
		* @param uid 用户uid
		* @param color 返回的颜色
		* @param handled 是否已经处理，调用者不需要添加，已处理填写非0，否则为0
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
	* @brief 频道窗口,可以用于窗口最大化，最小化，隐藏，显示，还原，设置背景图，隐藏titlebar等，但慎用于设置窗口大小，直接操作窗口HWND，否则容易导致UI等异常.
	* @return 频道窗口.
	*/
    virtual DuiFrameWindow* channelWindow() = 0;

   /**
   * @brief 创建公屏控件接口。所有权归调用者，请用成员变量等方法保存他，否则引用计数为空时会析构掉.
   * @param parent，布局父亲节点.
   * @return 公屏控件接口指针.
   */
	virtual IChannelMessageEditPtr createMessageWidget(QGraphicsItem* parent) = 0;

   /**
   * @brief 创建输入框接口。所有权归调用者，请用成员变量等方法保存他，否则引用计数为空时会析构掉.
   * @param parent，布局父亲节点.
   * @return 输入框接口指针.
   */
	virtual IChannelInputEditPtr createInputWidget(QGraphicsItem* parent) = 0;

	/**
	* @brief 创建用户树控件接口。所有权归调用者，请用成员变量等方法保存他，否则引用计数为空时会析构掉.
	* @param parent，布局父节点.
	* @return 用户树控件接口指针.
	*/
	virtual IChannelUserTreeWidgetPtr createUserTreeWidget(DuiWidget* parent) = 0;

	/**
	* @brief 创建管理员列表控件接口。所有权归调用者，请用成员变量等方法保存他，否则引用计数为空时会析构掉.
	* @param parent，布局父节点.
	* @return 管理员列表控件接口指针.
	*/
	virtual IChannelManagerWidgetPtr createManagerWidgt(DuiWidget* parent) = 0;


	/**
	* @brief 创建麦序控件接口。所有权归调用者，请用成员变量等方法保存他，否则引用计数为空时会析构掉.
	* @param parent，布局父节点.
	* @return 麦序控件接口指针.
	*/
	virtual IChannelMaixuWidgetPtr createMaixuWidget(DuiWidget* parent) = 0;


	/**
	* @brief 创建频道用户搜索控件接口。所有权归调用者，请用成员变量等方法保存他，否则引用计数为空时会析构掉.
	* @param parent，布局父节点.
	* @return 麦序控件接口指针.
	*/
	virtual IChannelSearchWidgetPtr createSearchWidget(DuiWidget* parent) = 0;


	/**
	* @brief 获取频道现有菜单，里面包含菜单的响应处理，由业务自行显示,现掉现用.

	* @param menuName，菜单名字
			UserContextMenu			:频道树用户item和公屏用户item
			TreeFolderContextMenu	:频道树Folder
			HeaderContextMenu		:频道图标
			TextEditContextMenu		:公屏，私聊
			EditContextMenu			:频道发送框

	* @param QMap<QString, QVariant> &param，param 放入菜单所需要的参数
			于频道有关则		param.insert(menuData::KSid, channelId);
			于用户有关则		param.insert(menuData::KUid, userId);
			于item序列有关则	param.insert(menuData::KItemIndex, itemIndex);
			于char格式有关则	param.insert(menuData::KPoint, event->pos());
	* @return 菜单指针.
	*/
	virtual DuiMenu* getMenu(const QString& menuName, const Args &param) = 0;

	/**
	* @brief 获取频道一些现有的完整弹出式窗口，包括逻辑处理。由业务自行显示.（收藏“菜单”是模拟菜单的弹出式窗口）.
	* @param subWindow，子窗口名字
				AudioSetting	(麦克风设置）
				FavorWindow		(频道收藏窗口)
				KalaokSetting	(伴奏设置）
				RecordFinish	(录音完成窗口)
				ChangeTemplate  (切换模版窗口)(可能返回NULL[内部获取不到模版列表时])
			param,	参数 
				比如
				subWin::Args args;
				args.insert(subWin::KRecordPath, path);
				
	* @return 窗口指针.
	*/
	virtual DuiFrameWindow* getSubWindow(subWin::SubWindow subWindow, const subWin::Args &param) = 0;

	/**
	* @brief 弹出频道一些现有的完整弹出式窗口,包括逻辑处理.
	* @param popWin, 需要弹出的频道子窗口名字
				ResetPlayer		(设置播放器窗口)
				ChannelInfo		(频道信息窗口)
				VoiceWizard		(声音预设)
			param,	参数 
				比如
				popWin::Args param;
				param.insert(popWin::KSid, channelId);
				param.insert(popWin::KUid, userId);
	* @return void.
	*/

	virtual void popSubWindow(popWin::PopWindow popWindow, const popWin::Args &param) = 0;
	/**
	* @brief 设置是否默认UI.
	* @param defaultChannel，true,默认UI， flase，显示全定制UI，通过fullCustomContainer()获取全定制UI容器
	*/
	virtual void showDefaultChannel(bool defaultChannel) = 0;

	/**
	* @brief 获取全定制UI容器.
	* @return 全定制UI容器.
	*/
	virtual DuiWidget* fullCustomContainer() = 0;
	    
	/**
	* @brief 获取频道特定控件接口.
	* @param 可获取控件的枚举值
	* @return 对应控件的指针.
	*/
    enum AvailableWidgetName
    {
        YYGameButton = 0,
		DefaultPublicMessageEdit = 1,
		DefaultPrivateMessageEdit = 2,
    };
	virtual DuiWidget* getWidget(AvailableWidgetName name) = 0;

	/**
	* @brief 创建私聊自动回复窗口.
	* @param 布局父节点.
	* @return 自动回复窗口的指针.
	*/
	virtual IChannelPrivateChatAutoReplyPtr createPrviateChatAutoReplyWindow(DuiWidget* parent) = 0;

	/**
	* @brief 创建快捷回复窗口.
	* @param 布局父节点.
	* @return 自动回复窗口的指针.
	*/
	virtual IChannelChatQuickReplyPtr createChatQuickReplyWindow(DuiWidget* parent) = 0;

	/**
	* @brief 创建频道基础公告窗口.
	* @param 布局父节点.
	* @return 频道基础公告窗口的指针.
	*/
	virtual DuiWidget* createChannelBaseBulletinWidget(DuiWidget* parent) = 0;

	/**
	* @brief 注册绘制修饰器
	* @param priority 优先级
	* @param func 注册的函数，可用值参见namespace decoratorMethod
	* @param decorator 修饰器指针，要求修饰器的invoke函数可以调用到所注册的func函数。
	* @return 是否注册成功
	*/
	virtual bool registerDrawDecorator(const qint32 priority, const Decorator::Method method, IDWQComponentPtr decorator) = 0;

	/**
	* @brief 反注册绘制修饰器
	* @param func 反注册的函数，可用值参见namespace decoratorMethod
	* @param decorator 修饰器指针
	* @return 是否反注册成功
	*/
	virtual bool unregisterDrawDecorator(const Decorator::Method method, IDWQComponentPtr decorator) = 0;

	/**
	* @brief 调用修饰器函数
	* @param func 修饰器函数，可用值参见namespace decoratorMethod
	* @param args 参数列表，顺序根据函数确定
	* @return 是否调用成功
	*/
	virtual bool invokeDecorator(const Decorator::Method method, QVariantList &args) = 0;

	/*
	* @brief channelBottomBar 位置插入业务widget，插入位置是 expand按钮(关闭舞台)左侧，基础模板不显示expand按钮
	* @param widget 要插入的widget
	* @param index 插入位置
	* @return 0=成功，非0=失败
	*/
	virtual int insertBussWidgetToChannelBtmBar(DuiWidget* widget, int index = -1) = 0;

	/*
	* @brief channelBottomBar 位置插入业务widget移除操作
	* @param widget 已插入的widget
	* @return 0=成功，非0=失败
	*/
	virtual int removeBussWidgetFromChannelBtmBar(DuiWidget* widget) = 0;

	/*
	*@brief PUBG+辅助工具宽度设置（频道窗口最大化时预留右侧位置显示，频道窗口假最大化）
	*@param width 辅助工具窗口宽度
	*/
	virtual void setPubgWidth(int width) = 0;

	/*
	*@brief 获取频道用户举报窗口
	*@param uid 被举报用户uid
	*/
	virtual DuiFrameWindow* getInfoFrame(quint32 uid) = 0;
};
DWBIND_INTERFACE(popWin::Args, DWMAKE_INTERFACE_UUID_NAME(QMap<QString_QVariant>))
