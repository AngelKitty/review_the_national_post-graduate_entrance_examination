/*******************************************************
《加密与解密》第三版配套实例
5.6 网络验证
code by riijj
(c)  看雪软件安全网站 www.pediy.com 2000-2008
********************************************************/

#include "windows.h"
#include <stdio.h>
#include <time.h>


int checkkey(char *name, char *key);

int main()
{

BYTE bufResult[] = 
{
0x14, 0x15, 0x00, 0x00, 0xD5, 0x07, 0x09, 0x00 , 0x01, 0x00, 0x13, 0x00, 0x03, 0x00, 0x00, 0x00, 
0x11, 0x00, 0xBB, 0x00, 0x91, 0x53, 0x01 , 0x00, 0x21, 0x61, 0x00, 0x00, 0x1E, 0x00, 0xC5, 0x0B, 
0xC9, 0x0B, 0x30, 0xBD, 0x97, 0x88 , 0x8E, 0x00, 0xBE, 0x19, 0x00, 0x00, 0xD4, 0x12, 0x00, 0x00, 
0x6F, 0x35, 0xE1, 0x52, 0x51 , 0xA4, 0xB7, 0x07, 0x76, 0xE7, 0xD4, 0xA1, 0x43, 0x98, 0x88, 0xd6, 
0x45, 0xff, 0xc6, 0xb1 , 0x43, 0x66, 0x77, 0x98, 0x77, 0x67, 0x54, 0x66, 0x77, 0x53, 0x64, 0x58, 
0x6C, 0x66, 0x05 , 0x08, 0x60, 0x16, 0x30, 0xb4, 0xaa, 0x54
};

	struct   sockaddr_in name;
	struct   sockaddr_in name2;
	SOCKET s;
	SOCKET s2;
	int len;
	WSADATA wsa;
	FD_SET fd;

	srand(time(0));

	printf( "****  riijj crackme net-2 server  ****\n");

	WSAStartup( MAKEWORD( 2, 2 ), &wsa);

	if( ( s = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)
	{
		printf( "Error: socket creation failed\n");
		printf( "Code: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	name.sin_port = htons(8877);

	if( bind( s,  (struct sockaddr *)&name, sizeof(name) ) == SOCKET_ERROR)
	{
		printf( "Error: socket error\n");
		printf( "Code: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	if( listen( s, SOMAXCONN) == SOCKET_ERROR)
	{
		printf( "Error: socket listening error\n");
		printf( "Code: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	while( 1)
	{
		FD_ZERO( &fd);
		FD_SET( s, &fd);

		printf( "Waiting for connection...");

		select( 0, &fd, 0, 0, 0);

		len = sizeof(name2);
		s2 = accept( s, (struct sockaddr *)&name2, &len);
		
		if( s2 == INVALID_SOCKET)
		{
			printf( "\nError : connection failed\n");
			printf( "Code: %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		else
		{
			char bufname[51];
			char bufkey[51];
			char bufEncrypt[200];
			int nameLength, keyLength;
			int numRecv;
			int numResultLength = 0x5a;
			BYTE ran_K;
			BYTE bufResultEncrypt[0x5a];
			int i;

			numRecv = recv( s2, bufEncrypt, 200, 0);

			for( i=0; i< numRecv; i++)
			{
				bufEncrypt[i] = bufEncrypt[i] ^ 0xa6;  //  XOR decrypt
			}

			nameLength = bufEncrypt[0];
			keyLength = bufEncrypt[1];
			ran_K = bufEncrypt[2];
			
			memset( bufname, 0, sizeof(bufname));
			memset( bufkey, 0, sizeof(bufkey));
			memcpy( bufname, bufEncrypt+3, nameLength);
			memcpy( bufkey, bufEncrypt+3+nameLength, keyLength);

			bufResult[0xe] = ran_K;

			if( strcmp( bufname, "pediy") == 0 &&
				strcmp( bufkey, "www.pediy.com") == 0)
			{
				for( i=0; i<numResultLength; i++)
				{
					bufResultEncrypt[i] = bufResult[i] ^ 0x6e;					
				}
				send( s2, bufResultEncrypt, sizeof(bufResultEncrypt) , 0);
				printf("\n --- Registration correct.\n");
			}
			else
			{
				for( i=0; i<numResultLength; i++)
				{
					bufResultEncrypt[i] = rand() % 256;
				}

				// ensure random num makes registration die
				bufResultEncrypt[0x42] = 0xc6 + (rand() % 10);  

				send( s2, bufResultEncrypt, sizeof(bufResultEncrypt), 0);
				printf( "\n *** Incorrect information.\n");
			}

		}
	}

	WSACleanup();
	return 0;
}

