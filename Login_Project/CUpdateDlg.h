#pragma once
#include "afxdialogex.h"
#include "string"

// CUpdateDlg dialog

class CUpdateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateDlg)

public:
	CUpdateDlg(const std::string& email, CWnd* pParent = nullptr);   // // Constructor with email
	virtual ~CUpdateDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UPDATE };
#endif

private:
	std::string logged_in_email;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_old_pass;
	CString m_new_pass;
	CString m_confirm_pass;
	afx_msg void OnBnClickedOk(); // Handler for Update Password action
	  
};
