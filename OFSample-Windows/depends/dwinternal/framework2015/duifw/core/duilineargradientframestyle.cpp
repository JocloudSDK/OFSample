#include "stable.h"
#include "dwutility/dwxmldocument.h"
#include "duitheme.h"
#include "duilineargradientframestyle.h"
#include "utility/duicolorformat.h"

DuiLinearGradientObject::DuiLinearGradientObject( )
{
}

DuiLinearGradientObject::~DuiLinearGradientObject( )
{
	if ( m_itemList.count() )
	{
		m_itemList.clear();
	}
}

void DuiLinearGradientObject::Add( int h, int s, int l, qreal pos )
{
	ItemInfo item;
	item.h = h;
	item.s = s;
	item.l = l;
	item.pos = pos;
	m_itemList.append( item );
}

void DuiLinearGradientObject::load( QString filePath )
{
	if ( m_itemList.count() )
	{
		m_itemList.clear();
	}

	QFile file(filePath);
	bool retCode	=	false;
	retCode = file.open( QIODevice::ReadOnly | QIODevice::Text );
	if ( !retCode )
	{
		DW_ASSERT( retCode );
		return ;
	}
	QTextStream inStream(&file);

	DWXmlDocument doc;
	retCode = doc.loadFromString( inStream.readAll().toStdWString() );
	if ( !retCode && doc.rootElement()->childCount() != 1 )
	{
		return;
	}

	const DWXmlElement* element = doc.rootElement()->child( 0 );

	DuiTheme::ColorType type = (DuiTheme::ColorType)QStringFromWCharArray( element->attribute2( L"themeColorType" ) ).toUInt();
	m_color = DuiTheme::getColor( type );

	QString str = QStringFromWCharArray( element->attribute2( L"start" ) );
	QStringList list = QStringFromWCharArray( element->attribute2( L"start" ) ).split( "," );
	if (list.count() != 2 )
	{
		return;
	}
	m_start.setX( list.at( 0 ).toFloat() );
	m_start.setY( list.at( 1 ).toFloat() );


	list = QStringFromWCharArray( element->attribute2( L"finalStop" ) ).split( "," );
	if (list.count() != 2 )
	{
		return;
	}
	m_finalStop.setX( list.at( 0 ).toFloat() );
	m_finalStop.setY( list.at( 1 ).toFloat() );

	for ( int i = 0; i < element->childCount(); i++ )
	{
		const DWXmlElement*item = element->child( i );
		int h = QStringFromWCharArray( item->attribute2( L"h" ) ).toInt();
		int s = QStringFromWCharArray( item->attribute2( L"s" ) ).toInt();
		int l = QStringFromWCharArray( item->attribute2( L"l" ) ).toInt();
		qreal pos = QStringFromWCharArray( item->attribute2( L"pos" ) ).toFloat();
		Add( h, s, l, pos );
	}

	doGradient();
}

void DuiLinearGradientObject::doGradient()
{
	if ( m_itemList.count() == 0 )
	{
		return;
	}

	QLinearGradient g(m_start, m_finalStop );
	g.setCoordinateMode(QGradient::StretchToDeviceMode);
	g.setSpread(QGradient::ReflectSpread);
	
	for ( int i = 0; i < m_itemList.count(); i++ )
	{
		int h = 0, s = 0, l = 0;
		m_color.getHsl(&h, &s, &l);

		ItemInfo item = m_itemList.at( i );
		h += item.h;
		l += item.l;
		s += item.s;
		
		checkValueValid( h, s, l );
		
		QColor colorItem;
		colorItem.setHsl( h, s, l );	
		g.setColorAt( item.pos, colorItem);
	}

	m_brush = QBrush( g );
}

QBrush DuiLinearGradientObject::getBrush() const
{
	return m_brush;
}

void DuiLinearGradientObject::checkValueValid( int &h, int &s, int &l )
{
	h = Dui::clamp(h, 0, 359);
	s = Dui::clamp(s, 0, 255);
	l = Dui::clamp(l, 0, 255);

	return;
}
