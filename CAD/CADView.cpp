
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

// �����ļ�
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
	// ��ȡ����
	BOOL bRet = GetPrivateProfileStruct("CAD", "Pen", &m_logPen, sizeof(m_logPen), INI_FILE);
	bRet = GetPrivateProfileStruct("CAD", "Brush", &m_logBrush, sizeof(m_logBrush), INI_FILE);

	if (!bRet)
	{
		// ��һ���������򣬸�Ĭ��ֵ
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

	// ����״̬��
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetMessageText("������");

	// ��ȡ�ͻ�����С
	CRect rcClient;
	GetClientRect(&rcClient);
	
	// ʹ��˫����
	// 1.��������DC
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	// 2.��������λͼ��ѡ��λͼ
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(bmpMem);
	// ���ñ���Ϊ��ɫ
	dcMem.FillSolidRect(&rcClient, RGB(255, 255, 255));
	// 3.���ڴ��л�ͼ
	POSITION position = m_lstShapes.GetHeadPosition();
	while (position != NULL)
	{
		IShape* shape = m_lstShapes.GetNext(position);
		if (shape == m_pSelShape)
		{
			// ����ѡ�е�ͼ��
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
	// 4.��ͼƬ���ڴ������ͻ���
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	pMainFrame->SetMessageText("�������");
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
	// ���մ�����������Ϣ
	SetCapture();

	// ���ѡ�е�ͼ��
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
		// �ж�������������ĸ�ͼ�η�Χ��
		POSITION position = m_lstShapes.GetTailPosition();
		while (position != NULL)
		{
			IShape* pShape = m_lstShapes.GetPrev(position);
			if (pShape->IsSelected(point))
			{
				// ѡ����
				m_pSelShape = pShape;
				m_ptMovePre = point;
				m_ptMoveBegin = point;
				m_nOptionType = OT_MOVE;
				break;
			}
		}

		// ���»���
		InvalidateRect(NULL, FALSE);

		break;
	}
	}

	CView::OnLButtonDown(nFlags, point);
}


void CCADView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// ���ٽ��մ�����������Ϣ
	ReleaseCapture();

	if (m_pCurShape != NULL)
	{
		// �����������յ���ͬ��ͼ��
		if (m_pCurShape->GetBeginPoint() == point)
		{
			delete m_pCurShape;
			m_pCurShape = nullptr;
		}
		else
		{
			m_pCurShape->SetEndPoint(point);
			// ����ͼ��
			m_lstShapes.AddTail(m_pCurShape);

			// ���ͼ�β�����ջ
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
		// �ƶ�ͼ�β�����ջ
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

	// ������ʾ��Ϣ����
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
	// ��������ֵ
	penBrushDialog.m_logPen = m_logPen;
	penBrushDialog.m_logBrush = m_logBrush;

	penBrushDialog.DoModal();

	// ��ȡ���ĺ��ֵ
	m_logPen = penBrushDialog.m_logPen;
	m_logBrush = penBrushDialog.m_logBrush;

	// ���ĵ�ǰѡ��ͼ�εĻ��ʺͻ�ˢ
	if (m_pSelShape != NULL)
	{
		m_pSelShape->SetPen(m_logPen);
		m_pSelShape->SetBrush(m_logBrush);
		InvalidateRect(NULL, FALSE);
	}

	// ���浽�����ļ�
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

	// ���Ҳ�ɾ��
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

	// ������ջ
	m_operationManager.InsertOperation(new CDeleteOperation(m_pSelShape, &m_lstShapes, nIdx));

	//delete m_pSelShape;
	m_pSelShape = NULL;

	// ���»���
	InvalidateRect(NULL, FALSE);
}


void CCADView::OnOptionRotate()
{
	if (m_pSelShape == NULL)
	{
		return;
	}

	m_pSelShape->Rotate();

	// ������ջ
	m_operationManager.InsertOperation(new CRotateOperation(m_pSelShape));

	// ���»���
	InvalidateRect(NULL, FALSE);
}


void CCADView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// �ͻ�������ת��Ϊ��Ļ����
	ClientToScreen(&point);

	// ��ȡ�Ӳ˵�������
	//CMenu* pSubMenu = LoadMenu(IDR_MAINFRAME).GetSubMenu(4);
	//pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, ID_OPTION_DELETE, "ɾ��");
	menu.AppendMenu(MF_STRING, ID_OPTION_ROTATE, "��ת");
	menu.AppendMenu(MF_STRING, ID_SET_PEN_BRUSH, "�޸Ļ��ʻ�ˢ");
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
