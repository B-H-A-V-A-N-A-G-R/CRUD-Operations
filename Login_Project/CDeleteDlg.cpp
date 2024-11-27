// CDeleteDlg.cpp : implementation file
//

#include "pch.h"
#include "Login_Project.h"
#include "afxdialogex.h"
#include "CDeleteDlg.h"
#include "DBConnection.h"

// CDeleteDlg dialog

IMPLEMENT_DYNAMIC(CDeleteDlg, CDialogEx)

CDeleteDlg::CDeleteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DELETE, pParent)
	, m_email(_T(""))
{

}

CDeleteDlg::~CDeleteDlg()
{
}

void CDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DELETE_EMAIL, m_email);
}


BEGIN_MESSAGE_MAP(CDeleteDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDeleteDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDeleteDlg message handlers


void CDeleteDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//1. Connect to DB
	DBConnection DBConObj(this);
	DBConObj.ConnectToDB();

	//2. Get user input from the fields
	GetDlgItem(IDC_EDIT_DELETE_EMAIL)->GetWindowText(m_email);

	//3. Convert CString to std::string for DB interaction
	std::string entered_email = std::string(CT2CA(m_email));

	if(entered_email.empty())
		AfxMessageBox(_T("Please enter the email"));
	else
	{
		// Display confirmation dialog with Yes/No buttons
		int response = AfxMessageBox(
			_T("Are you sure you want to delete this user?"),
			MB_YESNO | MB_ICONQUESTION
		);

		// If the user clicks Yes, proceed with the deletion
		if (response == IDYES)
		{
			//4. Verify and Delete the user
			if (DBConObj.DeleteUser(entered_email))
			{
				// If deletion is successful, show a success message
				MessageBox(_T("User Deleted successfully!"), _T("Info"), MB_OK);
				EndDialog(IDOK);  // Close the dialog on successful deletion
			}
			else
			{
				// If login fails, show an error message
				AfxMessageBox(_T("Invalid email. Please try again."));

				// Clear the email field
				SetDlgItemText(IDC_EDIT_DELETE_EMAIL, _T(""));
			}
		}
	}
}
