#pragma once
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
//#include <Login_ProjectDlg.h>

class DBConnection
{
public:
	DBConnection(CDialogEx* dialog);
	bool ConnectToDB();
	bool ValidateUser(const std::string& entered_email, const std::string& entered_password);
	bool RegisterUser(const std::string& entered_FN, const std::string& entered_LN,
		const std::string& entered_email, const std::string& entered_password, const std::string& entered_address);
	bool ValidateEmail(const std::string& email);
	bool GetUserDetails(const std::string& email, std::tuple<std::string, std::string, std::string>& userDetails);
	bool ValidatePassword(const std::string& entered_password, const std::string& user_email);
	bool UpdatePassword(const std::string& entered_password, const std::string& user_email);
	bool DeleteUser(const std::string& entered_email);

private:
	CDialogEx* _pDialog;
	sql::Driver* driver;
	sql::PreparedStatement* pstmt;
	sql::Connection* conn;
};

