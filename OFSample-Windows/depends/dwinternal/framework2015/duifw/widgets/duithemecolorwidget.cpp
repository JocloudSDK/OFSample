#include "stable.h"

#include "duitheme.h"
#include "duithemecolorwidget.h"

const DuiTheme::ColorType KDefaultColorType = DuiTheme::MoreLightColor;

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF( DuiThemeColorWidget )

DuiThemeColorWidget::DuiThemeColorWidget( QGraphicsItem *parent )
: DuiWidget( parent )
{
	setThemeColorType( KDefaultColorType );
	setBackgroundColor( DuiTheme::getColor( KDefaultColorType ) );
}

DuiThemeColorWidget::~DuiThemeColorWidget( void )
{

}

void DuiThemeColorWidget::colorize( QRgb color )
{
	Q_UNUSED( color );
	DuiTheme::ColorType type = ( DuiTheme::ColorType )themeColorType(); 
	setBackgroundColor( DuiTheme::getColor( type ) );
}
