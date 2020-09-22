
// CADView.cpp : implementation of the CCADView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CAD.h"
#endif

#include "MainFrm.h"
#include "CADDoc.h"
#include "CADView.h"
#include "CPenBrushDialog.h"
#include "CMoveOperation.h"
#include "CAddOperation.h"
#include "CDeleteOperation.h"
#include "CRotateOperation.h"
#include "CShapeInfoEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 配置文件
#define INI_FILE "./cad.ini"

// CCADView

IMPLEMENT_DYNCREATE(CCADView, CView)

BEGIN_MESSAGE_MAP(CCADView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_SHAPE_LINE, &CCADView::OnShapeLine)
	ON_COMMAND(ID_SHAPE_RECT, &CCADView::OnShapeRect)
	ON_COMMAND(ID_SHAPE_ELLIPSE, &CCADView::OnShapeEllipse)
	ON_COMMAND(ID_SET_PEN_BRUSH, &CCADView::OnSetPen)
	ON_COMMAND(ID_OPTION_SELECT, &CCADView::OnOptionSelect)
	ON_COMMAND(ID_OPTION_DELETE, &CCADView::OnOptionDelete)
	ON_COMMAND(ID_OPTION_ROTATE, &CCADView::OnOptionRotate)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EDIT_UNDO, &CCADView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CCADView::OnEditRedo)
	ON_COMMAND(ID_FILE_SAVE, &CCADView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CCADView::OnFileOpen)
END_MESSAGE_MAP()

// CCADView construction/destruction

CCADView::CCADView() noexcept
{
	// TODO: add construction code here
	// 读取配置
	BOOL bRet = GetPrivateProfileStruct("CAD", "Pen", &m_logPen, sizeof(m_logPen), INI_FILE);
	bRet = GetPrivateProfileStruct("CAD", "Brush", &m_logBrush, sizeof(m_logBrush), INI_FILE);

	if (!bRet)
	{
		// 第一次启动程序，给默认值
		m_logPen = { PS_SOLID, {1, 0}, RGB(0, 0, 0) };
		m_logBrush = { BS_SOLID, RGB(255, 255, 255), 0 };
	}

	m_pCurShape = NULL;
	m_pSelShape = NULL;
	m_pShapeFactory = new CLineFactory();

	m_nOptionType = OT_DRAW;
}

CCADView::~CCADView()
{
}

BOOL CCADView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCADView drawing

void CCADView::OnDraw(CDC* pDC)
{
	CCADDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 设置状态栏
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetMessageText("绘制中");

	// 获取客户区大小
	CRect rcClient;
	GetClientRect(&rcClient);
	
	// 使用双缓冲
	// 1.创建兼容DC
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	// 2.创建兼容位图并选入位图
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(bmpMem);
	// 设置背景为白色
	dcMem.FillSolidRect(&rcClient, RGB(255, 255, 255));
	// 3.在内存中绘图
	POSITION position = m_lstShapes.GetHeadPosition();
	while (position != NULL)
	{
		IShape* shape = m_lstShapes.GetNext(position);
		if (shape == m_pSelShape)
		{
			// 绘制选中的图形
			shape->DrawSelected(&dcMem);
		}
		else
		{
			shape->Draw(&dcMem);
		}
	}
	if (m_pCurShape != NULL)
	{
		m_pCurShape->Draw(&dcMem);
	}
	// 4.把图片从内存贴到客户区
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	pMainFrame->SetMessageText("绘制完成");
}


// CCADView printing

BOOL CCADView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCADView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCADView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CCADView diagnostics

#ifdef _DEBUG
void CCADView::AssertValid() const
{
	CView::AssertValid();
}

void CCADView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCADDoc* CCADView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCADDoc)));
	return (CCADDoc*)m_pDocument;
}
#endif //_DEBUG


// CCADView message handlers


void CCADView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}


void CCADView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 接收窗口外的鼠标消息
	SetCapture();

	// 清除选中的图形
	m_pSelShape = NULL;

	switch (m_nOptionType)
	{
	case OT_DRAW:
	{
		m_pCurShape = m_pShapeFactory->Create();
		m_pCurShape->SetBeginPoint(point);
		m_pCurShape->SetPen(m_logPen);
		m_pCurShape->SetBrush(m_logBrush);

		break;
	}
	case OT_SELECT:
	{
		// 判断鼠标坐标落在哪个图形范围内
		POSITION position = m_lstShapes.GetTailPosition();
		while (position != NULL)
		{
			IShape* pShape = m_lstShapes.GetPrev(position);
			if (pShape->IsSelected(point))
			{
				// 选中了
				m_pSelShape = pShape;
				m_ptMovePre = point;
				m_ptMoveBegin = point;
				m_nOptionType = OT_MOVE;
				break;
			}
		}

		// 重新绘制
		InvalidateRect(NULL, FALSE);

		break;
	}
	}

	CView::OnLButtonDown(nFlags, point);
}


void CCADView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 不再接收窗口外的鼠标消息
	ReleaseCapture();

	if (m_pCurShape != NULL)
	{
		// 不保存起点和终点相同的图形
		if (m_pCurShape->GetBeginPoint() == point)
		{
			delete m_pCurShape;
			m_pCurShape = nullptr;
		}
		else
		{
			m_pCurShape->SetEndPoint(point);
			// 保存图形
			m_lstShapes.AddTail(m_pCurShape);

			// 添加图形操作入栈
			m_operationManager.InsertOperation(new CAddOperation(m_pCurShape, &m_lstShapes));

			m_pCurShape = nullptr;
		}
	}

	if (m_nOptionType == OT_MOVE)
	{
		m_nOptionType = OT_SELECT;
	}

	if (m_pSelShape != NULL && m_ptMoveBegin != point)
	{
		// 移动图形操作入栈
		m_operationManager.InsertOperation(new CMoveOperation(m_pSelShape, m_ptMoveBegin, point));
	}

	CView::OnLButtonUp(nFlags, point);
}


void CCADView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		if (m_pCurShape != NULL)
		{
			m_pCurShape->SetEndPoint(point);
			InvalidateRect(NULL, FALSE);
		}
		else if (m_pSelShape != NULL && m_nOptionType == OT_MOVE)
		{
			m_pSelShape->Move(m_ptMovePre, point);

			m_ptMovePre = point;

			InvalidateRect(NULL, FALSE);
		}
	}

	// 设置提示信息窗口
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CShapeInfoEditView* pShapeInfoEditView = (CShapeInfoEditView*)pMainFrame->m_wndSplitter.GetPane(1, 0);
	CString str;
	str.Format("x = %d, y = %d", point.x, point.y);
	pShapeInfoEditView->SetWindowText(str);

	CView::OnMouseMove(nFlags, point);
}


void CCADView::OnShapeLine()
{
	m_nOptionType = OT_DRAW;

	delete m_pShapeFactory;
	m_pShapeFactory = new CLineFactory();
}


void CCADView::OnShapeRect()
{
	m_nOptionType = OT_DRAW;

	delete m_pShapeFactory;
	m_pShapeFactory = new CRectangleFactory();
}


void CCADView::OnShapeEllipse()
{
	m_nOptionType = OT_DRAW;

	delete m_pShapeFactory;
	m_pShapeFactory = new CEllipseFactory();
}


void CCADView::OnSetPen()
{
	CPenBrushDialog penBrushDialog;
	// 设置现有值
	penBrushDialog.m_logPen = m_logPen;
	penBrushDialog.m_logBrush = m_logBrush;

	penBrushDialog.DoModal();

	// 获取更改后的值
	m_logPen = penBrushDialog.m_logPen;
	m_logBrush = penBrushDialog.m_logBrush;

	// 更改当前选中图形的画笔和画刷
	if (m_pSelShape != NULL)
	{
		m_pSelShape->SetPen(m_logPen);
		m_pSelShape->SetBrush(m_logBrush);
		InvalidateRect(NULL, FALSE);
	}

	// 保存到配置文件
	WritePrivateProfileStruct("CAD", "Pen", &m_logPen, sizeof(m_logPen), INI_FILE);
	WritePrivateProfileStruct("CAD", "Brush", &m_logBrush, sizeof(m_logBrush), INI_FILE);
}


void CCADView::OnOptionSelect()
{
	m_nOptionType = OT_SELECT;
}


void CCADView::OnOptionDelete()
{
	if (m_pSelShape == NULL)
	{
		return;
	}

	// 查找并删除
	/*POSITION position = m_lstShapes.Find(m_pSelShape);
	m_lstShapes.RemoveAt(position);*/

	int nIdx = 0;
	POSITION position = m_lstShapes.GetHeadPosition();
	while (m_lstShapes.GetNext(position) != m_pSelShape)
	{
		nIdx++;
	}

	position = m_lstShapes.Find(m_pSelShape);
	m_lstShapes.RemoveAt(position);

	// 操作入栈
	m_operationManager.InsertOperation(new CDeleteOperation(m_pSelShape, &m_lstShapes, nIdx));

	//delete m_pSelShape;
	m_pSelShape = NULL;

	// 重新绘制
	InvalidateRect(NULL, FALSE);
}


void CCADView::OnOptionRotate()
{
	if (m_pSelShape == NULL)
	{
		return;
	}

	m_pSelShape->Rotate();

	// 操作入栈
	m_operationManager.InsertOperation(new CRotateOperation(m_pSelShape));

	// 重新绘制
	InvalidateRect(NULL, FALSE);
}


void CCADView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// 客户区坐标转换为屏幕坐标
	ClientToScreen(&point);

	// 获取子菜单并弹出
	//CMenu* pSubMenu = LoadMenu(IDR_MAINFRAME).GetSubMenu(4);
	//pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, ID_OPTION_DELETE, "删除");
	menu.AppendMenu(MF_STRING, ID_OPTION_ROTATE, "旋转");
	menu.AppendMenu(MF_STRING, ID_SET_PEN_BRUSH, "修改画笔画刷");
	menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	
	CView::OnRButtonDown(nFlags, point);
}


void CCADView::OnEditUndo()
{
	m_operationManager.Undo();
	InvalidateRect(NULL, FALSE);
}


void CCADView::OnEditRedo()
{
	m_operationManager.Redo();
	InvalidateRect(NULL, FALSE);
}


void CCADView::OnFileSave()
{
	CFile file;
	//file.Open("data.bin", CFile::modeCreate | CFile::modeNoTruncate |CFile::modeReadWrite);
	file.Open("data.bin", CFile::modeCreate |CFile::modeReadWrite);
	file.SeekToEnd();

	CArchive archive(&file, CArchive::store);

	archive << m_lstShapes.GetSize();

	POSITION position = m_lstShapes.GetHeadPosition();
	while (position != NULL)
	{
		IShape* pShape = m_lstShapes.GetNext(position);
		archive << pShape;
	}

	archive.Close();
	file.Close();
}


void CCADView::OnFileOpen()
{
	CFile file;
	file.Open("data.bin", CFile::modeReadWrite);

	CArchive archive(&file, CArchive::load);

	int nCnt = 0;
	archive >> nCnt;
	while (nCnt)
	{
		IShape* pShape = NULL;
		archive >> pShape;
		m_lstShapes.AddTail(pShape);
		nCnt--;
	}

	InvalidateRect(NULL, FALSE);
}
