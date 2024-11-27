
// Login_ProjectDlg.h : header file
//

#pragma once
#include <string>

// CLoginProjectDlg dialog
class CLoginProjectDlg : public CDialogEx
{
// Construction
public:
	CLoginProjectDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN_PROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_email;
	CString m_password;
	afx_msg void OnBnClickedButtonSignin();

	
};
