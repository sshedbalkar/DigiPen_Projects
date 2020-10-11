#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <string>
#include <stdio.h>
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")


//using namespace std;

//buffer length, this is arbitrary, but note that I use a const and not a #define
const int BUF_LEN = 255;

// path of the config file
#define CONFIG_FILE "../config.txt"

//function to read port address from the config file
int readPortFromConfig();

int main(void){

	int result = 0;

	//WSAData, same as in the client
	WSADATA wsData;

	//host info
	char* localIP;
	hostent* localhost;
	
	//our receive buffer
	char receiveBuffer[BUF_LEN];
	SecureZeroMemory(receiveBuffer, BUF_LEN);
	
	//initialize winsock
	result = WSAStartup(MAKEWORD(2,2), &wsData);
	if(result){
		printf("Error starting winsock: %d", result);
		return result;
	}

	//our listener socket
	SOCKET listenerSocket;
	struct sockaddr_in socketAddress;

	//get local host IP, same as in client demo
	localhost = gethostbyname("");
	localIP = inet_ntoa(*(in_addr*)*localhost->h_addr_list);

	std::cout << "Locaohost: " << localhost->h_name << std::endl;
	std::cout << "Local IP: " << localIP << std::endl;

	//create local endpoint for listening.
	socketAddress.sin_family = AF_INET;
	//this time, set the port to something that's greater than 1024
	socketAddress.sin_port = htons(readPortFromConfig());
	socketAddress.sin_addr.s_addr  = inet_addr(localIP);

	//create our socket
	listenerSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	//handle errors
	if(listenerSocket == INVALID_SOCKET){
		printf("WSASocket call failed with error: %ld\n", WSAGetLastError());
		WSACleanup();			
		return 0;
	}

	//bind our listener socket to local endpoint
	result = bind(listenerSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress));
	if(result == SOCKET_ERROR)
		return WSAGetLastError();

	//now we listen for someone to connect.  This will put the socket into a listening
	//state, but will not automatically accept a connection
	result = listen(listenerSocket, 10);	
	if(result == SOCKET_ERROR)
		return WSAGetLastError();

	//we need to have another socket here.  When we accept a connection, that connection
	//will give us a socket that is effectively identical to the socket that we were
	//listening with, however this new socket will be part of a socket pair
	//with the remote socket that is connecting.
	SOCKET clientSocket;
	sockaddr_in remoteEndpoint;
	SecureZeroMemory(&remoteEndpoint, sizeof(remoteEndpoint));
	
	int endpointSize = sizeof(remoteEndpoint);

	//now we call accept on a listening socket.  This blocks until a remote connection is
	//established, at which time it returns a new socket that is connected to the remote one
	//so that we can send and receive data.  the remoteEndpoint that we pass in will
	//have the address information from the remote connection.
	while((clientSocket = accept(listenerSocket, (sockaddr*)&remoteEndpoint, &endpointSize)) != INVALID_SOCKET)
	{
		std::cout<<"Received connection from: "<<inet_ntoa(remoteEndpoint.sin_addr)<<":"<<ntohs(remoteEndpoint.sin_port)<<"\n";
		//listen for data
		int rcounter = 0;
		std::string s;
		int len;
		//we're going to keep listening for data until the client closes the 
		//connection.  if you wanted to accept additional clients, you would need
		//to do this differently with additional threads.
		while(true){
			//get some data.
			//clientSocket- the socket we're waiting for data on
			//receiveBuffer- a buffer of some sort
			//BUF_LEN- the length of receive buffer
			//0- socket options but we dont' need them here
			//recv() will return the number of bytes read, which will
			//be in the buffer you passed in.  It may read fewer bytes than 
			//were sent, which means you'll need to keep calling read until it either
			//blocks (meaning no more data right now) or returns 0 immediately, meaning
			//that the connection was closed on the other end.
			rcounter = recv(clientSocket, receiveBuffer, BUF_LEN, 0);
			if(rcounter == SOCKET_ERROR){
				rcounter = WSAGetLastError();
				std::cout<<"Disconnected abruptly: "<<inet_ntoa(remoteEndpoint.sin_addr)<<":"<<ntohs(remoteEndpoint.sin_port)<<"\n";
				break;
			}

			//means that no more data is available, happens
			//when client sends graceful shutdown.
			if(rcounter == 0)
			{
				std::cout<<"Disconnected gracefully: "<<inet_ntoa(remoteEndpoint.sin_addr)<<":"<<ntohs(remoteEndpoint.sin_port)<<"\n";
				break;
			}

			//print whatever we just got.
			s.assign(receiveBuffer, rcounter);
			std::cout << "[Client]: "<< s << std::endl;
			len = s.size();
			//
			rcounter = send(clientSocket, receiveBuffer, len, 0);
			if(rcounter != len)
				rcounter = send(clientSocket, receiveBuffer + rcounter, len - rcounter, 0);
			if(rcounter == SOCKET_ERROR)
			{
				rcounter = WSAGetLastError();
				break;
			}
		}
	}
	if(clientSocket == INVALID_SOCKET)
		return WSAGetLastError();
	printf("Shutdown received, ending loop");

	//shut down our sockets, both the one we originally created
	//and the one we got when the connection was established
	shutdown(clientSocket, SD_BOTH);
	shutdown(listenerSocket, SD_BOTH);
	
	//close the sockets
	closesocket(clientSocket);
	closesocket(listenerSocket);

	WSACleanup();

	std::cin.get();

	return 0;
}

// function to read the port address from the config file
int readPortFromConfig()
{
	int port = -1;
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
			if(fgets(line, 100, file) == NULL)
			{
				if(ferror(file))
				{
					std::cout << "[ERROR] server.cpp::fgets error2\n";
				}
			}
			else
			{
				port = atoi(line);
			}
		}
	}
	//std::cout<< "PORT: "<<port<<"\n";
	//
	return port;
}