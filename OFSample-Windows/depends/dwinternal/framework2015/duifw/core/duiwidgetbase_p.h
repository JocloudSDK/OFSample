
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

//------------------------------------------------------------------------------
class DuiWidgetBase;

//------------------------------------------------------------------------------
class DuiWidgetBasePrivate
{
	Q_DISABLE_COPY(DuiWidgetBasePrivate);
	Q_DECLARE_PUBLIC(DuiWidgetBase);
	
public:
	DuiWidgetBasePrivate();
	virtual ~DuiWidgetBasePrivate();

protected:
	DuiWidgetBase *q_ptr;
};
