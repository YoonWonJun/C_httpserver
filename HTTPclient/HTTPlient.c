#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>

int recvn(SOCKET hSocket, char *buf, int len, int flags);
void ErrorMessage(char * Message);

#define SERVER_PORT 3000	// 접속할 서버 포트
#define READ_BUFF_SIZE 512	// 한번에 받을 최대 버퍼 사이즈

int main(void)
{
	WSADATA wsaData;
	SOCKET hSocket;
	struct sockaddr_in servAddr;

	char sendBuf[2048];
	char readBuf[2048];

	int sendLen;
	int recvIndex = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorMessage("WSAStartup() Error !");
	}

	hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(hSocket == INVALID_SOCKET)
	{
		ErrorMessage("socket() Error !");
	}

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(SERVER_PORT);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(hSocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorMessage("connect() Error !");
	}

	_snprintf(sendBuf, sizeof(sendBuf),	// Request 헤더 정의
	 	"GET /HTTP/1.1\r\n"
		"Accept-Encoding: gzip, deflate. br\r\n"
		"Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7"
		"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36");
	sendLen = send(hSocket, sendBuf, strlen(sendBuf), 0);
	if (sendLen == SOCKET_ERROR)
	{
		ErrorMessage("send() Error !");
	}
	recvIndex = recvn(hSocket, readBuf, READ_BUFF_SIZE, 0);
	readBuf[recvIndex] = '\0';
	printf("%s\n", readBuf);
	closesocket(hSocket);
	WSACleanup();

	return 0;
}
int recvn(SOCKET hSocket, char *buf, int len, int flags)	// 서버로부터 받는 데이터의 크기가 한번에 받을 수 있는 버퍼를 초과했을때를 대비해 함수 정의
{
	char *ptr = buf;
	int recvLen;
	int recv_Index = 0;

	while (1)
	{
		recvLen = recv(hSocket, ptr, len, flags);			// recvLen에 받은 데이터의 크기 저장
		if (recvLen == SOCKET_ERROR)
		{
			ErrorMessage("recv() Error !");
		}
		else if (recvLen == 0) break;
		recv_Index += recvLen;								// recv_Index에 받은 데이터를 쌓는다
		ptr += recvLen;										// 추가 데이터가 있는경우 buf에 저장된 문자열의 뒤에 계속해서 문자를 추가해주기위해 받은 데이터크기만큼 시작주소를 더해준다
	}
	return recv_Index;										// 전체 받은 데이터의 크기 문자열의 마지막에 '\0'를 추가해주기위해 리턴
}

void ErrorMessage(char * Message)
{
	fprintf(stderr, "Error : %s\n", Message);
	exit(1);
}