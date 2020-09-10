#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;
/*
    use getObject function
*/
DWDEFINE_INTERFACE(IYYChannelTab) : public IDWQComponent
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int 		setEnableClose( const QString&  tabName, bool enable ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool 	isEnableClose( const QString& tabName ) const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int 		setTabTooltip( const QString& tabName, const QString& tooltip ) =0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int 		setTabText( const QString& tabName, const QString& text ) =0 ;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int		tabCount() const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int		selectTab( int index ) =0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int		selectTab( const QString& tabName ) =0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString currentTab() const =0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int		currentIndex() const =0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int		indexOfTab( const QString& tabName ) const =0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QString tabName( int index ) const =0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int		removeTab( const QString& tabName ) =0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int		removeTab( int index ) =0;

	//RQ-4110: icon变为复合参数，如果有两个ico，中间用'|'隔开，前面为正常ico，后面为发光ico
	virtual DuiWidget*	addTab( const QString& tabName, const QString& tabText, const QString &icon ) =0;
	virtual DuiWidget*	insertTab(int index, const QString& tabName, const QString& tabText, const QString &icon ) =0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual IUnkPtr			insertIpcHostTab(int index, const QString& tabName, const QString& tabText) = 0;

	virtual DuiWidget*	findTabItem(const QString& tabName) = 0;

	/**
	* @brief 将首页抽出原有的地方，有业务自己摆放.
	* @param containerWidget，用与放首页的容器，该widget必须有scene.
	* @return 成功，0；失败-1.
	*/
	virtual int				extractHomePage(DuiWidget* containerWidget) = 0;

	/**
	* @brief 将首页放回频道默认位置.
	* @param containerWidget，用与放首页的容器，该widget必须有scene.
	*/
	virtual void			restoreHomePage(DuiWidget* containerWidget) = 0;
/*
signals:
    void	selectChanged( const QString& from, const QString& to );
    void	selectChanged( int index );
*/


	typedef _def_boost_signals2_signal_type<void (int nIndex)>::type	_SIG_ACTIVE_PAGE_BY_INDEX;
	virtual _SIG_ACTIVE_PAGE_BY_INDEX* getStartActivePageByIndex() = 0;

	typedef _def_boost_signals2_signal_type<void (DuiWidget* page)>::type	_SIG_ACTIVE_PAGE_BY_WIDGET;
	virtual _SIG_ACTIVE_PAGE_BY_WIDGET* getStartActivePageByWidget() = 0;
};

DWBIND_INTERFACE(IYYChannelTab::_SIG_ACTIVE_PAGE_BY_WIDGET, DWMAKE_INTERFACE_UUID_NAME(_SIG_ACTIVE_PAGE_BY_WIDGET))
DWBIND_INTERFACE(IYYChannelTab::_SIG_ACTIVE_PAGE_BY_INDEX, DWMAKE_INTERFACE_UUID_NAME(_SIG_ACTIVE_PAGE_BY_INDEX))
DWBIND_INTERFACE(IUnkPtr, DWMAKE_INTERFACE_UUID_NAME(IUnkPtr))

