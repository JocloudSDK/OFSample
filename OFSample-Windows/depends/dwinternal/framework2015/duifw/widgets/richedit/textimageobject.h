#pragma once
#include <QTextObjectInterface>

class RichTextEdit;
class  DuiTextImageObject : public QObject, public QTextObjectInterface
{
	Q_OBJECT
	Q_INTERFACES(QTextObjectInterface)
public:
	DuiTextImageObject( RichTextEdit* textedit );

	bool	isZoomHovered() const;
	void	setZoomHovered(bool hovered );

	bool	hasZoomIcon() const;
	void	setZoomIcon( bool hasZoomIcon );
	
	void	setHoveredPos(int pos);
	QRectF  objectRect(int posInDoc) const;

private:
	virtual QSizeF intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format);
	virtual void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format);

private:
	RichTextEdit*	m_textedit;
	QPixmap		m_zoomIconNormal;
	QPixmap		m_zoomIconHover;
	bool		m_isHovered;
	bool		m_hasZoomIcon;
	int			m_hoveredPos;
	QHash<int, QRectF> m_objectHash;
};