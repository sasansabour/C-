
// KarAmoziProject.h : main header file for the KarAmoziProject application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKarAmoziProjectApp:
// See KarAmoziProject.cpp for the implementation of this class
//

class CKarAmoziProjectApp : public CWinApp
{
public:
	CKarAmoziProjectApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKarAmoziProjectApp theApp;
