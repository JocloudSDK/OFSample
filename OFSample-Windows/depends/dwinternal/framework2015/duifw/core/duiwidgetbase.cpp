#include "stable.h"

#include "duiwidgetbase_p.h"
#include "duiwidgetbase.h"

//------------------------------------------------------------------------------
DuiWidgetBasePrivate::DuiWidgetBasePrivate()
: q_ptr(NULL)
{
}

DuiWidgetBasePrivate::~DuiWidgetBasePrivate()
{
}

//------------------------------------------------------------------------------
DuiWidgetBase::DuiWidgetBase( QGraphicsItem *parent /* = NULL*/ )
	: QGraphicsWidget(parent)
	, d_ptr(new DuiWidgetBasePrivate)
{
	d_ptr->q_ptr = this;
}

DuiWidgetBase::DuiWidgetBase(DuiWidgetBasePrivate &dd, QGraphicsItem *parent /* = NULL*/)
	: QGraphicsWidget(parent) 
	, d_ptr(&dd)
{
	d_ptr->q_ptr = this;
}

DuiWidgetBase::~DuiWidgetBase()
{
	DW_ASSERT_X(d_ptr, __FUNCTION__, "private data is null.");
	DW_SAFE_DELETE(d_ptr);
}
