
// KarAmoziProjectView.cpp : implementation of the CKarAmoziProjectView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "KarAmoziProject.h"
#endif

#include "KarAmoziProjectDoc.h"
#include "KarAmoziProjectView.h"
#include <math.h>
#include <stdlib.h>

#include "fftw3.h"
//#pragma comment(lib, "libfftw3-3.lib")

struct Scplx{
	double real;
	double imag;
};

__forceinline double absx2(Scplx x){ return sqrt( sqrt(x.real) + sqrt(x.imag) ); }


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKarAmoziProjectView

IMPLEMENT_DYNCREATE(CKarAmoziProjectView, CView)

BEGIN_MESSAGE_MAP(CKarAmoziProjectView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_COMMAND(ID_AUDIO_START, &CKarAmoziProjectView::OnAudioStart)
	ON_COMMAND(ID_FILE_NEW, &CKarAmoziProjectView::OnFileNew)
END_MESSAGE_MAP()

// CKarAmoziProjectView construction/destruction

CKarAmoziProjectView::CKarAmoziProjectView()
{
	// TODO: add construction code here
	for(int i=0;i<waveIn.m_iLenVoiceData;i++)
		waveIn.m_arVoiceData[i]=0;

}

CKarAmoziProjectView::~CKarAmoziProjectView()
{
}

BOOL CKarAmoziProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKarAmoziProjectView drawing

void CKarAmoziProjectView::OnDraw(CDC* pDC)
{
	CKarAmoziProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: add draw code for native data here
}

void CKarAmoziProjectView::Draw(CPaintDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc, RGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////
	//Draw background rectangle for top graph (signal)
	RECT rcBack = {10, 10, rc.Width()-10, rc.Height()/2-10};
	CPen pen(PS_DOT, 1, RGB(127,127,127));
	pDC->SelectObject(&pen);
	pDC->Rectangle(&rcBack);
	CFont fnt;
	fnt.CreatePointFont(140, _T("Times New Roman"));
	pDC->SelectObject(&fnt);
	rcBack.top += 10;
	rcBack.left += 10;
	pDC->DrawText(_T("Audio Signal"), 12, &rcBack, 0);
		
	//////////////////////////////////////////////////////////////////////////
	//Draw background rectangle for bottom graph (FFT)
	rcBack.top += rc.Height()/2-10;
	rcBack.bottom += rc.Height()/2-10;
	rcBack.left=10;
	pDC->Rectangle(&rcBack);
	rcBack.top += 10;
	rcBack.left += 10;
	pDC->DrawText(_T("Fourier Transform"), 17, &rcBack, 0);
}

void CKarAmoziProjectView::DrawSignal(CPaintDC* pDC)
{

	CKarAmoziProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CRect rc;
	GetClientRect(&rc);
	//////////////////////////////////////////////////////////////////////////
	//Draw top graph
	int x0 = 20;
	int y0 = rc.Height()/4;
	int step = 1;
	if(waveIn.m_iLenVoiceData > 20000)
		step = waveIn.m_iLenVoiceData / 20000;
	
	pDC->MoveTo(x0, y0);
	CPen pen2(PS_SOLID, 1, RGB(0, 0, 150));
	pDC->SelectObject(&pen2);
	for (int i = 1; i < waveIn.m_iLenVoiceData; i +=step)
	{
		int x = i * (rc.Width()) /128;
		int y = waveIn.m_arVoiceData[i]/2-rc.Height()/128;
		pDC->LineTo(x0+x, y0-y);
		pDC->MoveTo(x0+x, y0-y);
	}

	//////////////////////////////////////////////////////////////////////////
	////Draw bottom graph
	CPen pen3(PS_SOLID, 1, RGB(150, 0, 0));
	pDC->SelectObject(&pen3);
	waveIn.m_arVoiceData;
	double pdSignal[2048];
	for(int i=0; i<2048; i++)
		pdSignal[i] = waveIn.m_arVoiceData[i]/2 ;
	double pdFFT_real[2048];
	double pdFFT_image[2048];
	double pdFFT[2048];
	m_fft.fft_double(2048, 0, pdSignal, 0, pdFFT_real, pdFFT_image);
	double re,im;
	for(int i=1;i < 1024;i++)//Use FFT_LEN/4 since the data is mirrored within the array.
		pdFFT[i] = sqrt(pdFFT_real[i]*pdFFT_real[i] + pdFFT_image[i]*pdFFT_image[i]);
	y0 = rc.Height()-100;
	pDC->MoveTo(x0, y0-pdFFT[0]*rc.Height()/4);
	for (int i =0 ;i<1024 ; i++)
	{    
		int x = i * (rc.Width()) /128;
		int y = pdFFT[i] / 100 - rc.Height()/128;
		pDC->LineTo(x0+x, y0-y);
		pDC->MoveTo(x0+x, y0-y);
	}
}


// CKarAmoziProjectView printing

BOOL CKarAmoziProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKarAmoziProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKarAmoziProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKarAmoziProjectView diagnostics

#ifdef _DEBUG
void CKarAmoziProjectView::AssertValid() const
{
	CView::AssertValid();
}

void CKarAmoziProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKarAmoziProjectDoc* CKarAmoziProjectView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKarAmoziProjectDoc)));
	return (CKarAmoziProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CKarAmoziProjectView message handlers
void CKarAmoziProjectView::OnPaint()
{
	CPaintDC dc(this); 
	Draw( &dc );
    DrawSignal(&dc);
}

void CKarAmoziProjectView::OnTimer(UINT_PTR nIDEvent)
{
    Invalidate(FALSE);
	
	CView::OnTimer(nIDEvent);
}
int CKarAmoziProjectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(ID_CLOCK_TIMER, 100, NULL);

	return 0;
}

void CKarAmoziProjectView::OnAudioStart()
{
	// TODO: Add your command handler code here
	monWave.BuildFormat(1, 8000, 16);
	if ( !monDevice.IsInputFormat(monWave) ) {
		AfxMessageBox(_T("Format not support"));
		return;
	}
	waveIn.SetWaveFormat( monWave.GetFormat() );
	waveIn.SetDevice(monDevice);
	if ( !waveIn.Open() ) {
		AfxMessageBox( waveIn.GetError() );
		return;
	}
	if ( !waveIn.Record() ) {
		AfxMessageBox( waveIn.GetError() );
		return;
	}
	waveIn.m_arVoiceData;
}


void CKarAmoziProjectView::OnFileNew()
{
	// TODO: Add your command handler code here
	if ( !waveIn.Close() ) 
	{
		AfxMessageBox( waveIn.GetError() );
		return;
	}
	CPaintDC dc(this); 
	OnDraw( &dc );
}

