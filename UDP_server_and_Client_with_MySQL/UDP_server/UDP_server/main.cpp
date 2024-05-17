#include <iostream>
#include <string>
#include <WinSock2.h>
#include <mysql.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define udpPortNo 4400 // assign a udp host number *use between 1024 to 65535
#define host "localhost" // using localhost to connect with MySql
#define mySqlUsername "root" // mySqlUsername **typically named as "root"
#define muSqlPassword "your_password_here" //your MySql "root" user password here
#define databaseName "userInfoDb" // database name to use

// Struct for user information
typedef struct user_info {
    int user_id;
    char first_name[80];
    char last_name[80];
    char email[80];
    char subscription_time[20]; // YYYY-MM-DD HH:MM:SS
    char last_entry_time[20];   
    long long int view_time; // to match to mysql bigint type
} user_info;

// Helper function to establish MySQL connection
MYSQL* connectMySQL() {
    MYSQL* conn = mysql_init(NULL);
    if (conn == NULL) {
        cerr << "Error initializing MySQL connection" << endl;
        return nullptr;
    }

    if (!mysql_real_connect(conn, host, mySqlUsername, muSqlPassword, databaseName, 3306, NULL, 0)) {
        cerr << "Error connecting to MySQL: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return nullptr;
    }
    else {
        cout << "Logged in to MySQL..." << endl << endl;
    }

    return conn;
}

// find function to search for "test" in email address
bool findInEmail(const string& email, const string& searchString) {
    size_t emailLen = email.length();
    size_t testLen = searchString.length();

    // Iterate through the email string
    for (int i = 0; i <= emailLen - testLen; ++i) {
        bool found = true;
        // Check if the current substring matches "test"
        for (int j = 0; j < testLen; ++j) {
            if (email[i + j] != searchString[j]) {
                found = false;
                break;
            }
        }
        // If "test" is found, return true
        if (found) {
            return true;
        }
    }
    // If "test" is not found, return false
    return false;
}

// Custom function to check if the email ends with ".com"
bool isEmailEndsWithCom(const string& email) {
    const string comString = ".com";
    size_t emailLen = email.length();
    size_t comLen = comString.length();

    // Check if email length is less than 7 characters as (x@y.com)
    if (emailLen < 7) {
        return false; // Minimum length is not 7
    }

    // function to Check if the email is long enough to contain ".com"
    if (emailLen >= comLen) {
        // Compare the end of the email with ".com"
        for (size_t i = emailLen - comLen, j = 0; i < emailLen; ++i, ++j) {
            if (email[i] != comString[j]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

// function to check if the email contain "@" symbol at a valid position
bool isEmailContainAtSymbol(const string& email) {
    size_t emailLen = email.length();

    // Check if email length is less than 7 characters as (x@y.com)
    if (emailLen < 7) {
        return false; // Minimum length is not at least 7
    }

    bool foundAt = false;
    // Iterate through the email string except the first char and "y.com"
    for (size_t i = 1; i < emailLen - 5; ++i) {
        if (email[i] == '@') {
            if (foundAt) {
                return false; // More than one "@" symbol
            }
            else {
                foundAt = true; // Set foundAt flag to true for first occurrence of "@"
            }
        }
    }

    // check if "@" symbol was not found at a valid position
    if (!foundAt) {
        return false; // "@" symbol not found
    }

    return true; // Email contain "@"
}

// Custom function to validate email
bool isValidEmail(const string& email) {
    // Check validation criteria
    const string searchString = "gmail";
    if (!findInEmail(email, searchString) || !isEmailContainAtSymbol(email) || !isEmailEndsWithCom(email)) {
        return false; // Failed validation
    }

    return true; // Email is valid
}

// Function for receiving data (UDP receiver)
void receiveData() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return;
    }

    // Create UDP socket
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        cerr << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return;
    }
    else cout << "Socket Created." << endl << endl;

    // Bind socket to address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(udpPortNo);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error binding socket: " << WSAGetLastError() << endl;
        closesocket(sockfd);
        WSACleanup();
        return;
    }
    else cout << "Socket bound" << endl << endl;

    // MySQL connection
    MYSQL* conn = connectMySQL();
    if (!conn) {
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    // Receive user_info structures and store valid information into MySQL database
    user_info data;
    struct sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    while (true) {
        int bytesReceived = recvfrom(sockfd, reinterpret_cast<char*>(&data), sizeof(data), 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (bytesReceived == SOCKET_ERROR) {
            cerr << "Error receiving data: " << WSAGetLastError() << endl;
            continue;
        }
        else
            cout << "Data received successfully" << endl;

        // Validate email address
        if (!isValidEmail(data.email)) {
            cerr << "Invalid email address: " << data.email << endl << endl;;
            continue; // Skip processing invalid email addresses
        }

        // Insert data into MySQL database
        string query = "INSERT INTO UserInfoDB.userInfo (user_id, first_name, last_name, email, subscription_time, last_entry_time, view_time) VALUES (" +
            to_string(data.user_id) + ", '" + data.first_name + "', '" + data.last_name + "', '" + data.email + "', '" +
            data.subscription_time + "', '" + data.last_entry_time + "', " + to_string(data.view_time) + ")";
        if (mysql_query(conn, query.c_str())) {
            cerr << "Error inserting data into MySQL: " << mysql_error(conn) << endl << endl;
        }
        else {
            cout << "Inserted data into MySQL" << endl << endl;
        }
    }

    // Close MySQL connection
    mysql_close(conn);

    // Close socket
    closesocket(sockfd);

    // Cleanup Winsock
    WSACleanup();
}

int main() {
    // Call receiveData function to receive and store data
    receiveData();

    return 0;
}
