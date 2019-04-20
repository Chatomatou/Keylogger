#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 1455
#define IP "127.0.0.1"
#define PATH "./"
#define BUFFER_SIZE 255


INT APIENTRY WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
    WSADATA data;
    SOCKET sockfd = 0;
    SOCKADDR_IN sockfd_addr_in;

    SOCKET csockfd = 0;
    SOCKADDR_IN csockfd_addr_in;
    INT caddr_len = sizeof(csockfd_addr_in);
    CHAR buffer[BUFFER_SIZE];

    FILE* file = NULL;
 
    ZeroMemory(&data, sizeof(data));
    ZeroMemory(&sockfd_addr_in, sizeof(sockfd_addr_in));
    ZeroMemory(&csockfd_addr_in, sizeof(sockfd_addr_in));
    ZeroMemory(buffer, sizeof(buffer));

    if(WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        MessageBoxEx(NULL, "Error : Windowns socket can't be initialize !", "Initalize", MB_ICONERROR | MB_OK, 0);
        return EXIT_FAILURE;
    }
 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == INVALID_SOCKET)
    {
        MessageBoxEx(NULL, "Error : Socket can't be created !", "Socket Creation", MB_ICONERROR | MB_OK, 0);
        return EXIT_FAILURE;
    }
    sockfd_addr_in.sin_family = AF_INET;
    sockfd_addr_in.sin_port = htons(PORT);
    sockfd_addr_in.sin_addr.s_addr = inet_addr(IP);

    if(bind(sockfd, (SOCKADDR*)& sockfd_addr_in, sizeof(sockfd_addr_in)) == SOCKET_ERROR)
    {
        MessageBoxEx(NULL, "Error : Socket can't be bind in the own context !", "Bind", MB_ICONERROR | MB_OK, 0);
        closesocket(sockfd);  
        WSACleanup();      
        return EXIT_FAILURE;
    }

    listen(sockfd, 5); 
 
    printf("Listen...\n");

    csockfd = accept(sockfd, (SOCKADDR*)& csockfd_addr_in, &caddr_len);

    if(csockfd == INVALID_SOCKET)
    {
        MessageBoxEx(NULL, "Error : Socket connect error", "connect", MB_ICONERROR | MB_OK, 0);
        closesocket(sockfd);  
        WSACleanup();      
        return EXIT_FAILURE;
    }

    printf("[+] User added : %s:%d\n", inet_ntoa(sockfd_addr_in.sin_addr), htons(csockfd_addr_in.sin_port));
    
    while(1)
    {
        if(recv(csockfd, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR)
        {
            MessageBoxEx(NULL, "Error : Send data failure !", "Send", MB_ICONERROR | MB_OK, 0);
            closesocket(csockfd);
            closesocket(sockfd);  
            WSACleanup();      
            return EXIT_FAILURE;
        }
        for(size_t i = 0; i < strlen(buffer) - 1; i++)
        {
            printf("%c", buffer[i]);
        }
        ZeroMemory(buffer, BUFFER_SIZE);

    }
    
    closesocket(csockfd);
    closesocket(sockfd);  
    WSACleanup();      


    return EXIT_SUCCESS;
}