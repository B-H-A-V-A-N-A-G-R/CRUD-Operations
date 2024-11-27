#pragma once
#include "afxdialogex.h"


// CReadDlg dialog

class CReadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReadDlg)

public:
	CReadDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CReadDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_READ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_email;
	afx_msg void OnBnClickedOk();
	CString m_first_name;
	CString m_last_name;
	CString m_address;
};
