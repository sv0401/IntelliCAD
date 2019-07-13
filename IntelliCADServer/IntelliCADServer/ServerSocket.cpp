/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	파일명			: ServerSocket.cpp
*	작성자			: 오수백
*	최종 수정일		: 19.04.09
*
*	서버 소켓 클래스
*/

#include "stdafx.h"
#include "ServerSocket.h"
#include "MacroTransaction.h"
#include "Parser.hpp"

using namespace std;



ServerSocket::ServerSocket(
	const SOCKET &hServerSockRaw, const SOCKADDR_IN &serverSockAddr, const int maxConnection)
	: __sock(hServerSockRaw), MAX_CONNECTION(maxConnection)
{
	//address connect & listening start
	bind(__sock, (SOCKADDR*)&serverSockAddr, sizeof(serverSockAddr));
	listen(__sock, maxConnection);
}

shared_ptr<Socket> ServerSocket::accept()
{
	SOCKADDR_IN clientAddr = {};
	int clientSockSize = sizeof(clientAddr);

	SOCKET clientSock = ::accept(__sock, (SOCKADDR*)&clientAddr, &clientSockSize);

	if (clientSock == INVALID_SOCKET)
		return nullptr;

	return make_shared<Socket>(clientSock, clientAddr, true);
}

bool ServerSocket::close()
{
	IF_T_RET_F(closesocket(__sock) == -1);

	return true;
}

ServerSocket::~ServerSocket()
{
	close();
}

unique_ptr<ServerSocket> ServerSocket::create(const tstring &port, const int maxConnection)
{
	USHORT sPort = Parser::portString$sin_port(port);

	//create socket	
	SOCKET sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		return nullptr;
	}

	//socket structure
	SOCKADDR_IN sockAddr = {};
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = sPort;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	return unique_ptr<ServerSocket>(new ServerSocket(sock, sockAddr, maxConnection));
}