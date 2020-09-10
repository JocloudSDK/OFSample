#pragma once
#include "dwglobal.h"
#include "duifw/duiwidget.h"
class DuiToolTipStyle2Private;

class DW_DUIFW_EXPORT DuiToolTipStyle2
{
	Q_DECLARE_PRIVATE(DuiToolTipStyle2)
public:
	//用于决定view的显示位置.
	enum ReferenceType
	{
		ReferenceWidget ,	//在widget的四周显示
		ReferenceMouse		//在mouse的四周显示
	};
	
	enum ShowStyle
	{
		NormalShow,
		AnimationShow
	};
public:
	DuiToolTipStyle2();
	DuiToolTipStyle2( const DuiToolTipStyle2& style);
	DuiToolTipStyle2& operator = ( const DuiToolTipStyle2& style );

	~DuiToolTipStyle2();

	void setBackground( const QBrush& backGroundBrush );
	void setBorder( const QPen& border );
	void setArrowType( Qt::ArrowType type );
	void setTriangleHeight( quint32 value );
	void setTriangleOffset( quint32 offset );			// 设置三角偏移。优先级比setTriangleOffsetPercent高.
	void setTriangleOffsetPercent( quint32 percentage );	// 设置三角偏移的百分比.
	void setRoundedRect( quint32 round );
	void setReferenceType( ReferenceType type );
	void setReferenceSpacing( int spacing );	// view和参照物的间距.
	void setReferenceOffset( int offset );
	void setReferenceOffsetPercent( int percent ); //负数向负方向偏移.
	void setShowStyle(ShowStyle showStyle );

	QPen	border() const;
	quint32 roundedRect() const;
	QBrush	background() const;
	quint32 triangleHeight() const;
	quint32 triangleOffset() const;
	quint32	triangleOffsetPercent() const;
	int 	referenceSpacing() const;
	int		referenceOffset() const;
	int		referencePercent() const;
	Qt::ArrowType	arrowType() const;
	ReferenceType	referenceType() const;
	ShowStyle		showStyle() const;

private:
	DuiToolTipStyle2Private*	d_ptr;
};