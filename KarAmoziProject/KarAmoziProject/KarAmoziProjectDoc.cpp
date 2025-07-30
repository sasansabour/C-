
// KarAmoziProjectDoc.cpp : implementation of the CKarAmoziProjectDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "KarAmoziProject.h"
#endif
#include "KarAmoziProjectDoc.h"
#include <propkey.h>
#include <math.h>
#include "fftw3.h"
#pragma comment(lib, "libfftw3-3.lib")

struct Scplx{
	double real;
	double imag;
};

//__forceinline double sqr(double x){ return x*x; }
__forceinline double absx2(Scplx x){ return sqrt( sqrt(x.real) + sqrt(x.imag) ); }

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKarAmoziProjectDoc

IMPLEMENT_DYNCREATE(CKarAmoziProjectDoc, CDocument)

BEGIN_MESSAGE_MAP(CKarAmoziProjectDoc, CDocument)
END_MESSAGE_MAP()


// CKarAmoziProjectDoc construction/destruction

CKarAmoziProjectDoc::CKarAmoziProjectDoc()
{
	// TODO: add one-time construction code here
	
}

CKarAmoziProjectDoc::~CKarAmoziProjectDoc()
{
}

BOOL CKarAmoziProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

// CKarAmoziProjectDoc serialization

void CKarAmoziProjectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CKarAmoziProjectDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CKarAmoziProjectDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CKarAmoziProjectDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CKarAmoziProjectDoc diagnostics

#ifdef _DEBUG
void CKarAmoziProjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKarAmoziProjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CKarAmoziProjectDoc commands
