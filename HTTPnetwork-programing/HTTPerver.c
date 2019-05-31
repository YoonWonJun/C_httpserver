#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>

#define SERVER_PORT 3000

void ErrorMessage(char *Message);

int main(void)
{
	WSADATA wsaData;

	SOCKET sockServer;
	SOCKET sockClient;

	struct sockaddr_in addrSockServer;
	struct sockaddr_in addrSockClient;

	int aScLen;

	char readBuf[2048];
	char sendBuf[2048];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorMessage("WSAStartup() Error !");
	}
	
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	if (sockServer == INVALID_SOCKET)
	{
		ErrorMessage("socket() Error !");
	}

	addrSockServer.sin_family = AF_INET;
	addrSockServer.sin_port = htons(SERVER_PORT);
	addrSockServer.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockServer, (struct sockaddr *)&addrSockServer, sizeof(addrSockServer)) != 0)
	{
		ErrorMessage("bind() Error !");
	}

	if (listen(sockServer, 5) != 0)
	{
		ErrorMessage("listen() Error !");
	}

	memset(sendBuf, 0, sizeof(sendBuf));
	_snprintf(sendBuf, sizeof(sendBuf),
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"<HTML>\r\n"
		"<HEAD>\r\n"
		"<TITLE>HTTP SERVER</TITLE>\r\n"
		"</HEAD>\r\n"
		"<BODY>\r\n"
		"<H1>PUSH BUTTON</H1>\r\n"
		"<a href=/?on1 target=inlineframe>ON</a>\r\n"
		"<BODY>\r\n"
		"</HTML>\r\n");
	
	while (1)
	{
		aScLen = sizeof(addrSockClient);
		sockClient = accept(sockServer, (struct sockaddr *)&addrSockClient, &aScLen);
		if (sockClient == INVALID_SOCKET)
		{
			ErrorMessage("accept() Error !");
		}
		memset(readBuf, 0, sizeof(readBuf));
		recv(sockClient, readBuf, sizeof(readBuf), 0);
		printf("%s", readBuf);
		send(sockClient, sendBuf, (int)strlen(sendBuf), 0);
		closesocket(sockClient);
	}
	WSACleanup();

	return 0;
}

void ErrorMessage(char *Message)
{
	fprintf(stderr, "Error : %s\n", Message);
	exit(1);
}