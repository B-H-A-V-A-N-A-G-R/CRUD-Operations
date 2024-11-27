#include "pch.h"
#include "DBConnection.h"

DBConnection::DBConnection(CDialogEx* dialog)
{
	_pDialog = dialog;
}

bool DBConnection::ConnectToDB()
{
	try
	{
		// 1. Get the MySQL driver instance
		driver = get_driver_instance();
		if (!driver) {
			std::cerr << "Error: Unable to get MySQL driver instance." << std::endl;
			return false;
		}

		sql::SQLString host("tcp://127.0.0.1:3306");
		sql::SQLString user("root");
		sql::SQLString pass("BMYSQL@2001");

		// 2. Establish a connection to the MySQL database
		conn = driver->connect(host, user, pass);
		if (!conn || !conn->isValid()) {
			std::cerr << "Error: Unable to establish a database connection." << std::endl;
			return false;
		}

		// 3. Set the target database
		conn->setSchema("login_project");
	}
	catch (sql::SQLException& e)
	{
		// Handle any SQL errors (e.g., wrong credentials, connection failure, etc.)
		//std::cerr << "Error during login: " << e.what() << std::endl;
		std::string except("# ERR: SQLException in ");
		except.append(__FILE__);
		except.append("(");
		except.append(__FUNCTION__);
		except.append(") on line ");
		//_itoa_s(__LINE__, ConfigBuff, 10);//Base 10 Decimal
		//except.append(ConfigBuff);
		except.append("\n");
		except.append("# ERR: ");
		except.append(e.what());
		except.append(" (MySQL error code: ");
		//_itoa_s(e.getErrorCode(), ConfigBuff, 10);//Base 10 Decimal
		//except.append(ConfigBuff);
		except.append(", SQLState: ");
		except.append(e.getSQLState());
		except.append(" )");
		except.append("\n");
		_pDialog->MessageBox(CString(except.c_str()));
		return false;
	}
}

bool DBConnection::ValidateUser(const std::string& entered_email, const std::string& entered_password)
{
	// 4. Prepare an SQL query to validate the credentials
	pstmt = conn->prepareStatement("SELECT * FROM users WHERE email=? AND password=?");

	// 5. Bind the user input (entered_email and entered_password) to the query
	pstmt->setString(1, entered_email);
	pstmt->setString(2, entered_password);

	// 6. Execute the query and get the result set
	std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

	// 7. Check if there is a matching record in the result set
	if (res->next())
	{
		// If a record exists, login is successful
		return true;
	}
	else
	{
		// If no matching record, login fails
		return false;
	}
}

bool DBConnection::RegisterUser(const std::string& entered_FN, const std::string& entered_LN,
	const std::string& entered_email, const std::string& entered_password, const std::string& entered_address)
{
	try {
		// 1. Prepare the SQL Insert query with placeholders
		pstmt = conn->prepareStatement(
			"INSERT INTO users (first_name, last_name, email, password, address) VALUES (?, ?, ?, ?, ?)"
		);

		// 2. Bind the user input to the query
		pstmt->setString(1, entered_FN);
		pstmt->setString(2, entered_LN);
		pstmt->setString(3, entered_email);
		pstmt->setString(4, entered_password);
		pstmt->setString(5, entered_address);

		// 3. Execute the insert query
		int rowsAffected = pstmt->executeUpdate();

		// 4. Check if the insert was successful
		if (rowsAffected > 0) {
			return true;  // User successfully registered
		}
		else {
			return false;  // Insert failed
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL error: " << e.what() << std::endl;
		return false;  // Registration failed due to exception
	}
}

bool DBConnection::ValidateEmail(const std::string& email)
{
	// 4. Prepare an SQL query to validate the credentials
	pstmt = conn->prepareStatement("SELECT * FROM users WHERE email=?");

	// 5. Bind the user input (entered_email and entered_password) to the query
	pstmt->setString(1, email);

	// 6. Execute the query and get the result set
	std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

	// 7. Check if there is a matching record in the result set
	if (res->next())
	{
		// If a email exists, return true 
		return true;
	}
	else
	{
		// If no matching email, return false
		return false;
	}
}

bool DBConnection::GetUserDetails(const std::string& email, std::tuple<std::string, std::string, std::string>& userDetails)
{
	try {
		// Prepare the SQL query to retrieve user details
		std::unique_ptr<sql::PreparedStatement> pstmt(
			conn->prepareStatement("SELECT first_name, last_name, address FROM users WHERE email = ?")
		);

		// Bind the email to the query
		pstmt->setString(1, email);

		// Execute the query
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		// If a matching record is found, extract the details
		if (res->next()) {
			std::string firstName = res->getString("first_name");
			std::string lastName = res->getString("last_name");
			std::string address = res->getString("address");

			// Store the details in the tuple
			userDetails = std::make_tuple(firstName, lastName, address);
			return true;
		}
		else {
			return false;  // No matching record found
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL error: " << e.what() << std::endl;
		return false;
	}
}

bool DBConnection::ValidatePassword(const std::string& entered_password,const std::string& user_email)
{
	// 4. Prepare an SQL query to validate the credentials
	pstmt = conn->prepareStatement("SELECT * FROM users WHERE email=? and password=?");

	// 5. Bind the user input (entered_password) to the query
	pstmt->setString(1, user_email);
	pstmt->setString(2, entered_password);

	// 6. Execute the query and get the result set
	std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

	// 7. Check if there is a matching record in the result set
	if (res->next())
	{
		// If a password exists, login is successful
		return true;
	}
	else
	{
		// If no matching password, login fails
		return false;
	}
}

bool DBConnection::UpdatePassword(const std::string& entered_password, const std::string& user_email)
{
	try {
		// 1. Prepare the SQL Update query with placeholders
		pstmt = conn->prepareStatement("UPDATE users SET password = ? WHERE email = ?");

		// 2. Bind the user input to the query
		pstmt->setString(1, entered_password);
		pstmt->setString(2, user_email);

		// 3. Execute the update query
		int rowsAffected = pstmt->executeUpdate();

		// 4. Check if the insert was successful
		if (rowsAffected > 0) {
			return true;  // Password updated successfully
		}
		else {
			return false;  //  Update failed
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL error: " << e.what() << std::endl;
		return false;  // Password Update failed due to exception
	}
}

bool DBConnection::DeleteUser(const std::string& entered_email)
{
	try {
		//1. Prepare SQL Delete query with placeholder
		pstmt = conn->prepareStatement("DELETE FROM users where email = ?");

		//2. Bind the user input to the query
		pstmt->setString(1, entered_email);

		//3.Execute the update query
		int rowsAffected = pstmt->executeUpdate();

		//4. Check if the deletion was successful
		if (rowsAffected > 0)
			return true;  // User deleted successfully
		else
			return false; //// No user found with the given email

	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL error: " << e.what() << std::endl;
		return false; //Deletion failed due to exception
	}

}