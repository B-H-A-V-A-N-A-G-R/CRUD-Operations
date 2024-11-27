// CReadDlg.cpp : implementation file
//

#include "pch.h"
#include "Login_Project.h"
#include "DBConnection.h"
#include "afxdialogex.h"
#include "CReadDlg.h"


// CReadDlg dialog

IMPLEMENT_DYNAMIC(CReadDlg, CDialogEx)

CReadDlg::CReadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_READ, pParent)
	, m_email(_T(""))
	, m_first_name(_T(""))
	, m_last_name(_T(""))
	, m_address(_T(""))
{

}

CReadDlg::~CReadDlg()
{
}

void CReadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_READ_EMAIL, m_email);
	DDX_Text(pDX, IDC_EDIT_READ_FN, m_first_name);
	DDX_Text(pDX, IDC_EDIT_READ_LN, m_last_name);
	DDX_Text(pDX, IDC_EDIT_READ_ADDR, m_address);
}


BEGIN_MESSAGE_MAP(CReadDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CReadDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CReadDlg message handlers


void CReadDlg::OnBnClickedOk()
{
	//clear the display fields
	SetDlgItemText(IDC_EDIT_READ_FN, _T(""));
	SetDlgItemText(IDC_EDIT_READ_LN, _T(""));
	SetDlgItemText(IDC_EDIT_READ_ADDR, _T(""));

	//1. connect to DB 
	DBConnection DBConObj(this);
	DBConObj.ConnectToDB();

	// 2. Get the input from the email fields
	GetDlgItem(IDC_EDIT_READ_EMAIL)->GetWindowText(m_email);

	// 3. Convert CString to std::string for database interaction
	std::string entered_email = std::string(CT2CA(m_email));

	if (entered_email.empty())
	{
		MessageBox(_T("Please Enter the email!"), _T("Error"), MB_ICONERROR);
	}
	else
	{
		// 4. Validate the email
		if (DBConObj.ValidateEmail(entered_email))
		{
			// Retrieve user details from the database
			std::tuple<std::string, std::string, std::string> userDetails;
			if (DBConObj.GetUserDetails(entered_email, userDetails))
			{
				//try to display the user details
				// Extract data and assign to member variables
				m_first_name = CString(std::get<0>(userDetails).c_str());
				m_last_name = CString(std::get<1>(userDetails).c_str());
				m_address = CString(std::get<2>(userDetails).c_str());

				// Update the edit controls with the new values
				UpdateData(FALSE);
			}
			else
			{
				MessageBox(_T("Failed to retrieve user details"), _T("Error"), MB_ICONERROR);
			}

		}
		else
		{
			// If login fails, show an error message
			MessageBox(_T("Email not found"), _T("Error"), MB_ICONERROR);

			//clear the email input field
			SetDlgItemText(IDC_EDIT_READ_EMAIL, _T(""));
		}
	}
}
