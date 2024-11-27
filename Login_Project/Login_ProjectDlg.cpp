
// Login_ProjectDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Login_Project.h"
#include "Login_ProjectDlg.h"
#include "CMenuDlg.h"
#include "CRegisterDlg.h"
#include "afxdialogex.h"
#include "DBConnection.h" //include DBConnection class

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoginProjectDlg dialog



CLoginProjectDlg::CLoginProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN_PROJECT_DIALOG, pParent)
	, m_email(_T(""))
	, m_password(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoginProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_LOGIN_EMAIL, m_email);
	DDX_Text(pDX, IDC_EDIT_LOGIN_PASS, m_password);
}

BEGIN_MESSAGE_MAP(CLoginProjectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLoginProjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SIGNIN, &CLoginProjectDlg::OnBnClickedButtonSignin)
END_MESSAGE_MAP()


// CLoginProjectDlg message handlers

BOOL CLoginProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoginProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoginProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLoginProjectDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	//1. create DDConnection object
	DBConnection DBConObj(this);
	DBConObj.ConnectToDB();

	// 2. Get the input from the email and password fields
	GetDlgItem(IDC_EDIT_LOGIN_EMAIL)->GetWindowText(m_email);
	GetDlgItem(IDC_EDIT_LOGIN_PASS)->GetWindowText(m_password);

	// 3. Convert CString to std::string for database interaction
	std::string entered_email = std::string(CT2CA(m_email));
	std::string entered_password = std::string(CT2CA(m_password));

	// 4. check if there are any empty fields
	if (entered_email.empty() && entered_password.empty())
	{
		MessageBox(_T("Enter email and password!"), _T("Info"), MB_OK);
	}
	else if (entered_email.empty())
	{
		MessageBox(_T("Enter email!"), _T("Info"), MB_OK);
	}
	else if (entered_password.empty())
	{
		MessageBox(_T("Enter password"), _T("Info"), MB_OK);
	}
	else
	{
		// 4. Check if the email exists in DB
		if (!DBConObj.ValidateEmail(entered_email))
		{
			MessageBox(_T("Email is not registered! Please Sign in"), _T("Info"), MB_OK);

			//clear the input fields
			SetDlgItemText(IDC_EDIT_LOGIN_EMAIL, _T(""));
			SetDlgItemText(IDC_EDIT_LOGIN_PASS, _T(""));
		}
		else
		{
			// 3. Call the function to validate the credentials
			if (DBConObj.ValidateUser(entered_email, entered_password))
			{
				// If login is successful, show a success message
				//MessageBox(_T("Login Successful!"), _T("Info"), MB_OK);

				EndDialog(IDOK); //close the dialog after successful login

				// Optionally, proceed to another window or perform some action
				CMenuDlg MenuDlgObj;
				MenuDlgObj.logged_in_email = entered_email;  // Store email in the menu dialog
				MenuDlgObj.DoModal();
			}
			else
			{
				// If login fails, show an error message
				MessageBox(_T("Invalid username or password."), _T("Error"), MB_ICONERROR);
			}
		}
	}
}


void CLoginProjectDlg::OnBnClickedButtonSignin()
{
	// TODO: Add your control notification handler code here

	//open register dialog
	CRegisterDlg RegisterDlgObj;
	RegisterDlgObj.DoModal();
}
