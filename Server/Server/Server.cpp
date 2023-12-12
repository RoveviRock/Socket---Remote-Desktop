#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
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
	SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET chỉ định sử dụng giao thức IPv4
	//SOCK_STREAM chỉ định dùng giao thức TCP
	//Phần cuối là tham số khác, đặc mặc định là 0 
	if (server == INVALID_SOCKET) //Kiểm tra khởi tạo thành công hay chưa 
	{
		cerr << "Khoi tao ServerSocket that bai\n";
		WSACleanup();
		//Đóng và dọn dẹp tất cả tài nguyên được tạo bởi WS 
		return 1;
	}

	//Cấu hình địa chỉ server
	sockaddr_in serverAddr{};
	//Biến kiểu sockaddr_in dùng để	định nghĩa thông tin địa chỉ của máy chủ 
	serverAddr.sin_family = AF_INET;
	//Đặt loại địa chỉ của server là AF_INET, cho biết dùng giao thức IPv4
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	//Đặt địa chỉ IP của server
	//INADDR_ANY dùng để chỉ định rằng máy chủ sẽ lắng nghe trên tất cả các địa chỉ IP của máy tính
	serverAddr.sin_port = htons(12345);
	//Đặt cổng mà máy chủ sẽ lắng nghe kết nối

	//Gán (Bind) socket
		//server: là socket muốn bind địa chỉ cho
		//Tham số thứ 2 là con trỏ đến 1 biến kiểu sockaddr
		//nhưng vì serverAddr là biến kiểu sockaddr_in nên dùng (struct sockaddr*)&serverAddr để chuyển đổi
		//struct sockaddr*: là 1 con trỏ đến 1 biến kiểu sockaddr, đại diện cho địa chỉ mạng
		//&serverAddr: là địa chỉ của biến serverAddr, là địa chỉ của đối tượng chứa thông tin về địa chỉ máy chủ   
		//sizeof(serverAddr): kích thước của đối tượng địa chỉ, truyền dưới dạng số byte
		//là kích thước của biến kiểu sockaddr, chứa thông tin địa chỉ IPv4 và port
	if (bind(server, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		cerr << "Gan (Bind) that bai\n";
		closesocket(server);
		//Đóng socket, giải phóng tài nguyên và tránh rò rỉ socket 
		WSACleanup();
		return 1;
	}

	//Lắng nghe kết nối từ client
	listen(server, 1);
	//Tham số thứ 2 là số lượng client mà server có thể chấp nhận kết nối đồng thời
	cout << "Dang doi client ket noi...\n";

	//Chấp nhận kết nối
	SOCKET client = accept(server, NULL, NULL);
	//NULL & NULL: tham số chỉ định không quan tâm đến thông tin địa chỉ của client
	cout << "Da co client ket noi\n";

	while (true)
	{
		int x, y;
		char bufferX[sizeof(int)];
		char bufferY[sizeof(int)];
		//Nhận dữ liệu vào bufferX, bufferY với kiểu dữ liệu char*
		recv(client, bufferX, sizeof(int), 0);
		recv(client, bufferY, sizeof(int), 0);
		//Chuyển đổi về kiểu int
		x = *reinterpret_cast<int*>(bufferX);
		y = *reinterpret_cast<int*>(bufferY);
		//Sử dụng cấu trúc INPUT để chỉ định sự kiện chuột
		INPUT input;
		input.type = INPUT_MOUSE;
		//Xác định rằng đây là sự kiện liên quan đến chuột
		input.mi.dx = x;
		input.mi.dy = y;
		//Lưu trữ tọa độ chuột
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		//Xác định 1 sự kiện di chuyển chuột
		SendInput(1, &input, sizeof(INPUT));
		//SendInput là 1 hàm WIN API, dùng để gửi sự kiện đầu vào đến hệ điều hành, "1" là số lượng sự kiện
		Sleep(100);
	}

		////Nhận và gởi tin nhắn
		//char buffer[100];
		//while (true)
		//{
		//	memset(buffer, 0, sizeof(buffer));
		//	//Đảm bảo rằng buffer luôn được làm mới (các giá trị đều = 0) trước khi nhận tin mới 
		//	recv(client, buffer, sizeof(buffer), 0);
		//	//Tham số cuối là flag, để xác định cách thức hoạt động của hàm 
		//	//Ở đây không có cờ đặc biệt nào được áp dụng nên flag = 0
		//	cout << "Client nhan: " << buffer << endl;
		//	cout << "Server nhan: ";
		//	cin.getline(buffer, sizeof(buffer));
		//	send(client, buffer, sizeof(buffer), 0);
		//}

	//Đóng socket và dọn dẹp WS
	closesocket(server);
	closesocket(client);
	WSACleanup();
	return 0;
}