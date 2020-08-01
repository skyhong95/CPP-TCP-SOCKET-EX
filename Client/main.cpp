#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h> //���̺귯�� �߰�

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[���� �߻�]: " << message << endl;
	system("pause");
	exit(1);
} // �����޽��� �Լ�

int main() {
	WSADATA wsaData; // ���� �ʱ�ȭ ���� ����ü
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;
	char received[256]; //���ڿ� ������ ũ��
	string sent;

	// ���� �ʱ�ȭ(������ ��� �����޽��� ���)
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ShowErrorMessage("WSAStartup()");
	}

	// �������� �ʱ�ȭ
	// PF_INET - IPv4, SOCK_STREAM - TCP
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (clientSocket == INVALID_SOCKET) ShowErrorMessage("socket()");

	// serverAdress �ʱ�ȭ �� �ּҰ� ����
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //���ڿ� IP ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort); // 2 ����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	cout << "[���� ����] connect()\n"; 

	while (1) {
		cout << "[�޽��� ����]: ";
		getline(cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;

		if (strcmp(received, "[exit]") == 0) {
			cout << "[���� ����]\n";
			break;
		}
		cout << "[���� �޽���]: " << received << endl;
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}
