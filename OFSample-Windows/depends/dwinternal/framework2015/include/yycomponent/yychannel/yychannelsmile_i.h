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
   * @brief .��ʾ�����
   * @param .refecenceWidget����������ݸð�ť��λ����ʾ
   * @return .��
   */
	virtual void showFrame(DuiWidget* referenceWidget) = 0;
	//json������������һЩѡ�㣬��������������ĳЩ��������ʾ��
	//{"FontSetting": 1/0", "VipRightPart": 1/0, "VipFacePage":1/0, "GameSmiliesPage":1/0, "VipMallPage":1/0, "VipMallFaces":1/0}
	virtual void setControl(const QString &json) = 0;
	//�����õ�window�����ø��ӹ�ϵ
	virtual DuiFrameWindow* window() = 0;

    /**
    * @brief .��ʾ���ʹ�ñ����
    * @param .refecenceWidget����������ݸð�ť��λ����ʾ
    * @return .��
    */
	virtual void showRecentUsedEmotionFrame(DuiWidget* referenceWidget) = 0;

	/**
	  * @brief ��ʾ�����
	  * @param refecenceWidget ��������ݸð�ť��λ����ʾ
	  * @param json �����ַ��������ø�setControl��ͬ
	  */
	virtual void showFrameWithControl(DuiWidget *referenceWidget, const QString &json) = 0;
};
