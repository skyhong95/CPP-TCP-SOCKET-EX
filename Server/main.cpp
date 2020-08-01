#include <iostream>
#include <string>
#include <WinSock2.h> //라이브러리 추가

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[오류 발생]: " << message << endl;
	system("pause");
	exit(1);
} // 에러메시지 함수

int main() {
	WSADATA wsaData; // 윈속 초기화 정보 구조체
	SOCKET serverSocket, clientSocket; 
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256]; //문자열 버퍼의 크기
	string sent;

	// 윈속 초기화(에러일 경우 에러메시지 출력)
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ShowErrorMessage("WSAStartup()");
	}

	// 서버소켓 초기화
	// PF_INET - IPv4, SOCK_STREAM - TCP
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET) ShowErrorMessage("socket()");

	// serverAdress 초기화 및 주소값 저장
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 4바이트 정수를 네트워크 바이트 형식으로
	serverAddress.sin_port = htons(serverPort); // 2 바이트 정수 네트워크 바이트 형식으로

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[클라이언트 메시지]: " << received << endl;
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[서버 종료]\n";
			break;
		}
		cout << "[메시지 전송]: ";
		getline(cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);

	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}

