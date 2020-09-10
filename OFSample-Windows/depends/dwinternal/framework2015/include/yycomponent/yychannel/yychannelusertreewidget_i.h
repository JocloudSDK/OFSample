#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yychannel/yyproperty.h"

class DuiWidget;
class DuiMenu;
struct ITreeIndicatorHandler;

DWDEFINE_INTERFACE(IChannelUserTreeWidget) : public IDWQComponent 
{
	
	enum FoldState
	{
		Expand // READ:展开的 WRITE:展开
		,Collapse // READ:收起的 WRITE:收起
	};

	typedef yyproperty::property<FoldState> FoldStatePro;

	/**
	* @brief 获取对应widget.
	* @return widget.
	*/
	virtual DuiWidget*	widget() = 0;

	/**
	* @brief 注册频道树尾灯.
	* @param handle,尾灯处理器.
	* @return 注册成功返回true，否则false.
	*/
	virtual bool registerIndicatorHandler( ITreeIndicatorHandler * handler ) = 0;


	/**
	* @brief 反注册频道树尾灯.
	* @param handle,尾灯处理器.
	* @return 反注册成功返回true，否则false.
	*/
	virtual bool unRegisterIndicatorHandler( ITreeIndicatorHandler *handler) = 0;


	/**
	* @brief 频道树是否处于展开状态.
	* @return 频道树处于展开状态，返回true，否则返回false.
	*/
	virtual bool isExpanded() = 0; /* # obsolete , 用 pro_foldState() == Expand 代替*/

	/**
	* @brief 展开频道树.
	*/
	virtual void expandTree() = 0; /* # obsolete , 用 pro_foldState() = Expand 代替*/

	/**
	* @brief 收起频道树.
	*/
	virtual void collapseTree() = 0; /* # obsolete , 用 pro_foldState() = Collapse 代替 */


	/**
	* @brief 在频道树定位某个用户，让其处于可见位置.
	* @param uid，用户uid.
	* @return 成功返回0，无效id返回-1.
	*/
	virtual int locateUser(quint32 uid) = 0;

	/**
	* @brief 在频道树定位某个子频道，让其处于可见位置.
	* @param sid，子频道id.
	* @return 成功返回0，无效id返回-1.
	*/
	virtual int locateChannel(quint32 sid) = 0;

	/**
	* @brief 获取子频道界面显示的人数，如果是一级子频道，则不包含二级子频道人数.
	* @param sid，子频道id.
	* @param recursive，人数是否包含二级子频道人数.
	* @return 子频道人数.
	*/
	virtual int subChanneUserCount( quint32 sid, bool recursive) = 0;

	/**
   * @brief 频道树展开状态, READ/WRITE.
   * @return FoldState.
   */
	virtual FoldStatePro& pro_foldState() = 0;

	typedef struct SChannelOnlineUserCount
	{
		quint32 userOnlineCount;//当前频道不包含二级子频道的人数.
		quint32 totalUserOnlineCount;//包含二级子频道的总人数.
	}TChannelOnlineUserCount;
	/**
   * @brief 提供给模板业务使用的设置频道在线人数接口,接口调一次会触发一次频道树刷新逻辑，这里至少每隔3秒设置一次.
   * @param componentId，传入业务方组件的id.
   * @param onlineUserDataList，key为子频道id,value为对应的在线人数，设置空列表取消这个设置.
   * @return bool.
   */
	virtual bool setAppChannelOnlineUserCount(quint32 componentId,const QMap<quint32,TChannelOnlineUserCount>& onlineUserDataList) = 0;

	/**
	* @brief 提供给模板业务使用,用来获取右键点击指定区域用户昵称后要显示的菜单.
	* @param uid，传入业务方被点击用户的uid.
	*/

	virtual DuiMenu* getMenu(quint32 uid) = 0;
};
