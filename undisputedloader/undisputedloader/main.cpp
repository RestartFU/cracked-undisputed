#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <windows.h>
#pragma comment (lib, "Ws2_32.lib")

const char* content12 = "HTTP/1.1 200 OK\nContent-Length: 437\nContent-Type: text/plain; charset=utf-8\n\n{"reach_base_addr" : 84671456,"reach_nop_addr" : 6084600,"timer_addr" : 81664600,"velocity" : {"detour" : "f3,f,10,2,f3,f,59,5,f4,f,0,0,f3,41,f,11,40,-2,f3,f,10,42,4,f3,F,59,5,e5,f,0,0,f3,41,f,11,40,-2,f3,f,10,42,8,f3,f,59,5,ce,f,0,0,f3,41,f,11,40,-2,e9,-1,-1,-1,-1","hook" : "e9,-1,-1,-1,-1,90,f,1f,0,f,1f,40,0,f,1f,0,f,1f,40,0","sig" : "74,19,8b,2,41,89,40,-1,8b,42,4,41,89,40,-1,8b,42,8,41,89,40,-1,48,83,c4,28,c3,e8,-1,-1,-1,-1,cc,cc","vars" : "2"}}";

// sparkloader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <thread>
void listen_and_serve();
void inject(DWORD procID)
{

    const char* dllPath = "dll.dll";

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procID);
    if (hProc && hProc != INVALID_HANDLE_VALUE)
    {
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);

        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

        if (hThread)
        {
            CloseHandle(hThread);
        }
    }
    if (hProc)
    {
        CloseHandle(hProc);
    }
}

int main()
{
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)listen_and_serve, NULL, NULL, NULL);
    Sleep(1000);

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    std::cout << "cracked in 4 seconds, stop buying skidded clients.\n";
    if (CreateProcessA(
        "undisputed.exe",
        NULL,
        NULL,
        NULL,
        FALSE,
        CREATE_SUSPENDED,
        NULL,
        NULL,
        &si,
        &pi))
    {
        Sleep(2000);
        inject(pi.dwProcessId);
        MessageBoxA(NULL, "cracked w/<3 by restartfu", "cracked", MB_ICONWARNING);
        Sleep(1000);
        // Close process and thread handles
        ResumeThread(pi.hThread);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        while (true)
        {
            Sleep(1);
        }
    }
}

static void listen_and_serve() {
    // Initialize WSA variables
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    // Check for initialization success
    if (wsaerr != 0) {
        std::cout << "The Winsock dll not found!" << std::endl;
        exit(0);
    }

    // Create a socket
    SOCKET serverSocket;
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(0);
    }

    // Bind the socket to an IP address and port number
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with your desired IP address
    service.sin_port = htons(1337);  // Choose a port number

    // Use the bind function
    if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
        std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(0);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cout << "listen(): Error listening on socket: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "Server Listening on port 1337..." << std::endl;
    }

    while (true) {
        SOCKET acceptSocket;
        acceptSocket = accept(serverSocket, nullptr, nullptr);

        // Check for successful connection
        if (acceptSocket == INVALID_SOCKET) {
            std::cout << "accept failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            exit(0);
        }

        recv(acceptSocket, NULL, 0, 0);
        printf("Sent Hook Data\n");
        send(acceptSocket, content12, strlen(content12), 0);
        MessageBoxA(NULL, "press X, A and B to launch the client", "keybinds", MB_ICONWARNING);
        ExitProcess(0);
    }
}
