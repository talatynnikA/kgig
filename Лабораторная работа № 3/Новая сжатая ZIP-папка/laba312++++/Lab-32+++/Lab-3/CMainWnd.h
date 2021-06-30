#pragma once
#include "resource.h"
#include "CMatrix.h"
#include "Lib.h"
#include "CPlot2D.h"
#include <string>


CMatrix SpaceToWindow(CRectD& RS, CRect& RW)	// Возвращает матрицу пересчета координат из мировых в оконные
{
	CMatrix M(3, 3);
	CSize sz = RW.Size();          // Размер области в ОКНЕ
	int dwx = sz.cx;               // Ширина
	int dwy = sz.cy;               // Высота
	CSizeD szd = RS.SizeD();       // Размер области в МИРОВЫХ координатах

	double dsx = szd.cx;           // Ширина в мировых координатах
	double dsy = szd.cy;           // Высота в мировых координатах

	double kx = (double)dwx / dsx; // Масштаб по x
	double ky = (double)dwy / dsy; // масштаб по y

	M(0, 0) = kx; M(0, 1) = 0;   M(0, 2) = (double)RW.left - kx*RS.left;
	M(1, 0) = 0;  M(1, 1) = -ky; M(1, 2) = (double)RW.bottom + ky*RS.bottom;
	M(2, 0) = 0;  M(2, 1) = 0;   M(2, 2) = 1;

	return M;
}
void SetMyMode(CDC& dc, CRectD& RS, CRect& RW)
{// Устанавливает режим отображения MM_ANISOTROPIC и его параметры
// dc - ссылка на класс CDC MFC
// RS -  область в мировых координатах 
// RW -	 Область в оконных координатах - int  

	double dsx = RS.right - RS.left;
	double dsy = RS.top - RS.bottom;
	double xsL = RS.left;
	double ysL = RS.bottom;

	int dwx = RW.right - RW.left;
	int dwy = RW.bottom - RW.top;
	int xwL = RW.left;
	int ywL = RW.bottom;

	dc.SetMapMode(MM_ANISOTROPIC);       // Установка режима отображения
	dc.SetWindowExt((int)dsx, (int)dsy); // Измененение ориентации и масштаба
	dc.SetViewportExt(dwx, -dwy);
	dc.SetWindowOrg((int)xsL, (int)ysL); // Установка смещения
	dc.SetViewportOrg(xwL, ywL);
}


class CMainWnd : public CFrameWnd
{
private:
	CMatrix X, Y;
	CRect RW;
	CRectD RS;
	CPlot2D Graph[4];
	CMyPen PenLine, PenAxis;
	bool viewAll = false;
	int numGraph = 0;
	enum { menuTestsF1, menuTestsF2, menuTestsF3, menuTestsF4, menuTestsF1234, clear } condition;
	CMenu menu;
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);

public:
	CMainWnd::CMainWnd() {
		Create(NULL, L"Lab-3", WS_OVERLAPPEDWINDOW, CRect(10, 10, 800, 600), NULL, NULL);
		condition = clear;
	}

	void OnPaint();

	void SetFunction(std::string, double, double, double, CMyPen&, CMyPen&, CRect);
	void MenuTestsF1();
	void MenuTestsF2();
	void MenuTestsF3();
	void MenuTestsF4();
	void MenuTestsF1234();

	double MyF1(double);
	double MyF2(double);
	double MyF3(double);
	double MyF4(double);

	void Clear();
	void Exit();
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_TESTS_F1, MenuTestsF1)
	ON_COMMAND(ID_TESTS_F2, MenuTestsF2)
	ON_COMMAND(ID_TESTS_F3, MenuTestsF3)
	ON_COMMAND(ID_TESTS_F4, MenuTestsF4)
	ON_COMMAND(ID_TESTS_F1234, MenuTestsF1234)
	ON_COMMAND(ID_CLEAR, Clear)
	ON_COMMAND(ID_EXIT, Exit)
	ON_WM_PAINT()
END_MESSAGE_MAP()
int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	menu.LoadMenu(IDR_MENU1); // Загрузить меню из файла ресурса
	SetMenu(&menu); // Установить меню
	return 0;
}


void CMainWnd::OnPaint()
{
	CPaintDC dc(this);
	switch (condition)
	{
	case menuTestsF1: {
		Graph[numGraph].Draw(dc, 1, 1);
	} break;
	case menuTestsF2: {
		Graph[numGraph].GetRS(RS);
		SetMyMode(dc, RS, RW); // Устанавливаем режим отображения MM_ANISOTROPIC
		Graph[numGraph].Draw1(dc, 1, 1);
	} break;
	case menuTestsF3: {
		Graph[numGraph].Draw(dc, 1, 1);
	} break;
	case menuTestsF4: {
		Graph[numGraph].GetRS(RS);
		SetMyMode(dc, RS, RW);
		Graph[numGraph].Draw1(dc, 1, 1);
	} break;
	case menuTestsF1234: {
		viewAll = true;

		PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));
		PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));
		SetFunction("MyF1", (pi / 36), (-3 * pi), (3 * pi), PenLine, PenAxis, { 50, 50, 300, 200 });
		Graph[0].Draw(dc, 1, 1);

		PenLine.Set(PS_SOLID, 0, RGB(0, 255, 0));
		PenAxis.Set(PS_SOLID, 0, RGB(0, 0, 255));
		SetFunction("MyF2", (0.25), (-5), (5), PenLine, PenAxis, { 450, 50, 700, 200 });
		Graph[1].GetRS(RS);
		SetMyMode(dc, RS, RW);
		Graph[1].Draw1(dc, 1, 1);
		dc.SetMapMode(MM_TEXT);

		PenLine.Set(PS_DASHDOT, 1, RGB(255, 0, 0));
		PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));
		SetFunction("MyF3", (pi / 36), (0), (6 * pi), PenLine, PenAxis, { -400, -25, -150, 125 });
		Graph[2].Draw(dc, 1, 1);

		PenLine.Set(PS_SOLID, 2, RGB(255, 0, 0));
		PenAxis.Set(PS_SOLID, 0, RGB(0, 0, 255));
		SetFunction("MyF4", (0.25), (-10), (10), PenLine, PenAxis, { 450, 300, 700, 450 });
		Graph[3].GetRS(RS);
		SetMyMode(dc, RS, RW);
		Graph[3].Draw1(dc, 1, 1);
		dc.SetMapMode(MM_TEXT);

		viewAll = false;
	} break;
	case clear: {
		Invalidate();
	} break;
	}
}

void CMainWnd::SetFunction(std::string nameFunc, double _dx, double _xL, double _xH, CMyPen &_Pline, CMyPen &_PAxis, CRect _rect = { 150, 50, 600, 450 })
{
	double dx = _dx;
	double xL = _xL;
	double xH = _xH;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);

	for (int i = 0; i <= N; i++) {
		X(i) = xL + i*dx;
		if (nameFunc == "MyF1")
			Y(i) = MyF1(X(i));
		if (nameFunc == "MyF2")
			Y(i) = MyF2(X(i));
		if (nameFunc == "MyF3")
			Y(i) = MyF3(X(i));
		if (nameFunc == "MyF4")
			Y(i) = MyF4(X(i));
	}
	RW.SetRect(_rect.left, _rect.top, _rect.right, _rect.bottom);
	if (numGraph == 4)
		numGraph = 0;
	if (viewAll)
	{
		if (numGraph < 4)
		{
			Graph[numGraph].SetParams(X, Y, RW);
			Graph[numGraph].SetPenLine(_Pline);
			Graph[numGraph].SetPenAxis(_PAxis);

			numGraph++;
		}
	}
	else
	{
		Graph[0].SetParams(X, Y, RW);
		Graph[0].SetPenLine(_Pline);
		Graph[0].SetPenAxis(_PAxis);
	}

	if (!viewAll)
	{
		if (nameFunc == "MyF1")
			condition = menuTestsF1;
		if (nameFunc == "MyF2")
			condition = menuTestsF2;
		if (nameFunc == "MyF3")
			condition = menuTestsF3;
		if (nameFunc == "MyF4")
			condition = menuTestsF4;
	}
}
void CMainWnd::MenuTestsF1()
{
	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));
	Invalidate();
	SetFunction("MyF1", (pi / 36), (-3 * pi), (3 * pi), PenLine, PenAxis);
}
void CMainWnd::MenuTestsF2()
{
	PenLine.Set(PS_SOLID, 0, RGB(0, 255, 0));
	PenAxis.Set(PS_SOLID, 0, RGB(0, 0, 255));
	Invalidate();
	SetFunction("MyF2", (0.25), (-5), (5), PenLine, PenAxis);

}
void CMainWnd::MenuTestsF3()
{
	PenLine.Set(PS_DASHDOT, 2, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));
	Invalidate();
	SetFunction("MyF3", (pi / 36), (0), (6 * pi), PenLine, PenAxis);
}
void CMainWnd::MenuTestsF4()
{
	PenLine.Set(PS_SOLID, 2, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 0, RGB(0, 0, 255));
	Invalidate();
	SetFunction("MyF4", (0.25), (-10), (10), PenLine, PenAxis);
}
void CMainWnd::MenuTestsF1234()
{
	Invalidate();
	condition = menuTestsF1234;
}

double CMainWnd::MyF1(double x)
{
	return sin(x) / x;
}
double CMainWnd::MyF2(double x)
{
	return x*x*x;
}
double CMainWnd::MyF3(double x)
{
	return sqrt(x)*sin(x);
}
double CMainWnd::MyF4(double x)
{
	return x*x;
}

void CMainWnd::Clear()
{
	condition = clear;
	RedrawWindow();
}
void CMainWnd::Exit()
{
	DestroyWindow();
}