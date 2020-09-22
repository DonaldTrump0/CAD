#pragma once


// CShapeInfoEditView view

class CShapeInfoEditView : public CEditView
{
	DECLARE_DYNCREATE(CShapeInfoEditView)

protected:
	CShapeInfoEditView();           // protected constructor used by dynamic creation
	virtual ~CShapeInfoEditView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


