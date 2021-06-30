// ChildView.h : ��������� ������ CChildView

#pragma once

// ���� CChildView

class CChildView : public CWnd
{
	// ��������
public:
	CChildView();

	// ��������
public:
	POINT targetPoint;
	RECT targetArea;
	bool startSelected;
	// ��������
public:

	// ���������������
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// ����������
public:
	virtual ~CChildView();

	// ��������� ������� ����� ���������
protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnRButtonDown(UINT, CPoint);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBmpLoad();
	afx_msg void OnBmpSave();
};