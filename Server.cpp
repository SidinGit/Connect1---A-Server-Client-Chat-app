/*This will work perfectly in the same system but if you want to make it work for two different systems, follow these comments*/
// 1. Type ipconfig in cmd to get ur ip address under the wifi-adapter ipv4 section
// 2. InetPton(AF_INET, _T("0.0.0.0"), &service.sin_addr.s_addr);  // under step 3: Bind to all available interfaces
// 3. #pragma comment(lib, "ws2_32.lib") add this after #include"iostream"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include "iostream"

using namespace std;

int main(int argc, char* argv[])
{   
    cout<<"========== SERVER ==========\n";
    cout<<"\n==== Step 1: Set up DLL ====\n";
    SOCKET serverSocket=INVALID_SOCKET, acceptSocket = INVALID_SOCKET;
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
        cout << "The Winsock dll found!!\n" ;
        cout << "The Status: " << wsaData.szSystemStatus << '\n';
    }
    cout << "\n=== Step 2: Set up SOCKET ===\n";
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Error at socket():" << WSAGetLastError() << '\n';
        WSACleanup();
        return 0;
    }
    else {
        cout << "socket() is OK!!\n";
    }
    cout << "\n==== Step 3: Bind SOCKET ====\n";
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    service.sin_port = htons(port);
    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cout << "bind() failed: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else {
        cout << "bind() is OK!!\n";
    }
    cout << "\n== Step 4: Initiate Listen ==\n";
    if (listen(serverSocket, 1) == SOCKET_ERROR)
        cout << "listen(): Error listening on socket" << WSAGetLastError() << '\n';
    else
        cout << "listen() is OK!!, I am waiting for connections...\n";
    
    cout << "\n== Step 5: Accept Connection ==\n";
    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET) {
        cout << "accept failed: " << WSAGetLastError() << '\n';
        WSACleanup();
        return -1;
    }
    cout << "Accepted Connection!!!\n";
    cout << "\n== Step 6: Chat to the Client (press 'q' to quit chat) ==\n";
    char buffer[200];
    while (true) {
        int byteCount = recv(acceptSocket, buffer, sizeof(buffer), 0);
        if (byteCount > 0) {
            buffer[byteCount] = '\0'; //received string must end with null
            cout << "Client : " << buffer << '\n';
            if (strcmp(buffer,"q")==0) {
                cout << "Client terminated the chat!!!\n";
                break; 
            }
        }
        else
            WSACleanup();
        cout << "Server : ";
        cin.getline(buffer, sizeof(buffer));
        byteCount = send(acceptSocket, buffer, strlen(buffer), 0);
        if (byteCount > 0)
        {
            buffer[byteCount] = '\0';
            if (strcmp(buffer, "q") == 0) {
                cout << "Server is terminating the chat...\n";
                break;
            }
            else {
                cout << "Message sent to client!!\n";
            }
        }
        else
            WSACleanup();
    }
    cout << "\n===== Step 7: Close Socket =====\n";
    system("pause");
    closesocket(acceptSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}