#include "stdafx.h"
#include "CMatrix.h"
#include "LibGraph.h"
#include "math.h"

//void DrawPyramid(CDC& dc,CMatrix& VA,CMatrix& VB,CMatrix& VC,CMatrix& VD,CMatrix& PView)
//// ������ �������� ��� �������� ��������� ����� 
//// � ������ MM_ANISOTROPIC - ��� ������������ ��������� ���������
//// � ������� ������ ������� ����� � ��������� ������ ������ ���� �����������
//// dc - ������ �� ����� CDC MFC
//// D - ������� ��������
//// ABC - ��������� ��������
//// VA,VB,VC - ���������� ������ A,B,C � ������� ������� ���������
//// PView - ���������� ����� ���������� � ������� ����������� ������� ���������
//// (r,fi(����.), q(����.))
//{
//	double r=PView(0);
//	double fi=PView(1);
//	double q=PView(2);
//	CMatrix MV=CreateViewCoord(r,fi,q);
//
//	CMatrix Ve=MV*VA; // ���������� ������� � � ������� �����������
//	int ax=(int)Ve(0);
//	int ay=(int)Ve(1);
//
//	Ve=MV*VB;         // ���������� ������� B � ������� �����������
//	int bx=(int)Ve(0);
//	int by=(int)Ve(1);
//
//	Ve=MV*VC;         // ���������� ������� C � ������� �����������
//	int cx=(int)Ve(0);
//	int cy=(int)Ve(1);
//
//	Ve=MV*VD;         // ���������� ������� D � ������� �����������
//	int dx=(int)Ve(0);
//	int dy=(int)Ve(1);
//
//	CPen MyPen1,MyPen2;
//	MyPen1.CreatePen(PS_SOLID,0, RGB(0,0,255)); 
//	CPen* pOldPen= dc.SelectObject(&MyPen1);
//	
//	dc.MoveTo(ax,ay);	 // ����� A 	
//	dc.LineTo(bx,by);  // ����� AB
//	dc.LineTo(cx,cy);  // ����� BC
//	dc.LineTo(ax,ay);  // ����� CA
//
//	MyPen2.CreatePen(PS_SOLID,0, RGB(255,0,0)); 
//	dc.SelectObject(&MyPen2);
//
//	dc.LineTo(dx,dy);  // ����� AD
//	dc.LineTo(bx,by);  // ����� DB
//	dc.MoveTo(dx,dy);	 // ����� D
//	dc.LineTo(cx,cy);  // ����� DC
//
//	dc.SelectObject(pOldPen);
//}
//
//void DrawPyramid1(CDC& dc,CMatrix& VA,CMatrix& VB,CMatrix& VC,CMatrix& VD,CMatrix& PView)
//// ������ �������� � ��������� ��������� ����� 
//// � ������ MM_ANISOTROPIC - ��� ������������ ��������� ���������
//// � ������� ������ ������� ����� � ��������� ������ ������ ���� �����������
//// dc - ������ �� ����� CDC MFC
//// D - ������� ��������
//// ABC - ��������� ��������
//// VA,VB,VC - ���������� ������ A,B,C � ������� ������� ���������
//// PView - ���������� ����� ���������� � ������� ����������� ������� ���������
//// (r,fi(����.), q(����.))
//{
//// ��������� ��������� ���������� ������� ����� ����������
//	double r=PView(0);
//	double fi=PView(1);						// �������
//	double q=PView(2);						// �������
//	double fi_rad=(fi/180.0)*pi;	// ������� fi � �������
//	double q_rad=(q/180.0)*pi;		// ������� q � �������
//	double zv=r*cos(q_rad);
//	double xv=r*sin(q_rad)*cos(fi_rad);
//	double yv=r*sin(q_rad)*sin(fi_rad);
//	CMatrix VR(3);
//	VR(0)=xv;  VR(1)=yv;     VR(2)=zv;
////----------���������� ������ � ������� ����������� -----------------------
//	POINT A,B,C,D;
//	CMatrix MV=CreateViewCoord(r,fi,q);	// ������� ��������� � ������� ������� ���������
//	CMatrix Ve=MV*VA; // ���������� ������� � � ������� �����������
//	A.x=(int)Ve(0);
//	A.y=(int)Ve(1);
//
//	Ve=MV*VB;         // ���������� ������� B � ������� �����������
//	B.x=(int)Ve(0);
//	B.y=(int)Ve(1);
//
//	Ve=MV*VC;         // ���������� ������� C � ������� �����������
//	C.x=(int)Ve(0);
//	C.y=(int)Ve(1);
//
//	Ve=MV*VD;         // ���������� ������� D � ������� �����������
//	D.x=(int)Ve(0);
//	D.y=(int)Ve(1);
//
//	CMatrix AD=VD-VA;                 // ������ AD - ������ 4
//	CMatrix AC=VC-VA;                 // ������ AC - ������ 4
//	CMatrix AB=VB-VA;                 // ������ AB - ������ 4
//	CMatrix BD=VD-VB;                 // ������ BD - ������ 4
//	CMatrix BC=VC-VB;                 // ������ BC - ������ 4
//	//-- ������� ��������� ������� �� ��������
//	AD.RedimData(3);
//	AC.RedimData(3);
//	AB.RedimData(3);
//	BD.RedimData(3);
//	BC.RedimData(3);
//
//	CMatrix VN=VectorMult(AC,AD);			// ������ �������(!)������� � ����� ACD
//
//	double sm=ScalarMult(VN,VR);      // ��������� ������������ ������� ������� � ����� �
//																		// � ������� ����� ����������
//	if(!(sm<0)) // ����� ACD ������ - ������ ����� 
//	{		
//		POINT ACD[]={A,C,D,A};
//		dc.Polyline(ACD,4);
//	}
//
//	VN=VectorMult(AD,AB);		  	// ������ �������(!)������� � ����� ADB
//	sm=ScalarMult(VN,VR);       // ��������� ������������ ������� ������� � ����� �
//														  // � ������� ����� ����������
//	if(!(sm<0)) // ����� ADB ������ - ������ ����� 
//	{		
//		POINT ADB[]={A,B,D,A};
//		dc.Polyline(ADB,4);
//	}
//
//	VN=VectorMult(AB,AC);		  	// ������ �������(!)������� � ����� ABC
//	sm=ScalarMult(VN,VR);       // ��������� ������������ ������� ������� � ����� �
//													  	// � ������� ����� ����������
//	if(!(sm<0)) // ����� ABC ������ - ������ ����� 
//	{		
//		POINT ABC[]={A,B,C,A};
//		dc.Polyline(ABC,4);
//	}
//
//	VN=VectorMult(BD,BC);		  	// ������ �������(!)������� � ����� BDC
//	sm=ScalarMult(VN,VR);       // ��������� ������������ ������� ������� � ����� �
//													  	// � ������� ����� ����������
//	if(!(sm<0)) // ����� BDC ������ - ������ ����� 
//	{		
//		POINT BDC[]={B,D,C,B};
//		dc.Polyline(BDC,4);
//	}
//
//}
