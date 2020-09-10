#pragma once
#include "stable.h"
#include "float.h"

namespace Dui{

// rgb 2 xyz
//----------------------------------------------------------------------------------------
void rgb2xyz(double* x, double* y, double* z, ushort r, ushort g, ushort b);

// xyz 2 rgb
//----------------------------------------------------------------------------------------
void xyz2rgb(ushort* r, ushort* g, ushort* b, double x, double y, double z);

// xyz 2 lab
//----------------------------------------------------------------------------------------
void xyz2lab(double* l, double* a, double* b, double x, double y, double z);

// lab 2 xyz
//----------------------------------------------------------------------------------------
void lab2xyz(double* x, double* y, double* z, double l, double a, double b);

// rgb 2 lab
//----------------------------------------------------------------------------------------
void rgb2lab(double* l, double* a, double* bex, ushort r, ushort g, ushort b);

// lab 2 rgb
//----------------------------------------------------------------------------------------
void lab2rgb(ushort* r, ushort* g, ushort* b, double l, double a, double bex);

// tool function
//------------------------------------------------------------------------------
template<typename T>
T clamp(T var, T min, T max)
{
	if(var > max) return max;
	if(var < min) return min;
	return var;
}

};// end of Dui