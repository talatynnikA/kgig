
// lab8.h : главный файл заголовка для приложения lab8
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// Clab8App:
// О реализации данного класса см. lab8.cpp
//

class Clab8App : public CWinApp
{
public:
	Clab8App();


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clab8App theApp;
