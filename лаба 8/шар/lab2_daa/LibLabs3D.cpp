#include "stdafx.h"
#include "CMatrix.h"
#include "LibGraph.h"
#include "LibPyramid.h"
#include "math.h"

void DrawLightSphere(CDC& dc,double Radius,CMatrix& PView,CMatrix& PSourceLight,CRect RW,COLORREF Color,int Index)
// ������ ����� � ������ ������������
// Radius - ������ �����
// PView - ���������� ����� ���������� � ������� ����������� ������� ���������
// (r,fi(����.), q(����.))
// PSourceLight - ���������� ��������� ����� � ������� ����������� ������� ���������
// (r,fi(����.), q(����.))
// RW - ������� � ���� ��� ����������� ����
// Color - ���� ��������� �����
// Ks - �����������, ����������� ������ ��������� Ks=(0...1)-
// Index=0 -  ������������ ������ ��������� ����� :cos(e)
// Index=1 -  ���������� ������ ��������� ����� :[cos(a)]^2
// e - ���� ������� ���� ����� - ������������ ������� � ����� �������
// a - ���� ����� ���������� ����� � �������� ����������
{
  BYTE red=GetRValue(Color);
  BYTE green=GetGValue(Color);
  BYTE blue=GetBValue(Color);

  CMatrix VSphere(3),VSphereNorm(3),PV(4);  // V0(3)
  COLORREF Col;
  double df=0.7;                            // ��� �� �������,�������
  double dq=0.9;                            // �������	
  double kLight;
  VSphere(0)=Radius;	                    // ������ �����
  CMatrix VR=SphereToCart(PView);       	// ��������� ���������� ����� ����������
  CMatrix VS=SphereToCart(PSourceLight);	// ��������� ���������� ��������� �����
  CRectD  RV(-Radius,Radius,Radius,-Radius); // ������� � ������� ����������� � ��������� XY ��� ����������� �������� ����
  CMatrix MW=SpaceToWindow(RV,RW);		    // ������� (3 �� 3)��������� � ������� ������� ���������
  CMatrix MV=CreateViewCoord(PView(0),PView(1),PView(2));	//�������(4 �� 4) ���������  

  for(double fi=0;fi<=360.0;fi+=df)			// ������
	for(double q=0;q<=360.0;q+=dq)			// q= 90 - ���� �����
	{
	  VSphere(1)=fi;  // ������,VSphere(0)==Radius 
	  VSphere(2)=q;
	  CMatrix VCart=SphereToCart(VSphere);	// ��������� ���������� ����� �����                                     
	  VSphereNorm=VCart;				    // ������ ������� � ����������� �����!
	  double cos_RN=CosV1V2(VR,VSphereNorm); // ������� ���� ����� �������� ����� ���������� R � �������� ������� N � ����� �����
	  //if(cos_RN>0)	  // ���� ����� ������ �����������...
	  if(1)
	  {
		  PV(0)=VCart(0);
		  PV(1)=VCart(1);
		  PV(2)=VCart(2);
		  PV(3)=1;
		  PV=MV*PV;        // ���������� ����� � ������� ������� ���������
		  VCart(0)=PV(0);  // Xv
		  VCart(1)=PV(1);  // Yv
		  VCart(2)=1;
		  VCart=MW*VCart;  // ������� ���������� ������� ����� �����

		  CMatrix  VP=VS-VR;                 // ����������� �� �������� ����� ������������ ������� � ����� �������
		  double cos_PN=CosV1V2(VP,VSphereNorm); // ������� ���� ������� ���� ( ����� VP � �������� VSphereNorm
		  if(cos_PN>0)	// ���� ����� ����� ����������...
			{
			  if(Index==0) // ������������ ������ ���������
				{
				  kLight=cos_PN;
				}
			  if(Index==1) // ���������� ������ ���������
				{
				  double xx=2*ModVec(VP)*cos_PN/ModVec(VSphereNorm);
				  CMatrix RX=VSphereNorm*xx-VP; //���������� ���
				  double cos_A=CosV1V2(RX,VR);  // ������� ���� ����� ���������� ����� � �������� ����������
				  if(cos_A>0)kLight=cos_A*cos_A; 
				  else kLight=0;			  
				}
			  Col=RGB(kLight*255,kLight,kLight);
			  dc.SetPixel((int)VCart(0),(int)VCart(1),Col);
			}
		  else
			{
			  Col=RGB(0,0,0);
			  dc.SetPixel((int)VCart(0),(int)VCart(1),Col);
			}
		}
	}
}


