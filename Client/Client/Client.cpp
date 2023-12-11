﻿#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	//Khởi tạo WinSock
	//Biến kiểu WSADATA lưu trữ thông tin về việc khởi tạo WS
	WSADATA wsaData;
	//Hàm WSAStartup khởi tạo WS với phiên bản 2.2 
	//Nếu hàm trả về giá trị != 0 thì đã xảy ra lỗi trong qt khởi tạo
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cerr << "Khoi tao WinSock that bai\n";
		//cerr dùng để in thông báo lỗi hoặc thông tin debug 
		//Dùng cerr sẽ hiển thị ngày lập tức và không bị đệm như cout 
		//Nó sẽ xuất hiện ngay trước các thông điệp của cout
		return 1;
	}

	//Khởi tạo Socket
	SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET chỉ định sử dụng giao thức IPv4
	//SOCK_STREAM chỉ định dùng giao thức TCP
	//Phần cuối là tham số khác, đặc mặc định là 0 
	if (client == INVALID_SOCKET) //Kiểm tra khởi tạo thành công hay chưa 
	{
		cerr << "Khoi tao ClientSocket that bai\n";
		WSACleanup();
		//Đóng và dọn dẹp tất cả tài nguyên được tạo bởi WS 
		return 1;
	}

	//Cấu hình địa chỉ server
	sockaddr_in serverAddr;
	//Biến kiểu sockaddr_in dùng để	định nghĩa thông tin địa chỉ của máy chủ 
	serverAddr.sin_family = AF_INET;
	//Đặt loại địa chỉ của server là AF_INET, cho biết dùng giao thức IPv4
	/*char addr[] = "127.0.0.1";
	char* Addr = new char[strlen(addr) + 1];
	strcpy_s(Addr, strlen(addr) + 1, addr);*/
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//Đặt địa chỉ IP của server
	//"127.0.0.1" là địa chỉ loopback. Máy chủ sẽ lắng nghe kết nối từ client trên cùng máy tính
	serverAddr.sin_port = htons(12345);
	//Đặt cổng mà máy chủ sẽ lắng nghe kết nối

	//Kết nối đến server 
		//client: là socket muốn gởi kết nối đến server 
		//Tham số thứ 2 là con trỏ đến 1 biến kiểu sockaddr
		//nhưng vì serverAddr là biến kiểu sockaddr_in nên dùng (struct sockaddr*)&serverAddr để chuyển đổi
		//struct sockaddr*: là 1 con trỏ đến 1 biến kiểu sockaddr, đại diện cho địa chỉ mạng
		//&serverAddr: là địa chỉ của biến serverAddr, là địa chỉ của đối tượng chứa thông tin về địa chỉ máy chủ   
		//sizeof(serverAddr): kích thước của đối tượng địa chỉ, truyền dưới dạng số byte
		//là kích thước của biến kiểu sockaddr, chứa thông tin địa chỉ IPv4 và port
	if (connect(client, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		cerr << "Ket noi that bai\n";
		closesocket(client);
		//Đóng socket, giải phóng tài nguyên và tránh rò rỉ socket 
		WSACleanup();
		return 1;
	}

	//Nhận và gởi tin nhắn
	char buffer[100];
	while (true)
	{
		cout << "Client nhan: ";
		cin.getline(buffer, sizeof(buffer));
		send(client, buffer, sizeof(buffer), 0);
		memset(buffer, 0, sizeof(buffer));
		//Đảm bảo rằng buffer luôn được làm mới (các giá trị đều = 0) trước khi nhận tin mới 
		recv(client, buffer, sizeof(buffer), 0);
		//Tham số cuối là flag, để xác định cách thức hoạt động của hàm 
		//Ở đây không có cờ đặc biệt nào được áp dụng nên flag = 0
		cout << "Server nhan: " << buffer << endl;
	}

	//Đóng socket và dọn dẹp WS
	closesocket(client);
	WSACleanup();
	return 0;
}