//get rid of all the crap in windows.h that we won't ever use
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

//usual stuff
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>

//winsock
#include "winsock2.h"

//need for getnameinfo() function
#include "Ws2tcpip.h"

//help the compiler figure out where winsock lives.  You could
//also add this to the linker but this means no 
//extra settings for the .csproj file
#pragma comment(lib, "ws2_32.lib")

//using namespace std;

//length of our buffers
const int BUF_LEN = 255;

//our remote port that we shall connect to
const int remote_port = 3000u;

//path of the config file
#define CONFIG_FILE "../config.txt"

//struct to store the IP address and port of the remote system
struct IPAddr
{
	char IP[16];
	int Port;
};

// function to read the remote Ip and port from config file
IPAddr readFromConfig();

int main(void){

	//our buffers
	char receiveBuffer[BUF_LEN];	
	char sendBuffer[BUF_LEN];

	char* password = "notmypassword";
	SecureZeroMemory(sendBuffer, BUF_LEN);
	SecureZeroMemory(receiveBuffer, BUF_LEN);
	
	int ret = 0;

	//start up winsock, asking for version 2.2
	
	//our WSAData structure.  This holds information about the version of winsock that we are using
	//not necessarily the version that we are requesting
	WSADATA wsaData;
	//let's start with zeroing wsaData, not a bad idea
	SecureZeroMemory(&wsaData, sizeof(wsaData));

	//you must call this function to initialize winsock.  You must call WSACleanup when you are finished.
	//this uses a reference counter, so for each call to WSAStartup, you must call WSACleanup or suffer 
	//memory issues
	ret = WSAStartup(MAKEWORD(2,2), &wsaData);

	//check that WSAStartup was sucessful.  
	if(ret != 0){
		std::cout << "Error initializing winsock!  Error was: " << ret << std::endl;
		return ret;
	}

	//see what happens with this instead (uncomment)
	//WSAStartup(MAKEWORD(1,1), &wsaData);

	//let's examine WSADATA and see what we have here
	int majorversion = LOBYTE(wsaData.wVersion);
	int minorversion = HIBYTE(wsaData.wVersion);

	std::cout << "Version:" << majorversion << "." << minorversion << std::endl;

	//the other fields in wsaData are not so important or are not used.
	//the only other one is 
	//wsaData.wHighVersion, which stores the highest version of winsock that is available
	majorversion = LOBYTE(wsaData.wHighVersion);
	minorversion = HIBYTE(wsaData.wHighVersion);
	std::cout << "Highest version available is: " << majorversion << "." << minorversion << std::endl;
	

	//now we have the version and winsock is initialized, let's start doing something useful with it

	//a sockaddr_in specifies IP address and port information.  the address family tells what the add
	//address used to bind to socket, holds port, IP, and address family
	struct sockaddr_in socketAddress;
	//address family-  this identifies the addressing structure of this address structure.
	//for IP, it must always be set to AF_INET
	socketAddress.sin_family = AF_INET;
	
	//this is the port.  remember, networks are in network-order, which is big-endian.  we use htons() to ensure
	//that the number is in the correct "endian-ness" for the network regardless of what machine architecture we're on
	//for a local connection going out, pick 0 and winsock will assign a random port
	socketAddress.sin_port = 0;

	//now we need to set the actual IP address.  this is an in_addr struct for IPv4.  for Ipv6, it's an in6_addr
	//it is represented as a ulong with each byte representing one part of the ip address.  we can convert an address in
	//it's dot-notation (i.e. 192.168.1.1) to this format using the inet_addr() function, OR we can just use ADDR_ANY which
	//will cause this address to listen to any available network interface (machines may have more than one network card)
	//I hard-coded it to my machine's IP for testing, but let's comment that out and
	//we'll get it from our host below

	//socketAddress.sin_addr.s_addr  = inet_addr("192.168.1.107");  

	//get host.  hostent stores information about a host
	hostent* localhost;

	//gethostbyname- gets host information based on host name.
	//this allocates memory only once per host no matter how many times you call
	//gethostbyname().  
	//deprecated in favor of getaddrinfo

	//there are a lot of ways to do this
	localhost = gethostbyname("");
	char* localIP;

	//this will give us the four-byte representation of our ip address.
	localIP = inet_ntoa(*(in_addr*)*localhost->h_addr_list);

	//let's use the address that we got from gethostbyname for our local endpoint
	socketAddress.sin_addr.s_addr  = inet_addr(localIP);

	// Read the IP and port of the remote system
	IPAddr ip = readFromConfig();
	sockaddr_in remote;
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = inet_addr(ip.IP);
	remote.sin_port = htons(ip.Port);
	
	//gethostbyaddr-  used to look up a host by its address.  otherwise identical to gethostbyname
	//gethostbyaddr(localIP, 4, AF_INET);


	//BOTH of these things are obsolete in favor of getnameinfo()
	//getnameinfo();- more on this much later

	SOCKET clientSocket;	

	//let's create a socket!
	//this socket is not bound to anything, so all it is is an empty socket at this point
	//AF_INET- this means that we want an IP socket.  If we wanted an IPv6 socket, we would use AF_INET6
	//if we wanted to talk to an infrared port, we'd use AF_IRDA
	//bluetooth is AF_BTM

	//SOCK_STREAM - tells us the type of socket.  SOCK_STREAM is two-way, reliable, connection-based byte streams
	//that can transmit OOB data.  uses TCP.  for UDP use SOCK_DGRAM, and for raw, use SOCK_RAW.  there are others also

	//IPPROTO_TCP- tells us to use the TCP protocol.  this parameter can only be used certain ways with
	//certain address family and type settings.  if we wanted UDP, we would have had to set our type to SOCK_DGRAM 
	//or we'll get an error.  passing 0 means that we dont' care about the protocol and the socket can choose

	//NULL-  we specified TCP, so we don't need this parameter, but it would contain protocol info normally.  if
	//we enumerate all the protocols supported on this machine, we could pick one of them and pass it in here to get
	//an explicit protocol implementation.  mostly, we don't care, but if we were using IPX (for example) then we would
	//care a lot

	//0-  this is reserved, always set it to 0.  I'll ask Anthony wtf it's for

	//0-  these are the options.  normally the only one you'd care about is creating an overlapped socket for
	//use with IO ports.  if you want to do that, send WSA_IO_OVERLAPPED here.  more on that in a future demo
	clientSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	//error checking																																		-20 copy paste
	if(clientSocket == INVALID_SOCKET){
		int errorcode = WSAGetLastError();
		std::cout << "Error on creating socket: " << errorcode << std::endl;
		WSACleanup();
		return errorcode;
	}

	//so now we have a socket, and we have a sockaddr_in that describes an address and port, let's put them
	//together and send some data!

	//bind- binds a socket to an address.  
	//clientSocket- the socket to bind
	//socketAddress- the address structure to bind, need to recast to sockaddr here
	//size of the sockaddr struct
	ret = bind(clientSocket, (sockaddr*)&socketAddress, sizeof(socketAddress));
	if(ret == SOCKET_ERROR){
		ret = WSAGetLastError();
		return ret;
	}

	//now we have a socket and we can do something to it.  let's connect to a server somewhere and 
	//send some data

	//we're going to connect.  for connection-oriented sockets (TCP) this will establish a connection.
	//for datagram sockets, it will only set up a default endpoint to send subsequent calls to SEND to

	//we're going connect to a remote host now.  
	//clientSocket is the socket that we've created for this end of the connection
	//remote is the endpoint that we're connection to.  We have to cast it as sockaddr for this
	//version of connect
	//we also pass the size of the sockaddr struct since this could potentially change based on 
	//what type of address we're using
	std::cout<<"Connecing to: "<<inet_ntoa(remote.sin_addr)<<":"<<ntohs(remote.sin_port)<<"\n";
	ret = connect(clientSocket, (sockaddr*)&remote, sizeof(remote));
	if(ret == SOCKET_ERROR){
		ret = WSAGetLastError();
		return ret;
	}

	//Execute the following loop until user hits '/q' or an error occurs
	bool quit = false;
	std::cout<<"Type /q to end the chat session\n";
	std::string str, s;
	while(!quit)
	{
		std::getline(std::cin, str);
		if(!str.compare("/q"))
		{
			quit = true;
			std::cout<<"Quitting\n";
		}
		else
		{
			int start = 0;
			int len = 0;

			// code to send typed data as 255 byte chunks
			do
			{
				len = str.copy(sendBuffer, BUF_LEN-1, start);
				sendBuffer[len] = '\0';
				start += len;
				//
				//send-  this sends
				//clientSocket- the socket to send to
				//sendbuffer - the buffer to send (char[])
				//12- number of bytes to send.  I hard-coded this because it's 2:30 am and I'm tired and
				//this is just some demo program so it's not horribly important,
				//but you probably shouldn't do that
				//0- flags.  only useful flag here is MSG_OOB, whic will send this as out of band data,
				//which for TCP will set the URG flag
				ret = send(clientSocket, sendBuffer, ++len, 0);
				if(ret != len)
					ret = send(clientSocket, sendBuffer + ret, len - ret, 0);
				if(ret == SOCKET_ERROR)
				{
					ret = WSAGetLastError();
					return ret;
				}
				//
				ret = recv(clientSocket, sendBuffer, BUF_LEN, 0);
				if(ret == SOCKET_ERROR){
					ret = WSAGetLastError();
					break;
				}
				//
				s.assign(sendBuffer, ret);
				std::cout << "[Server]: "<< s << std::endl;
			}
			while(len == BUF_LEN);
		}
	}
	//copy some data to a buffer.  many ways to do this, pick one.  I chose sprintf
	//sprintf(sendBuffer, "Hello, world");

	//shutdown the socket
	//clientSocket- the socket to shut down
	//SD_SEND-  how to shut it down.  SD_SEND means that
	//you will no longer send over this socket.  for TCP, it 
	//also means that you will send a FIN packet once all 
	//data has been received and acknowledged

	//you also have other options
	//SD_RECEIVE - no more receiving data.  triggers a RST if
	//data is received after this is done

	//SD_BOTH- no more send and receive
	ret = shutdown(clientSocket, SD_BOTH);
	if(ret == SOCKET_ERROR){
		ret = WSAGetLastError();
		return ret;
	}
	
	//clean up the socket.  Techncially, WSACleanup will do this for you
	//but it's good to get in the habit of closing your own sockets.
	ret = closesocket(clientSocket);
	if(ret == SOCKET_ERROR){
		ret = WSAGetLastError();
		return ret;
	}

	//don't forget to call this at the end.  in total, call it as many times as you have called
	//WSAStartup()
	WSACleanup();

	std::cout << "done" << std::endl;

	std::cin.get();


return 0;

}

//function to read the ip address and port from a config file
IPAddr readFromConfig()
{
	IPAddr ip;
	sprintf_s(ip.IP, "000.000.000.000");
	ip.Port = -1;
	//
	FILE *file;
	char line[100];
	//
	if (!(file = fopen(CONFIG_FILE, "r" )))
	{
		std::cout << "[ERROR] server.cpp::config.txt could not be opened for reading\n" << "\n";
	}
	else
	{
		if(fgets(line, 100, file) == NULL)
		{
			if(ferror(file))
			{
				std::cout << "[ERROR] server.cpp::fgets error1\n";
			}
		}
		else
		{
			std::string str(line);
			str.copy(ip.IP, str.size(), 0);
			//
			if(fgets(line, 100, file) == NULL)
			{
				if(ferror(file))
				{
					std::cout << "[ERROR] server.cpp::fgets error2\n";
				}
			}
			else
			{
				ip.Port = atoi(line);
			}
		}
	}
	//std::cout<< "\nIP: "<<ip.IP<<", PORT: "<<ip.Port<<"\n";
	//
	return ip;
}
