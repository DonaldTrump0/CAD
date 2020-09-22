// CShapeInfoEditView.cpp : implementation file
//

#include "pch.h"
#include "CAD.h"
#include "CShapeInfoEditView.h"


// CShapeInfoEditView

IMPLEMENT_DYNCREATE(CShapeInfoEditView, CEditView)

CShapeInfoEditView::CShapeInfoEditView()
{

}

CShapeInfoEditView::~CShapeInfoEditView()
{
}

BEGIN_MESSAGE_MAP(CShapeInfoEditView, CEditView)
END_MESSAGE_MAP()


// CShapeInfoEditView diagnostics

#ifdef _DEBUG
void CShapeInfoEditView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CShapeInfoEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CShapeInfoEditView message handlers
