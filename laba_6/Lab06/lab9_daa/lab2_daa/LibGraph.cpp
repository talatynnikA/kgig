#include "stdafx.h"
#include "CMatrix.h"
#include "LibGraph.h"
#include "math.h"

CRectD::CRectD(double l,double t,double r,double b)////////////
{
  left = l; 
  top = t; 
  right = r; 
  bottom = b; 
}
//------------------------------------------------------------------------------
void CRectD::SetRectD(double l,double t,double r,double b)///////////
{
  left = l; 
  top = t; 
  right = r; 
  bottom = b; 
}

//------------------------------------------------------------------------------
CSizeD CRectD::SizeD()/////////////
{
  CSizeD cz;
  cz.cx = fabs(right-left);
  cz.cy = fabs(top-bottom);
  return cz;
}

//------------------------------------------------------------------------------

CMatrix SpaceToWindow(CRectD& RS,CRect& RW)/////////////
{
	CMatrix M(3,3);
  CSize sz = RW.Size();	 
  int dwx = sz.cx;	     
  int dwy = sz.cy;	     
  CSizeD szd = RS.SizeD();

  double dsx = szd.cx;    
  double dsy = szd.cy;    
  
  double kx = (double)dwx/dsx;
  double ky = (double)dwy/dsy;
		

  M(0,0) = kx;  M(0,1) = 0;    M(0,2) = (double)RW.left-kx*RS.left;
  M(1,0) = 0;   M(1,1) = -ky;  M(1,2) = (double)RW.bottom+ky*RS.bottom;
  M(2,0) = 0;   M(2,1) = 0;    M(2,2) = 1;
	return M;
 }
//--------------------------------------------------------------------------------
CMatrix CreateViewCoord(double r,double fi,double q)///////////
{
	double fg = fmod(fi,360.);
  double ff = (fg/180.)*pi;
	fg = fmod(q,360.);
	double qq = (fg/180.)*pi;

	CMatrix VM(4,4);
	VM(0,0) = -sin(ff);   VM(0,1) = cos(ff);
	VM(1,0) = -cos(qq)*cos(ff);	VM(1,1) = -cos(qq)*sin(ff);	VM(1,2) = sin(qq);
	VM(2,0) = -sin(qq)*cos(ff);	VM(2,1) = -sin(qq)*sin(ff);	VM(2,2) = -cos(qq); VM(2,3) = r;
	VM(3,3) = 1;
	return VM;
}

//-----------------------------------------------------------------------------------
CMatrix SphereToCart(CMatrix& PView)//////////
{
	CMatrix R(3);
	double r = PView(0);
	double fi = PView(1);	
	double q = PView(2);	
	double fi_rad = (fi/180.0)*pi;
	double q_rad = (q/180.0)*pi;
	R(0) = r*sin(q_rad)*cos(fi_rad);
	R(1) = r*sin(q_rad)*sin(fi_rad);
	R(2) = r*cos(q_rad);			
	return R;
}