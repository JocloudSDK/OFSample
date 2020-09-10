#pragma once

#include "dwbase/dwcomex.h"

/**
* @brief IM聊天窗上的tip模型.
  
  tip显示规则:
	同一moduleName()的tip,通过优先级priority( )属性控制显示，只显示最高优先级的一组tip

  model获取及使用方法( 统一由IChatFrame接口生成 )
	IYYImChatFrameManagerPtr frameMgr = comMgr()->getServiceInterface<IYYImChatFrameManager>(KSERVICEImGroup);
	IYYImChatFramePtr charFrame = frameMgr->findImFrame( one.uid );
	IYYImFrameTipInfoModelPtr model = charFrame->appendNoticeTipModel( );
	
	model->setModuleName( L"channel" );
	model->setIconUrl( QString::fromStdString(one.url).toStdWString() );
	model->setText( QString::fromStdString(one.text).toStdWString() );
	model->setObjectName( LSTR(defIM_GameParty) );
	model->setPriority( TIP_PRIORITY_GAMEPARTY ); //由各自的module定义
	model->setNeedCloseButton( true );
	model->setAcceptClick( true );

	model->update();    //务必调此接口刷新界面. 

  model查询:
	IYYImChatFrameManagerPtr frameMgr = comMgr()->getServiceInterface<IYYImChatFrameManager>(KSERVICEImGroup);
	IYYImChatFramePtr charFrame = frameMgr->findImFrame( one.uid );
	IYYImFrameTipInfoModelPtr model = charFrame->getNoticeTipModel( objectname or modulename );
	
	当通过moduleName()查询时， 如果存在属于该模块的tip，则返回当前最高优先级tip组里的第一个tip对象指针
	当通过objectname()查询时， 返回objectname对应的对象指针
	
  注意事项:
	1.设置完数据后,务必调用update接口刷新界面
	2.当modulename为空的时候,不做优先级判断(即priority()字段被无视)，此类tip将全部显示
*/

DWDEFINE_INTERFACE(IYYImFrameTipInfoModel):public IUnk
{
public:

	/**
	* @brief objectname 供查询tipInfoModel对象使用 -- [可选]
	*/
	virtual void setObjectName( const std::wstring& objectname ) = 0;
	
	/**
	* @brief 返回objectname 
	*/
	virtual const std::wstring& objectName( )const = 0;
	
	/**
	* @brief 设置Tip显示的优先级.查看 DEF_PRIORITY
	*/
	virtual void setPriority( int priority ) = 0;

	/**
	* @brief 返回显示的优先级.只显示高优先级
	*/
	virtual int priority( ) = 0;
	
	/**
	* @brief 设置Tip所属的模块.(PS:同一个模块的Tip,只显示最高优先级的,较低优先级的则不给显示,
			一般modelname为组件名, 一个modulename 可以对应多个优先级的tipInfoModel)
	*/
	virtual void setModuleName( const std::wstring& modulename ) = 0;

	/**
	* @brief 返回tip所属的模块
	*/
	virtual const std::wstring& moduleName()const	= 0;
	
	/**
	* @brief 设置显示在tip左侧的图标的url
	*/
	virtual void setIconUrl( const std::wstring& url ) = 0;

	/**
	* @brief 返回显示在tip左侧的图标的url.
	*/
	virtual const std::wstring& iconUrl()const = 0;

	/**
	* @brief 设置Tip上显示的文本.
	*/
	virtual void setText( const std::wstring& text ) = 0;
	
	/**
	* @brief 返回Tip上显示的文本.
	*/
	virtual const std::wstring& text()const	= 0;

	/**
	* @brief 设置是否需要在tip上显示关闭按钮.
	*/
	virtual void setNeedCloseButton( bool need ) = 0;
	
	/**
	* @brief 返回是否需要在tip上显示关闭按钮.
	*/
	virtual bool needCloseButton() = 0;

	/**
	* @brief 设置tip上文本显示的颜色.
	*/
	virtual void setTextColor( const QColor& color ) = 0;
	
	/**
	* @brief 返回tip上文本显示的颜色.
	*/
	virtual const QColor& textColor()const = 0;
	
	/**
	* @brief 设置tip的背景颜色.
	*/
	virtual void setBackgroundColor( const QColor& bkColor, const QColor& topLineColor, const QColor& bottomLineColor ) = 0;
	
	/**
	* @brief 返回tip的背景颜色.
	*/
	virtual void backgroundColor( QColor& bkColor, QColor& topLineColor, QColor& bottomLineColor )const = 0;

	/**
	* @brief 返回tip的背景颜色.
	*/
	virtual void setAcceptClick( bool accept ) = 0;
	
	/**
	* @brief 返回tip的背景颜色.
	*/
	virtual bool acceptClick(  )const = 0;

	/**
	* @brief 设置扩展数据.
	*/
	virtual void setValue( const std::wstring& strValue ) = 0;

	/**
	* @brief 返回扩展数据.
	*/
	virtual const std::wstring& value()const = 0;

	/**
	* @brief 修改完tipinfoModel后，需要执行update以通知到界面或其它关心model内容的部件
	*/
	virtual void update() = 0;

	typedef xsignals::signal<void (IUnkPtr)>  _SIG_TIP_NOTIFY;
	/**
	* @brief model的信息更新后的通知
	*/
	virtual _SIG_TIP_NOTIFY* getUpdateSignal() = 0;

	/**
	* @brief Tip被点击后的信号.
	*/
	virtual _SIG_TIP_NOTIFY* getClickedSignal() = 0;

    /**
    * 所有设置了true会在同一个位置上滚动轮播
    */
    virtual void setRolling(bool rolling) = 0;
    virtual bool isRolling() const = 0;
};