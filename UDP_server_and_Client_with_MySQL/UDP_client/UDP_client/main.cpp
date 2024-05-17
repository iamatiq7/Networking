#include <iostream>
#include <string>
#include<Ws2tcpip.h>
#include <WinSock2.h>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#pragma comment(lib, "Ws2_32.lib")
//to avoid warning using localtime() is unsafe, try using localtime_s() 
#pragma warning(disable : 4996)

using namespace std;

#define udpPortNo 4400

// Struct for user information
typedef struct user_info {
    int user_id;
    char first_name[80];
    char last_name[80];
    char email[80];
    char subscription_time[20]; 
    char last_entry_time[20];   // YYYY-MM-DD HH:MM:SS
    long long int view_time; 
} user_info;

//Get the current Date and Time from the system and convert it to string fromat
std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    time_t now_c = std::chrono::system_clock::to_time_t(now);
    tm* tm_struct = std::localtime(&now_c);

    // to match format YYYY-MM-DD HH:MM:SS with Mysql
    std::stringstream ss;
    ss << put_time(tm_struct, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}


// Function to send data
void sendData() {
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

    // Server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(udpPortNo); // Server's UDP port
    InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr.s_addr);

    // Sample user_info data
    string currentDateTime = getCurrentDateTime();
    //cout << "Current Date and Time: " << currentDateTime << std::endl;
    user_info data;
    data.user_id = 5;
    strcpy_s(data.first_name, "John");
    strcpy_s(data.last_name, "Doe");
    strcpy_s(data.email, "john.doe@gmail.com"); // Valid email address
    strcpy_s(data.subscription_time, "2023-04-25 12:00:00");
    strcpy_s(data.last_entry_time, currentDateTime.c_str());
    data.view_time = 9223372036854775807;

    // Send multiple instances of user_info structure
    for (int i = 0; i < 5; ++i) {
        int bytesSent = sendto(sockfd, reinterpret_cast<const char*>(&data), sizeof(data), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (bytesSent == SOCKET_ERROR) {
            cerr << "Error sending data: " << WSAGetLastError() << endl;
            closesocket(sockfd);
            WSACleanup();
            return;
        }
        cout << "Sent data " << i + 1 << endl;
        Sleep(1000); // Wait 1 second between sends
    }

    // Close socket
    closesocket(sockfd);

    // Cleanup Winsock
    WSACleanup();
}

int main() {
    // Call sendData function to send data
    sendData();

    return 0;
}
