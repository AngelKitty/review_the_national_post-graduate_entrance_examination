#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32")

char Buff[2048];
void overflow(char * s,int size)
{
    char s1[50];
    printf("receive %d bytes",size);
    s[size]=0;
    strcpy(s1,s);
}

int main()
{
    WSADATA wsa;
    SOCKET listenFD;
    int ret;
    char asd[2048];

    WSAStartup(MAKEWORD(2,2),&wsa);

    listenFD = WSASocket(2,1,0,0,0,0);

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = INADDR_ANY;
    ret=bind(listenFD,(sockaddr *)&server,sizeof(server));
    ret=listen(listenFD,2);

    int iAddrSize = sizeof(server);
    SOCKET clientFD=accept(listenFD,(sockaddr *)&server,&iAddrSize);
    unsigned long lBytesRead;
    while(1)    {
            lBytesRead=recv(clientFD,Buff,2048,0);
            if(lBytesRead<=0)    break;
            
            printf("\nfd = %x\n", clientFD);

            overflow(Buff,lBytesRead);

            ret=send(clientFD,Buff,lBytesRead,0);
            if(ret<=0)    break;

    }
    WSACleanup();
    return 0;
}
