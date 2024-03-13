
// Task6.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTask6App:
// Сведения о реализации этого класса: Task6.cpp
//

class CTask6App : public CWinApp
{
public:
	CTask6App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTask6App theApp;
