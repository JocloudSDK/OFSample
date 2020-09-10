#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class QString;
class DuiWidget;
class QWidget;
class DuiFrameWindow;
class DuiMenu;
namespace Qt
{
	enum ConnectionType;
}
DWDEFINE_INTERFACE(IYYUICreator): public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool createWidget(const QString &fileName, const QString &rootPath, DuiWidget *parent) = 0;
	virtual DuiFrameWindow* createFrameWindow(const QString& fileName, const QString &rootPath, QWidget *parent) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual DuiMenu	*createMenu(const QString& xml, const QString& rootPath, QObject* actionSlotReceiver, Qt::ConnectionType connectionType) = 0;
	

	/**
	  * @brief 从XML字符串中创建DuiWidget
	  * @param xmlString xml字符串
	  * @param parent 父Widget指针
	  * @return true:succeed
	  */
    virtual bool createWidgetFromMemory(const QString &xmlString, DuiWidget *parent) = 0;

	/**
	* @brief 从XML字符串中创建DuiFrameWindow
	* @param xmlString xml字符串
	* @param parent 父窗口指针
	* @return 非空为成功
	*/
	virtual DuiFrameWindow* createFrameWindowFromMemory(const QString &xmlString, QWidget *parent) = 0;
};