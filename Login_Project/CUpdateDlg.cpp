// CUpdateDlg.cpp : implementation file
//

#include "pch.h"
#include "Login_Project.h"
#include "afxdialogex.h"
#include "CUpdateDlg.h"
#include "DBConnection.h"
#include "Login_ProjectDlg.h"


// CUpdateDlg dialog

IMPLEMENT_DYNAMIC(CUpdateDlg, CDialogEx)

CUpdateDlg::CUpdateDlg(const std::string& email, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UPDATE, pParent), logged_in_email(email)
	, m_old_pass(_T(""))
	, m_new_pass(_T(""))
	, m_confirm_pass(_T(""))
{

}

CUpdateDlg::~CUpdateDlg()
{
}

void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Update_Oldpass, m_old_pass);
	DDX_Text(pDX, IDC_EDIT_Update_Newpass, m_new_pass);
	DDX_Text(pDX, IDC_EDIT_Update_Confirmpass, m_confirm_pass);
}


BEGIN_MESSAGE_MAP(CUpdateDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUpdateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUpdateDlg message handlers


void CUpdateDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();

	//1. connect to DB
	DBConnection DBConObj(this);
	DBConObj.ConnectToDB();

	// 2. Get the input from the fields
	GetDlgItem(IDC_EDIT_Update_Oldpass)->GetWindowText(m_old_pass);
	GetDlgItem(IDC_EDIT_Update_Newpass)->GetWindowText(m_new_pass);
	GetDlgItem(IDC_EDIT_Update_Confirmpass)->GetWindowText(m_confirm_pass);

	// Convert CString to std::string for database interaction
	std::string entered_old_pass = std::string(CT2CA(m_old_pass));
	std::string entered_new_pass = std::string(CT2CA(m_new_pass));
	std::string entered_confirm_pass = std::string(CT2CA(m_confirm_pass));

	//check if any field is empty
	if (entered_old_pass.empty() || entered_new_pass.empty() || entered_confirm_pass.empty())
	{
		MessageBox(_T("Please enter all the fields!"), _T("Error"), MB_ICONERROR);
	}
	else
	{
		//validate if the old password matches with password stored in DB of logged in email
		if (DBConObj.ValidatePassword(entered_old_pass, logged_in_email))
		{
			//2. validate if the new password and confirm password are equal 
			if (entered_new_pass.compare(entered_confirm_pass) == 0) 
			{
				//change the password in the DB to new password of the user
				if (DBConObj.UpdatePassword(entered_new_pass, logged_in_email))
				{
					MessageBox(_T("Password changed successfully"), _T("Info"), MB_OK);
					EndDialog(IDOK);  // Close the dialog
				}
				else
				{
					AfxMessageBox(_T("Failed to change password. Try again."));
				}
			}
			else
			{
				AfxMessageBox(_T("New password and confirm password does not match."));

				//clear the fields
				SetDlgItemText(IDC_EDIT_Update_Newpass, _T(""));
				SetDlgItemText(IDC_EDIT_Update_Confirmpass, _T(""));
			}
		}
		else
		{
			// If password is incorrect, show an error message
			MessageBox(_T("Incorrect Password"), _T("Error"), MB_ICONERROR);

			//empty the old password field
			SetDlgItemText(IDC_EDIT_Update_Oldpass, _T(""));
		}
	}
}
