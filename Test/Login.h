
// Test.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestApp: 
// �йش����ʵ�֣������ Test.cpp
//

class CTestApp : public CWinApp
{
public:
	CTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
private:
    ULONG_PTR m_ulpGdiToken;
};

extern CTestApp theApp;