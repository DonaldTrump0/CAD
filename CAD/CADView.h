
// CADView.h : interface of the CCADView class
//

#pragma once
#include "IShape.h"
#include "CLine.h"
#include "CRectangle.h"
#include "CEllipse.h"
#include "COperationManager.h"
#include "CADDoc.h"

class CCADView : public CView
{
protected: // create from serialization only
	CCADView() noexcept;
	DECLARE_DYNCREATE(CCADView)

// Attributes
public:
	CCADDoc* GetDocument() const;

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
	virtual ~CCADView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

private:
	enum OPTION_TYPE
	{
		OT_DRAW,
		OT_SELECT,
		OT_MOVE
	};

private:
	IShape* m_pSelShape;			// 当前选中的图形
	IShape* m_pCurShape;			// 当前正在绘制的图形
	IShpaeFactory* m_pShapeFactory;	// 当前图形的工厂类
	CList<IShape*> m_lstShapes;		// 保存图形的链表

	LOGPEN m_logPen;				// 当前画笔和画刷属性
	LOGBRUSH m_logBrush;				

	OPTION_TYPE m_nOptionType;		// 操作类型

	CPoint m_ptMoveBegin;			// 移动操作的起始坐标
	CPoint m_ptMovePre;				// 移动操作的先前坐标

	COperationManager m_operationManager;	// 各类操作管理

public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShapeLine();
	afx_msg void OnShapeRect();
	afx_msg void OnShapeEllipse();
	afx_msg void OnSetPen();
	afx_msg void OnOptionSelect();
	afx_msg void OnOptionDelete();
	afx_msg void OnOptionRotate();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
};

#ifndef _DEBUG  // debug version in CADView.cpp
inline CCADDoc* CCADView::GetDocument() const
   { return reinterpret_cast<CCADDoc*>(m_pDocument); }
#endif

