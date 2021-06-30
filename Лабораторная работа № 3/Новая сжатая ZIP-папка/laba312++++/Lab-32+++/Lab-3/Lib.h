#include <math.h>
const double pi=3.14159;


struct CSizeD
{
  double cx;
  double cy;
};

struct CRectD 
 { 
  double left; 
  double top; 
  double right; 
  double bottom; 
  CRectD(){ left=top=right=bottom=0; };
  CRectD(double l,double t,double r,double b);
  void SetRectD(double l,double t,double r,double b);
  CSizeD SizeD();	// Возвращает размеры(ширина, высота) прямоугольника 
 };

CRectD::CRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}
void CRectD::SetRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}
CSizeD CRectD::SizeD()
{
	CSizeD cz;
	cz.cx = fabs(right - left);	// Ширина прямоугольной области
	cz.cy = fabs(top - bottom);	// Высота прямоугольной области
	return cz;
}