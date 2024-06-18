/*This will work perfectly in the same system but if you want to make it work for two different systems, follow these comments*/
// 1. Type ipconfig in cmd to get ur ip address under the wifi-adapter ipv4 section
// 2. InetPton(AF_INET, _T("Server_IP_Address_Here"), &clientService.sin_addr.s_addr);  //under step 3:  Replace with the actual server IP address
// 3. #pragma comment(lib, "ws2_32.lib") add this below #include "iostream"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include "iostream"

using namespace std;

int main()
{
    cout << "========== CLIENT ==========\n";
    cout << "\n==== Step 1: Set up DLL ====\n";
    SOCKET clientSocket = INVALID_SOCKET;
    int port = 55555;
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0) {
        cout << "The Winsock dll not found!\n";
        return 0;
    }
    else {
        cout << "The Winsock dll found!!\n";
        cout << "The Status: " << wsaData.szSystemStatus << '\n';
    }
    cout << "\n=== Step 2: Set up SOCKET ===\n";
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error at socket():" << WSAGetLastError() << '\n';
        WSACleanup();
        return 0;
    }
    else {
        cout << "socket() is OK!!\n";
    }
    cout << "\n== Step 3: Connect with SERVER ==\n";
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);
    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        cout << "Client: connect() - Failed to connect!\n";
        WSACleanup();
        return 0;
    }
    else {
        cout << "Client: connnect() is OK!!\n";
        cout << " Client: Can start sending and receiving data...\n";
    }
    cout << "\n== Step 4: Chat to the SERVER (press 'q' to quit chat) ==\n";
    char buffer[200];
    while (true) {
        cout << "Client : ";
        cin.getline(buffer, sizeof(buffer));
        int byteCount = send(clientSocket, buffer, strlen(buffer), 0);
        if (byteCount > 0)
        {
            buffer[byteCount] = '\0';
            if (strcmp(buffer, "q") == 0) {
                cout << "Client is terminating the chat...\n";
                break;
            }
            else {
                cout << "Message sent to server!!\n";
            }
        }
        else
            WSACleanup();
        byteCount = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (byteCount > 0) {
            buffer[byteCount] = '\0'; //received string must end with null
            cout << "Server : " << buffer << '\n';
            if (strcmp(buffer,"q")==0) {
                cout << "Server terminated the chat!!!\n";
                break; 
            }
        }
        else
            WSACleanup();
    }
    cout << "\n===== Step 5: Close Socket =====\n";
    system("pause");
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
