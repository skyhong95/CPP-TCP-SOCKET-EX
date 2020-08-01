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
	SOCKET serverSocket, clientSocket; 
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256]; //���ڿ� ������ ũ��
	string sent;

	// ���� �ʱ�ȭ(������ ��� �����޽��� ���)
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ShowErrorMessage("WSAStartup()");
	}

	// �������� �ʱ�ȭ
	// PF_INET - IPv4, SOCK_STREAM - TCP
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET) ShowErrorMessage("socket()");

	// serverAdress �ʱ�ȭ �� �ּҰ� ����
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 4����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort); // 2 ����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[���� ����] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "[���� ����] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[���� ����] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[Ŭ���̾�Ʈ �޽���]: " << received << endl;
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[���� ����]\n";
			break;
		}
		cout << "[�޽��� ����]: ";
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

