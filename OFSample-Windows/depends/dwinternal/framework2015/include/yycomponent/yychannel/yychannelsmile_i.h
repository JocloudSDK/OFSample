#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycommon/yysmilefacemanager_i.h"

/*
signals:
	// if your don't want to receive smile triggering by other caller, disconnect yourself while smileFrameClosed() is activating.
	void smileFrameClosed();
	void smileSelected( const IYYSmileFaceManager::YYSmileData& );
	void fontChanged(const QFont&);
	void fontColorChanged(const QColor&);
*/

class QString;
class DuiWidget;
class DuiFrameWindow;
DWDEFINE_INTERFACE(IYYChannelSmile) : public IDWQComponent
{
/**
   * @brief .显示表情框
   * @param .refecenceWidget，表情框依据该按钮的位置显示
   * @return .无
   */
	virtual void showFrame(DuiWidget* referenceWidget) = 0;
	//json可以用来设置一些选秀，例如音乐组期望某些东西不显示。
	//{"FontSetting": 1/0", "VipRightPart": 1/0, "VipFacePage":1/0, "GameSmiliesPage":1/0, "VipMallPage":1/0, "VipMallFaces":1/0}
	virtual void setControl(const QString &json) = 0;
	//可以拿到window后设置父子关系
	virtual DuiFrameWindow* window() = 0;

    /**
    * @brief .显示最近使用表情框
    * @param .refecenceWidget，表情框依据该按钮的位置显示
    * @return .无
    */
	virtual void showRecentUsedEmotionFrame(DuiWidget* referenceWidget) = 0;

	/**
	  * @brief 显示表情框
	  * @param refecenceWidget 表情框依据该按钮的位置显示
	  * @param json 控制字符串，作用跟setControl相同
	  */
	virtual void showFrameWithControl(DuiWidget *referenceWidget, const QString &json) = 0;
};
