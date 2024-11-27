// CRegisterDlg.cpp : implementation file
//

#include "pch.h"
#include "Login_Project.h"
#include "afxdialogex.h"
#include "CRegisterDlg.h"
#include "DBConnection.h"


#include <random>  // For secure random number generation
// CRegisterDlg dialog

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGISTER, pParent)
	, m_first_name(_T(""))
	, m_last_name(_T(""))
	, m_email(_T(""))
	, m_password(_T(""))
	, m_address(_T(""))
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIGNIN_FN, m_first_name);
	DDX_Text(pDX, IDC_EDIT_SIGNIN_LN, m_last_name);
	DDX_Text(pDX, IDC_EDIT_SIGNIN_EMAIL, m_email);
	DDX_Text(pDX, IDC_EDIT_SIGNIN_PASS, m_password);
	DDX_Text(pDX, IDC_EDIT_SIGNIN_ADDR, m_address);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegisterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRegisterDlg message handlers

bool CRegisterDlg::ValidatePassword(std::string& entered_password)
{
	// Check criteria: length >= 8, contains at least one uppercase, one digit, and one special character
	if (entered_password.length() < 8)
	{
		return false;
	}

	bool hasUpper = false, hasDigit = false, hasSpecial = false;
	//Loop through each character of the password
	for (char ch : entered_password)
	{
		if (isupper(ch))	hasUpper = true;
		else if (isdigit(ch))	hasDigit = true;
		else if (ispunct(ch))	hasSpecial = true;
	}

	// Password is valid if all conditions are met
	return hasUpper && hasDigit && hasSpecial;

}
void CRegisterDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	//create DBConnection object
	DBConnection DBConnObj(this);
	DBConnObj.ConnectToDB();

	// 2. Get the input from the fields
	GetDlgItem(IDC_EDIT_SIGNIN_FN)->GetWindowText(m_first_name);
	GetDlgItem(IDC_EDIT_SIGNIN_LN)->GetWindowText(m_last_name);
	GetDlgItem(IDC_EDIT_SIGNIN_EMAIL)->GetWindowText(m_email);
	GetDlgItem(IDC_EDIT_SIGNIN_PASS)->GetWindowText(m_password);
	GetDlgItem(IDC_EDIT_SIGNIN_ADDR)->GetWindowText(m_address);


	// Convert CString to std::string for database interaction
	std::string entered_FN = std::string(CT2CA(m_first_name));
	std::string entered_LN = std::string(CT2CA(m_last_name));
	std::string entered_email = std::string(CT2CA(m_email));
	std::string entered_password = std::string(CT2CA(m_password));
	std::string entered_address = std::string(CT2CA(m_address));

	//check if the fileds are empty
	if (entered_FN.empty() || entered_LN.empty() || entered_email.empty() || entered_password.empty() || entered_address.empty())
	{
		MessageBox(_T("Fill all the details"), _T("Error"), MB_OK);
		return;
	}

	//validate if the user exists through email
	if (DBConnObj.ValidateEmail(entered_email))
	{
		MessageBox(_T("Email already exists. Please login"), _T("Error"), MB_OK);
		return;
	}
	else
	{
		// Validate password criteria
		if (!ValidatePassword(entered_password))
		{
			MessageBox(_T("Password must be at least 8 characters long,one uppercase letter, one digit, and one special character."),
				_T("Invalid Password"), MB_ICONERROR);
			return;
		}
		else
		{
			// Register the user if validation is successful
			if (DBConnObj.RegisterUser(entered_FN, entered_LN, entered_email, entered_password, entered_address))
			{
				// If registered is successful, show a success message
				MessageBox(_T("User registered successfully!\n Login using registerd email and password"), _T("Info"), MB_OK);
				EndDialog(IDOK);  // Close the dialog on successful registration
			}
			else
			{
				// If login fails, show an error message
				MessageBox(_T("Failed to register"), _T("Error"), MB_ICONERROR);
			}
		}
	}	
}
