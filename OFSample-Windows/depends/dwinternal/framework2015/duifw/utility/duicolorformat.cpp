#include "duicolorformat.h"

/** reference to :
* http://en.wikipedia.org/wiki/Lab_color_space#CIE_XYZ_to_CIE_L.2Aa.2Ab.2A_.28CIELAB.29_and_CIELAB_to_CIE_XYZ_conversions 
* http://www.easyrgb.com/index.php?X=MATH
*/
namespace Dui{
// rgb 2 xyz
//----------------------------------------------------------------------------------------
static double rgb2xyz_correct(double cl) {
	if ( cl > 0.04045 ) cl = ::qPow( ( ( cl + 0.055 ) / 1.055 ) , 2.4);
	else  cl = cl / 12.92;

	return cl * 100.0;
}

void rgb2xyz(double* x, double* y, double* z, ushort r, ushort g, ushort b){
	r = clamp<ushort>(r, 0, 255);
	g = clamp<ushort>(g, 0, 255);
	b = clamp<ushort>(b, 0, 255);

	double varr = ( 1.0 * r / 255 );        //R from 0 to 255
	double varg = ( 1.0 * g / 255 );        //G from 0 to 255
	double varb = ( 1.0 * b / 255 );        //B from 0 to 255

	varr = rgb2xyz_correct(varr);
	varg = rgb2xyz_correct(varg);
	varb = rgb2xyz_correct(varb);

	//Observer. = 2бу, Illuminant = D65
	*x = varr * 0.4124 + varg * 0.3576 + varb * 0.1805;
	*y = varr * 0.2126 + varg * 0.7152 + varb * 0.0722;
	*z = varr * 0.0193 + varg * 0.1192 + varb * 0.9505;
}

// xyz 2 rgb
//----------------------------------------------------------------------------------------
static double xyz2rgb_correct(double cl) {
	if ( cl > 0.0031308 ) cl = 1.055 * ::qPow(cl, 0.41667) - 0.055;
	else  cl = cl * 12.92;

	return cl;
}

void xyz2rgb(ushort* r, ushort* g, ushort* b, double x, double y, double z)
{
	x = clamp(x, 0.0, 95.047);
	y = clamp(y, 0.0, 100.000);
	z = clamp(z, 0.0, 108.883);

	double varx = x / 100.0;        //X from 0 to  95.047      (Observer = 2бу, Illuminant = D65)
	double vary = y / 100.0;       //Y from 0 to 100.000
	double varz = z / 100.0;       //Z from 0 to 108.883

	double varr = varx *  3.2406 + vary * -1.5372 + varz * -0.4986;
	double varg = varx * -0.9689 + vary *  1.8758 + varz *  0.0415;
	double varb = varx *  0.0557 + vary * -0.2040 + varz *  1.0570;

	varr = xyz2rgb_correct(varr);
	varg = xyz2rgb_correct(varg);
	varb = xyz2rgb_correct(varb);
	
	*r = (ushort)(varr * 255.0);
	*g = (ushort)(varg * 255.0);
	*b = (ushort)(varb * 255.0);
}

// xyz 2 lab
//----------------------------------------------------------------------------------------
static double xyz2lab_correct(double cl) {
	if (cl > 0.008856) cl = ::qPow(cl, 0.33333);
	else cl = 7.787 * cl + 0.13793;
	return cl;
}

void xyz2lab(double* l, double* a, double* b, double x, double y, double z){
	double varx = x / 95.047;		//ref_X = Illuminant= D65 95.047
	double vary = y / 100.00;		//ref_Y = D65 100.00
	double varz = z / 108.88;        //ref_Z =  D65 108.88

	varx = xyz2lab_correct(varx);
	vary = xyz2lab_correct(vary);
	varz = xyz2lab_correct(varz);

	*l = ( 116.0 * vary ) - 16.0;
	*a = 500.0 * ( varx - vary );
	*b = 200.0 * ( vary - varz );
}

// lab 2 xyz
//----------------------------------------------------------------------------------------
static double lab2xyz_correct(double cl) {
	if (::qPow(cl, 3.0) > 0.008856) cl = ::qPow(cl, 3.0);
	else cl = (cl - 0.13793)/7.787;
	return cl;
}

void lab2xyz(double* x, double* y, double* z, double l, double a, double b)
{
	double vary = ( l + 16.0 ) / 116.0;
	double varx = a / 500.0 + vary;
	double varz = vary - b / 200.0;

	vary = lab2xyz_correct(vary);
	varx = lab2xyz_correct(varx);
	varz = lab2xyz_correct(varz);

	//  Observer= 2бу, Illuminant= D65
	*x = 95.047 * varx;
	*y = 100.000 * vary;
	*z = 108.883 * varz;
}

// rgb 2 lab
//----------------------------------------------------------------------------------------
void rgb2lab(double* l, double* a, double* bex, ushort r, ushort g, ushort b)
{
	double x, y, z;
	rgb2xyz(&x, &y, &z, r, g, b);
	xyz2lab(l, a, bex, x, y, z);
}

// lab 2 rgb
//----------------------------------------------------------------------------------------
void lab2rgb(ushort* r, ushort* g, ushort* b, double l, double a, double bex)
{
	double x, y, z;
	lab2xyz(&x, &y, &z, l, a, bex);
	xyz2rgb(r, g, b, x, y, z);
}

};// end of Dui