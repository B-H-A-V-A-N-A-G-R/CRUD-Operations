#pragma once
#include "afxdialogex.h"
#include "string"

// CMenuDlg dialog

class CMenuDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMenuDlg)

public:
	CMenuDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMenuDlg();

	std::string logged_in_email;  // Store the logged-in user's email 

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuRead();
	afx_msg void OnMenuUpdate();
	afx_msg void OnMenuDelete();
};
