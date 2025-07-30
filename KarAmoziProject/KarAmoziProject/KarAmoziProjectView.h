
// KarAmoziProjectView.h : interface of the CKarAmoziProjectView class
//

#pragma once
#include "Wave.h"
#include "WaveIn.h"
#include "WaveDevice.h"
#include "WaveBuffer.h"
#include "Fourier.h"

class CKarAmoziProjectView : public CView
{
protected: // create from serialization only
	CKarAmoziProjectView();
	DECLARE_DYNCREATE(CKarAmoziProjectView)

// Attributes
public:
	CKarAmoziProjectDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKarAmoziProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnPaint();
	void Draw (CPaintDC * dc);
	void DrawSignal(CPaintDC* cd);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAudioStart();
	CWaveIn waveIn;
	CWaveDevice monDevice;
	CWave monWave;
	CFourier m_fft;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnFileNew();

};

#ifndef _DEBUG  // debug version in KarAmoziProjectView.cpp
inline CKarAmoziProjectDoc* CKarAmoziProjectView::GetDocument() const
   { return reinterpret_cast<CKarAmoziProjectDoc*>(m_pDocument); }
#endif

