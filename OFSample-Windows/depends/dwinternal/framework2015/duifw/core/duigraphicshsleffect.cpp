#include "stable.h"
#include "duigraphicshsleffect.h"

void RGB_to_HSL(int r,int g,int b, quint8 * hr, quint8 * sr,quint8* lr)
{
	double rd,gd,bd,h,s,l,max,min;

	// convert RGB to HSV
	rd = r / 255.0;            // rd,gd,bd range 0-1 instead of 0-255
	gd = g / 255.0;
	bd = b / 255.0;

	// compute maximum of rd,gd,bd
	if (rd>=gd)
	{
		if (rd>=bd)
			max = rd;
		else
			max = bd;
	}
	else
	{
		if (gd>=bd)
			max = gd;
		else
			max = bd;
	}

	// compute minimum of rd,gd,bd
	if (rd<=gd)
	{
		if (rd<=bd)
			min = rd;
		else
			min = bd;
	}
	else
	{
		if (gd<=bd)
			min = gd;
		else
			min = bd;
	}

	l = (max + min) / 2.0;

	if(max==min)
		s = h = 0;
	else
	{
		if (l<=0.5)
			s = (max - min) / (max + min);
		else
			s = (max - min) / (2 - (max + min));

		if (max == rd)
			h = 42.5 * (gd-bd)/(max-min);
		else if (max == gd)
			h = 42.5 * (bd-rd)/(max-min)+85;
		else
			h = 42.5 * (rd-gd)/(max-min)+170;
		if (h<0) h+=255;
	}

	*hr = h;
	*lr = (l*255.0);
	*sr = (s*255.0);
}

/// Convert HSL back to RGB
/// Input and output are all in range 0..255
void HSL_to_RGB(quint8 h,quint8 s,quint8 l, quint8 * r, quint8 * g, quint8 * b)
{
	float rf = 0 ,gf = 0,bf = 0;
	float hf,lf,sf;
	float p,q;

	if(s == 0)
	{
		*r = *g = *b = l;
		return;
	}

	hf = h / 255.0;
	lf = l / 255.0;
	sf = s / 255.0;

	if (lf<=0.5)
		q = lf*(1+sf);
	else
		q = lf+sf-lf*sf;
	p = 2*lf-q;

	rf = hf + (1 / 3.0);
	gf = hf;
	bf = hf - (1 / 3.0);

	if (rf < 0) rf+=1;
	if (rf > 1) rf-=1;
	if (gf < 0) gf+=1;
	if (gf > 1) gf-=1;
	if (bf < 0) bf+=1;
	if (bf > 1) bf-=1;

	if (rf < 1/6.0)
		rf = p + ((q-p)*6*rf);
	else if(rf < 0.5)
		rf = q;
	else if(rf < 2/3.0)
		rf = p + ((q-p)*6*(2/3.0-rf));
	else
		rf = p;

	if (gf < 1/6.0)
		gf = p + ((q-p)*6*gf);
	else if(gf < 0.5)
		gf = q;
	else if(gf < 2/3.0)
		gf = p + ((q-p)*6*(2/3.0-gf));
	else
		gf = p;

	if (bf < 1/6.0)
		bf = p + ((q-p)*6*bf);
	else if(bf < 0.5)
		bf = q;
	else if(bf < 2/3.0)
		bf = p + ((q-p)*6*(2/3.0-bf));
	else
		bf = p;

	*r = rf * (255);
	*g = gf * (255);
	*b = bf * (255);
}

DuiGraphicsHslEffect::DuiGraphicsHslEffect( QObject *parent ) : QGraphicsEffect(parent)
{

}

void DuiGraphicsHslEffect::draw( QPainter * painter )
{
	QPoint offset;
	QPixmap pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset);

	QImage image = pixmap.toImage();

	int imageWidth = image.width();
	int imageHeight = image.height();

	int newHue = m_hslColor.hslHue();
	int newSaturation = m_hslColor.hslSaturation();

	quint8 r = 0, g = 0, b = 0;
	for(int y = 0; y < imageHeight; y++)
	{
		quint32 *scanLine = (quint32*)image.scanLine(y);
		for(int x = 0; x < imageWidth; x++)
		{
			quint32 pixel = *(scanLine + x);
			HSL_to_RGB(newHue, newSaturation, qGray(pixel), &r, &g, &b);
			*(scanLine + x) = qRgba(r, g, b, qAlpha(pixel));
		}
	}

	painter->drawImage(offset, image);
}

void DuiGraphicsHslEffect::setColor( const QColor &hslColor )
{
	m_hslColor = hslColor;
}
