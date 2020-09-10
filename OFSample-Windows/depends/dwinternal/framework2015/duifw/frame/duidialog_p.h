
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QObject>
#include <QtimeLine>

#include "duiframewindow_p.h"

//------------------------------------------------------------------------------
class DuiDialog;
class QEventLoop;

//------------------------------------------------------------------------------
class DuiDialogPrivate : public DuiFrameWindowPrivate
{
	Q_DECLARE_PUBLIC(DuiDialog)

public:
	DuiDialogPrivate();
	virtual  ~DuiDialogPrivate();

	int m_result;
	QTimeLine m_effectTimeLine;
	QEventLoop *m_eventLoop;
	bool m_acceptEnter;
	bool m_acceptEscape;
};