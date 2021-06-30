CMatrix SpaceToWindow(CRectD&, CRect&);
void SetMyMode(CDC&, CRectD&, CRect&);

struct CMyPen
{
	int PenStyle;	   // ����� ����
	int PenWidth;	   // ������� ���� 
	COLORREF PenColor; // ���� ���� 

	CMyPen()
	{ 
		PenStyle = PS_SOLID; PenWidth = 1; PenColor = RGB(0, 0, 0); 
	};
	void Set(int PS, int PW, COLORREF PC)
	{
		PenStyle = PS; PenWidth = PW; PenColor = PC;
	};
};


class CPlot2D
{
private:
	CMatrix X;		// ��������
	CMatrix Y;		// �������
	CMatrix K;		// ������� ��������� ��������
	CRect RW;		// ������������� � ����
	CRectD RS;		// ������������� ������� � ���
	CMyPen PenLine; // ���� ��� �����
	CMyPen PenAxis; // ���� ��� ����

public:
	CPlot2D() // ����������� �� ���������
	{ 
		K.RedimMatrix(3, 3); 
	};
	void SetParams(CMatrix&, CMatrix&, CRect&);        // ��������� ���������� �������
	void SetWindowRect(CRect&);	                       // ��������� ������� � ���� ��� ����������� �������
	void GetWindowCoords(double, double, int&, int &); // �������� ���������� ����� �� ��� � ������� ��
	void SetPenLine(CMyPen&);	                       // ���� ��� ��������� �������
	void SetPenAxis(CMyPen&);	                       // ���� ��� ���� ���������
	void Draw(CDC&, int, int);	                       // ��������� � ��������������� ���������� ���������
	void Draw1(CDC&, int, int);                        // ��������� � ��� ���������������� ��������� ���������
	void GetRS(CRectD&);                               // ���������� ������� ������� � ������� ��
};

void CPlot2D::SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX)
{
	int nRowsX = XX.rows();
	int nRowsY = YY.rows();

	if (nRowsX != nRowsY)
	{
		TCHAR* error = TEXT("SetParams: ������������ �������");
		MessageBox(NULL, error, TEXT("������"), MB_ICONSTOP);
		exit(1);
	}

	X.RedimMatrix(nRowsX);
	Y.RedimMatrix(nRowsY);
	X = XX;
	Y = YY;
	double x_max = X.MaxElement();
	double x_min = X.MinElement();
	double y_max = Y.MaxElement();
	double y_min = Y.MinElement();
	RS.SetRectD(x_min, y_max, x_max, y_min);
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);
	K = SpaceToWindow(RS, RW);
}
void CPlot2D::SetWindowRect(CRect& RWX)
{
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);
	K = SpaceToWindow(RS, RW);
}
void CPlot2D::GetWindowCoords(double xs, double ys, int &xw, int &yw)
{
	CMatrix V(3), W(3);
	V(2) = 1;
	V(0) = xs;
	V(1) = ys;
	W = K * V;
	xw = (int)W(0);
	yw = (int)W(1);
}
void CPlot2D::SetPenLine(CMyPen& PLine)
{
	PenLine.PenStyle = PLine.PenStyle;
	PenLine.PenWidth = PLine.PenWidth;
	PenLine.PenColor = PLine.PenColor;
}
void CPlot2D::SetPenAxis(CMyPen& PAxis)
{
	PenAxis.PenStyle = PAxis.PenStyle;
	PenAxis.PenWidth = PAxis.PenWidth;
	PenAxis.PenColor = PAxis.PenColor;
}
void CPlot2D::Draw(CDC& dc, int Ind1, int Ind2)
{
	double xs, ys; // ������� ���������� �����
	int xw, yw;    // ������� ���������� �����

	if (Ind1 == 1) // ����� � ����
		dc.Rectangle(RW);
	if (Ind2 == 1) // ���� ����� ���
	{
		CPen MyPen(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen = dc.SelectObject(&MyPen);

		if (RS.left * RS.right < 0) // ����� ��� Y
		{
			xs = 0; ys = RS.top;             // ����� (0, y_max) � ���
			GetWindowCoords(xs, ys, xw, yw);
			dc.MoveTo(xw, yw);
				
			xs = 0; ys = RS.bottom;
			GetWindowCoords(xs, ys, xw, yw);
			dc.LineTo(xw, yw);
		}
		if (RS.top * RS.bottom < 0) // ����� ��� X
		{
			xs = RS.left; ys = 0;
			GetWindowCoords(xs, ys, xw, yw);
			dc.MoveTo(xw, yw);

			xs = RS.right; ys = 0;
			GetWindowCoords(xs, ys, xw, yw);
			dc.LineTo(xw, yw);
		}
		dc.SelectObject(pOldPen);
	}

	xs = X(0); ys = Y(0);
	GetWindowCoords(xs, ys, xw, yw);									//��� �������������
	CPen MyPen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
	CPen* pOldPen = dc.SelectObject(&MyPen);
	dc.MoveTo(xw, yw);
	for (int i = 1; i < X.rows(); i++)
	{
		xs = X(i); ys = Y(i);
		GetWindowCoords(xs, ys, xw, yw);
		dc.LineTo(xw, yw);
	}
	dc.SelectObject(pOldPen);
}
void CPlot2D::Draw1(CDC& dc, int Ind1, int Ind2)
{
	CRect IRS(RS.left, RS.top, RS.right, RS.bottom);
	if (Ind1 == 1)
		dc.Rectangle(IRS);
	if (Ind2 == 1)
	{
		CPen MyPen(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen = dc.SelectObject(&MyPen);

		if (RS.left * RS.right < 0) // ����� ��� Y
		{
			dc.MoveTo(0, (int)RS.top);
			dc.LineTo(0, (int)RS.bottom);
		}
		if (RS.top * RS.bottom < 0) // ����� ��� X
		{
			dc.MoveTo((int)RS.left, 0);
			dc.LineTo((int)RS.right, 0);
		}
		dc.SelectObject(pOldPen);
	}

	CPen MyPen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
	CPen* pOldPen = dc.SelectObject(&MyPen);
	dc.MoveTo((int)X(0), (int)Y(0));
	for (int i = 1; i < X.rows(); i++)
		dc.LineTo((int)X(i), (int)Y(i));
	dc.SelectObject(pOldPen);
}
void CPlot2D::GetRS(CRectD& RS)
{
	RS.left = (this->RS).left;
	RS.top = (this->RS).top;
	RS.right = (this->RS).right;
	RS.bottom = (this->RS).bottom;
}