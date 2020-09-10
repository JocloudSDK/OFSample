
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QMargins.h>

#include "dwglobal.h"
#include "duiwidgetbase.h"
#include "duifw/duixmlui.h"

//------------------------------------------------------------------------------
class QGraphicsLayout;
class DuiWidgetPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiWidget : public DuiWidgetBase
{
public:
	enum DragWidgetFlag
	{
		DragWindow = 1,
		NotDragWindow = 2,
		DragWithParentFlag = 3
	};

	enum ColorizeWidgetFlag
	{
		FollowParent = 0,
		UnColorableFlag,
		ColorableFlag,
	};

	enum LayoutType
	{
		None = 0,
		VBox = 1,
		HBox = 2,
		Anchor = 4
	};

	enum AnchorPoint
	{
		Left = 0,
		HCenter,
		Right,
		Top,
		VCenter,
		Bottom
	};

	enum AccEvent
	{
		AccFocus = 0,
		AccSelection,
		AccStateChanged,
		AccLocationChanged
	};

	Q_OBJECT
	Q_DISABLE_COPY(DuiWidget)
	Q_DECLARE_PRIVATE(DuiWidget)
	Q_ENUMS(DragWidgetFlag)
	Q_ENUMS(ColorizeWidgetFlag)
	Q_ENUMS(LayoutType)
	Q_ENUMS(AnchorPoint)
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiWidget)

	Q_PROPERTY(int height READ height WRITE setHeight)
	Q_PROPERTY(int width READ width WRITE setWidth)
	Q_PROPERTY(int spacing READ spacing WRITE setSpacing)	//设置本widget的layout的spacing
	Q_PROPERTY(int itemSpacing READ itemSpacing WRITE setItemSpacing)	//设置本widget在父亲widget layout中的spacing
	Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
	Q_PROPERTY(QSizeF fixSize READ fixSize WRITE setFixSize)
	Q_PROPERTY(int fixHeight READ fixHeight WRITE setFixHeight)
	Q_PROPERTY(int fixWidth READ fixWidth WRITE setFixWidth)
	Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
	Q_PROPERTY(qreal minWidth READ minimumWidth WRITE setMinimumWidth)
	Q_PROPERTY(qreal maxWidth READ maximumWidth WRITE setMaximumWidth)
	Q_PROPERTY(qreal preferredWidth READ preferredWidth WRITE setPreferredWidth)
	Q_PROPERTY(qreal minHeight READ minimumHeight WRITE setMinimumHeight)
	Q_PROPERTY(qreal maxHeight READ maximumHeight WRITE setMaximumHeight)
	Q_PROPERTY(qreal preferredHeight READ preferredHeight WRITE setPreferredHeight)
	Q_PROPERTY(QSizeF minSize READ minimumSize WRITE setMinimumSize)
	Q_PROPERTY(QSizeF maxSize READ maximumSize WRITE setMaximumSize)
	Q_PROPERTY(QSizeF preferredSize READ preferredSize WRITE setPreferredSize)
	Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_PROPERTY(I18nString toolTip READ toolTip WRITE setToolTip)
	Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
	Q_PROPERTY(DragWidgetFlag dragPolicy READ dragPolicy WRITE setDragPolicy )
	Q_PROPERTY(ColorizeWidgetFlag colorizePolicy READ colorizePolicy WRITE setColorizePolicy)
	Q_PROPERTY(QString xmlPath READ xmlPath WRITE setXmlPath SCRIPTABLE false)
	Q_PROPERTY( int themeColorType READ themeColorType WRITE setThemeColorType )
	Q_PROPERTY(LayoutType layout READ layoutType WRITE setLayout)
	Q_PROPERTY(AnchorInfo anchor READ anchor WRITE setAnchor)
    Q_PROPERTY(AnchorInfo2 anchor2 READ anchor2 WRITE setAnchor2)
	Q_PROPERTY(qreal x READ x WRITE setX)
	Q_PROPERTY(qreal y READ y WRITE setY)
	Q_PROPERTY(QSizePolicy::Policy splitPolicy READ splitPolicy WRITE setSplitPolicy)
    Q_PROPERTY(QString styleId READ styleId WRITE setStyleId)
	Q_PROPERTY(QString uuid READ uuid WRITE setUuid)
    Q_PROPERTY(QString anchorString READ anchorString WRITE setAnchorString)
	Q_PROPERTY(bool autoWidth READ autoWidth WRITE setAutoWidth )
	Q_PROPERTY(bool autoHeight READ autoHeight WRITE setAutoHeight)
	Q_PROPERTY(I18nString accessibleName READ accessibleName WRITE setAccessibleName)
	Q_PROPERTY(I18nString accessibleDescription READ accessibleDescription WRITE setAccessibleDescription)
	Q_PROPERTY(I18nString whatsThis READ whatsThis WRITE setWhatsThis)

public:
	explicit DuiWidget(QGraphicsItem *parent = NULL);
	virtual ~DuiWidget();

	int height() const;
	void setHeight(int h);

	int width() const;
	void setWidth(int w);

	virtual void setSpacing(int s);
	void setItemSpacing(int s);
	virtual void setAlignment(Qt::Alignment align);
	void setFixSize(const QSizeF &s);
	void setFixHeight(int h);
	void setFixWidth(int w);

	virtual QMargins margins() const;
	virtual void setMargins(const QMargins& m);

	QColor backgroundColor() const;
	virtual void setBackgroundColor( QColor backgroundColor );

	void setDragPolicy( DragWidgetFlag flag );
	DragWidgetFlag dragPolicy() const;

	virtual void setColorizePolicy(ColorizeWidgetFlag flag);
	ColorizeWidgetFlag colorizePolicy() const;

	int themeColorType() const;
	virtual void setThemeColorType( int );

	LayoutType layoutType() const;
	void setLayout(LayoutType type);

	virtual DragWidgetFlag hitTest(const QPointF& point) const;

	AnchorInfo anchor() const;
	void setAnchor(const AnchorInfo &data);

    AnchorInfo2 anchor2() const;
    void setAnchor2(const AnchorInfo2 &data, bool isDynamic = false);

	QSizePolicy::Policy splitPolicy() const;
	void setSplitPolicy(QSizePolicy::Policy policy);

    QString styleId()const;
    void setStyleId(const QString& styleSheet);

	QString uuid() const;
	void setUuid(const QString &uuid);

    QString anchorString() const;
    void setAnchorString(const QString &anchorString);

	bool autoWidth() const;
	void setAutoWidth(bool isAutoWidth);
	
	bool autoHeight() const;
	void setAutoHeight(bool isAutoHeight);

	/** load file to create child OR config information */
	bool createFromFile(const QString &fileName);

	QWidget *topQWidget() const;
	QGraphicsView *graphicsView() const;

	QString accessibleName() const;
	void setAccessibleName(const QString &name);
	QString	accessibleDescription() const;
	void setAccessibleDescription(const QString & description);
	void setWhatsThis(const QString &);
	QString whatsThis() const;
	
    // auto set widget to zero size when hide, recover previous size wen show.
    void setVisible2(bool visible);

	QPoint mapToGlobal(const QPoint & pos) const;

public:
	//used for dynamic create
	Q_INVOKABLE virtual void addItem(DuiWidget* item);
	Q_INVOKABLE virtual void removeItem(DuiWidget *item);
    Q_INVOKABLE virtual int  insertItem(DuiWidget* item,int index = -1);

	//处理ScollArea的特殊情况，所以把setLayout变成虚方法
	virtual void setLayout(QGraphicsLayout *layout);
	virtual QGraphicsLayout *layout () const;
	virtual QString toolTip() const;
	virtual void setToolTip( const QString& toolTip );
	virtual void colorize(QRgb color);

protected:
	friend class DuiXmlUI;

	/// used for subclass constructor
	explicit DuiWidget( DuiWidgetPrivate &dd, QGraphicsItem *parent /* = NULL*/ );
	virtual void construct();
	virtual DuiWidget *getXmlNode();
	virtual void doColorize( QRgb color );
	virtual void focusInEvent(QFocusEvent *event);

	static void duiUpdateAccessibility(DuiWidget *duiWidget, AccEvent reason);
	void doConstruct();
	void setDisableStatus();

private:
	int spacing() const;
	int itemSpacing() const;
	Qt::Alignment alignment() const;
	QSizeF fixSize() const;
	int fixWidth() const;
	int fixHeight() const;

	QString xmlPath() const;
	void setXmlPath(const QString& path);
    void delaySetAnchor();
    bool canSetAnchor();
};