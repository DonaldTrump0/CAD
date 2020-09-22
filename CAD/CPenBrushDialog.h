#pragma once


// CPenBrushDialog dialog

class CPenBrushDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPenBrushDialog)

public:
	CPenBrushDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPenBrushDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PEN_BRUSH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// »­±Ê
	CComboBox m_comboPenStyle;
	int m_nPenWidth;
	CMFCColorButton m_btnPenColor;
	LOGPEN m_logPen;

	// »­Ë¢
	CComboBox m_comboBrushStyle;
	CMFCColorButton m_btnBrushColor;
	LOGBRUSH m_logBrush;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOk();
};
