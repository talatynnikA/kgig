
// lab8.h : ������� ���� ��������� ��� ���������� lab8
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"       // �������� �������


// Clab8App:
// � ���������� ������� ������ ��. lab8.cpp
//

class Clab8App : public CWinApp
{
public:
	Clab8App();


// ���������������
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����������

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clab8App theApp;
