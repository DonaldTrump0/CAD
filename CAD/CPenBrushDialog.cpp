// CPenBrushDialog.cpp : implementation file
//

#include "pch.h"
#include "CAD.h"
#include "CPenBrushDialog.h"
#include "afxdialogex.h"


// CPenBrushDialog dialog

IMPLEMENT_DYNAMIC(CPenBrushDialog, CDialogEx)

CPenBrushDialog::CPenBrushDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PEN_BRUSH, pParent)
	, m_nPenWidth(0)
{

}

CPenBrushDialog::~CPenBrushDialog()
{
}

void CPenBrushDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PEN_STYLE, m_comboPenStyle);
	DDX_Text(pDX, IDC_EDIT_PEN_WIDTH, m_nPenWidth);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_PEN_COLOR, m_btnPenColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_BRUSH_COLOR, m_btnBrushColor);
	DDX_Control(pDX, IDC_COMBO_BRUSH_STYLE, m_comboBrushStyle);
}


BEGIN_MESSAGE_MAP(CPenBrushDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CPenBrushDialog::OnBnClickedButtonOk)
END_MESSAGE_MAP()


// CPenBrushDialog message handlers


BOOL CPenBrushDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
		
	// TODO:  Add extra initialization here

	m_btnPenColor.EnableOtherButton("other", 0);
	m_btnBrushColor.EnableOtherButton("other", 0);

	// »­±Ê
	int nIdx = 0;
	m_comboPenStyle.AddString("¡ª¡ª¡ª");
	m_comboPenStyle.SetItemData(nIdx++, PS_SOLID);
	m_comboPenStyle.AddString("--------");
	m_comboPenStyle.SetItemData(nIdx++, PS_DASH);
	m_comboPenStyle.AddString("..........");
	m_comboPenStyle.SetItemData(nIdx++, PS_DOT);
	m_comboPenStyle.AddString("_._._._");
	m_comboPenStyle.SetItemData(nIdx++, PS_DASHDOT);
	m_comboPenStyle.AddString("_.._.._");
	m_comboPenStyle.SetItemData(nIdx++, PS_DASHDOTDOT);
	m_comboPenStyle.AddString("       ");
	m_comboPenStyle.SetItemData(nIdx++, PS_NULL);
	// ÉèÖÃ»­±Ê³õÊ¼Öµ
	m_comboPenStyle.SetCurSel(m_logPen.lopnStyle);
	m_nPenWidth = m_logPen.lopnWidth.x;
	m_btnPenColor.SetColor(m_logPen.lopnColor);

	// »­Ë¢
	nIdx = 0;
	m_comboBrushStyle.AddString("ÊµÐÄ");
	m_comboBrushStyle.SetItemData(nIdx++, BS_SOLID);
	m_comboBrushStyle.AddString("¿ÕÐÄ");
	m_comboBrushStyle.SetItemData(nIdx++, BS_NULL);
	// BS_HATCHED
	m_comboBrushStyle.AddString("-----");
	m_comboBrushStyle.SetItemData(nIdx++, HS_HORIZONTAL);
	m_comboBrushStyle.AddString("|||||");
	m_comboBrushStyle.SetItemData(nIdx++, HS_VERTICAL);
	m_comboBrushStyle.AddString("\\\\\\");
	m_comboBrushStyle.SetItemData(nIdx++, HS_FDIAGONAL);
	m_comboBrushStyle.AddString("/////");
	m_comboBrushStyle.SetItemData(nIdx++, HS_BDIAGONAL);
	m_comboBrushStyle.AddString("+++++");
	m_comboBrushStyle.SetItemData(nIdx++, HS_CROSS);
	m_comboBrushStyle.AddString("xxxxx");
	m_comboBrushStyle.SetItemData(nIdx++, HS_DIAGCROSS);
	//m_comboPenStyle.SetItemData(nIdx++, BS_PATTERN);
	// ÉèÖÃ»­Ë¢³õÊ¼Öµ
	if (m_logBrush.lbStyle == BS_HATCHED)
	{
		// BS_HATCHED
		m_comboBrushStyle.SetCurSel(m_logBrush.lbHatch + 2);
	}
	else
	{
		// BS_SOLIDºÍBS_NULL
		m_comboBrushStyle.SetCurSel(m_logBrush.lbStyle);
	}
	m_btnBrushColor.SetColor(m_logBrush.lbColor);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CPenBrushDialog::OnBnClickedButtonOk()
{
	UpdateData(TRUE);

	// »­±Ê
	m_logPen.lopnStyle = m_comboPenStyle.GetItemData(m_comboPenStyle.GetCurSel());
	m_logPen.lopnWidth.x = m_nPenWidth;
	m_logPen.lopnColor = m_btnPenColor.GetColor();

	// »­Ë¢
	m_logBrush.lbColor = m_btnBrushColor.GetColor();
	int nCurSel = m_comboBrushStyle.GetCurSel();
	switch (nCurSel)
	{
	case 0:
		m_logBrush.lbStyle = BS_SOLID;
		break;
	case 1:
		m_logBrush.lbStyle = BS_NULL;
		break;
	default:
		m_logBrush.lbStyle = BS_HATCHED;
		m_logBrush.lbHatch = m_comboBrushStyle.GetItemData(nCurSel);
		break;
	}

	EndDialog(0);
}
