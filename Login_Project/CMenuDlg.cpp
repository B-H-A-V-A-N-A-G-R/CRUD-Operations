// CMenuDlg.cpp : implementation file
//

#include "pch.h"
#include "Login_Project.h"
#include "afxdialogex.h"
#include "CMenuDlg.h"
#include "CReadDlg.h"
#include "CUpdateDlg.h"
#include "CDeleteDlg.h"

// CMenuDlg dialog

IMPLEMENT_DYNAMIC(CMenuDlg, CDialogEx)

CMenuDlg::CMenuDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MENU, pParent)
{

}

CMenuDlg::~CMenuDlg()
{
}

void CMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMenuDlg, CDialogEx)
	ON_COMMAND(ID_MENU_READ, &CMenuDlg::OnMenuRead)
	ON_COMMAND(ID_MENU_UPDATE, &CMenuDlg::OnMenuUpdate)
	ON_COMMAND(ID_MENU_DELETE, &CMenuDlg::OnMenuDelete)
END_MESSAGE_MAP()


// CMenuDlg message handlers


void CMenuDlg::OnMenuRead()
{
	// TODO: Add your command handler code here
	CReadDlg ReadDlg;
	ReadDlg.DoModal();
}


void CMenuDlg::OnMenuUpdate()
{
	// TODO: Add your command handler code here
	CUpdateDlg UpdateDlgObj(logged_in_email);
	UpdateDlgObj.DoModal();
}


void CMenuDlg::OnMenuDelete()
{
	// TODO: Add your command handler code here
	CDeleteDlg DeleteDlgObj;
	DeleteDlgObj.DoModal();
}
