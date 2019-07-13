/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	파일명			: Socket.cpp
*	작성자			: 오수백
*	최종 수정일		: 19.04.09
*
*	단대단 통신을 위한 소켓 모듈
*/

#include "stdafx.h"
#include "Socket.h"
#include "Parser.hpp"
#include "MacroTransaction.h"
#include "System.h"
#include "FileStream.h"
#include "NetworkStream.h"
#include "Debugger.h"
#include "Constant.h"

using namespace std;

Socket::Socket(const SOCKET &hSockRaw, const SOCKADDR_IN &sockAddr, const bool connected)
	: __sock(hSockRaw), __sockAddr(sockAddr), __connected(connected)
{
	BOOL opt = TRUE;
	setsockopt(__sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(opt));
}

bool Socket::isConnected() const
{
	return __connected;
}

shared_ptr<Socket> Socket::connect(const int timeout)
{
	return shared_from_this();
}

bool Socket::close()
{
	int err = closesocket(__sock);

	if (err == -1) return false;
	return true;
}

Socket::~Socket()
{
	close();
}

shared_ptr<Socket> Socket::create(const tstring &serverIP, const tstring &serverPort)
{
	//server-side : allways return nullptr
	return nullptr;
}


// 이하 send & recv 관련
int Socket::send(const char* const data, const size_t size)
{
	return ::send(__sock, data, static_cast<int>(size), 0);
}

int Socket::recv(char * const p, const size_t size)
{
	return ::recv(__sock, p, static_cast<int>(size), 0);
}

std::tstring Socket::recvMSG()
{
	recv(bufForTemp, BUF_SIZE);
	return Parser::LPCSTR$tstring(bufForTemp);
}

bool Socket::recvObj(const ObjectType objectType, const uint32_t byteCount)
{
	EventBroadcaster &eventBroadcaster = System::getSystemContents().getEventBroadcaster();
	NetworkStream nStream = NetworkStream(shared_from_this(), byteCount);

	//임시로 테스트 위해 여기서 add listener
	// eventBroadcaster.addDummyReceivedListener(SuperDummy::getInstance());

	switch (objectType) {

	case ObjectType::DUMMY_CLASS:
		// NetworkStream을 통한 DummyClass 객체 생성
		// notify
		// eventBroadcaster.notifyDummyReceived(make_shared<DummyClass>(nStream));
		break;
	}

	return true;
}

bool Socket::recvFile(const std::string path, const uint32_t byteCount)
{
	ofstream fout(path, ios::binary | ios::out);

	uint32_t recvCnt, offset = 0, needLength;
	memset(bufForTemp, 0, BUF_SIZE);

	while (offset < byteCount) {
		needLength = BUF_SIZE > byteCount - offset ? byteCount - offset : BUF_SIZE;

		recvCnt = recv(bufForTemp, needLength);
		if (recvCnt <= 0) return false;

		fout.write(bufForTemp, recvCnt);
		offset += recvCnt;
	}

	fout.close();

	return true;
}

const PacketHeader* Socket::getHeader()
{
	int err = recv(bufForTemp, BUF_SIZE);
	if (err <= 0) return nullptr;

	memcpy(bufForPH, bufForTemp, 12);
	return reinterpret_cast<PacketHeader*>(bufForPH);
}

bool Socket::sendHeader(PacketHeader ph)
{
	memcpy(bufForTemp, &ph, sizeof(ph));
	if (send(bufForTemp, BUF_SIZE) == -1) {
		return false;
	}
	return true;
}

bool Socket::sendMSG(std::tstring const t_msg, const ProtocolType protocolType)
{
	string s = Parser::tstring$string(t_msg);
	const char * msg = s.c_str();

	PacketHeader ph = PacketHeader(protocolType, ObjectType::MSG, static_cast<uint32_t>(strlen(msg) + 1));
	if (!sendHeader(ph)) return false;

	memcpy(bufForTemp, msg, strlen(msg) + 1);
	if (send(bufForTemp, BUF_SIZE) == -1)
		return false;
	return true;
}

bool Socket::sendObj(Serializable & obj, const ObjectType objectType)
{

	uint32_t readCnt, offset = 0, sendLength, elemOffset;
	uint32_t byteCount = obj.getStream().getStreamSize();
	std::vector<ElementMeta> elements = obj._getStreamMeta();
	size_t err;
	memset(bufForTemp, 0, BUF_SIZE);

	
	//send packet header
	PacketHeader ph = PacketHeader(ProtocolType::OBJECT, objectType, byteCount);
	sendHeader(ph);

	//send object
	for (const ElementMeta & em : elements) { // 멤버 변수 별 전송 
		
		if (em.elemSize < BUF_SIZE) {
			readCnt = obj.serialAccess(offset, reinterpret_cast<ubyte *>(bufForTemp), em.elemSize);
			err = send(bufForTemp, BUF_SIZE); // BUF_SIZE 보다 작은 경우, BUF_SIZE로 전송
			if (err == -1) return false;
			offset += readCnt;
			continue; // 보냈으므로 다음 element 전송
		}

		// BUF_SIZE보다 큰 경우는 분할 전송
		elemOffset = 0;
		while (static_cast<int>(elemOffset) < em.elemSize) {
			sendLength = BUF_SIZE > em.elemSize - elemOffset ? em.elemSize - elemOffset : BUF_SIZE;

			readCnt = obj.serialAccess(offset, reinterpret_cast<ubyte *>(bufForTemp), sendLength); // ObjectStream.get()으로 치환가능
			err = send(bufForTemp, readCnt);
			if (err == -1) return false;
			elemOffset += readCnt;
			offset += readCnt;	
		}
	}
	
	return true;
}

bool Socket::sendFile(const std::string path)
{
	FileStream fstream = FileStream(path);
	uint32_t readCnt, offset = 0;
	uint32_t byteCount = fstream.getStreamSize();
	size_t err;
	memset(bufForTemp, 0, BUF_SIZE);
	
	// send packet header
	PacketHeader ph = PacketHeader(ProtocolType::FILE_RESPONSE, ObjectType::MAX, byteCount);
	sendHeader(ph);

	// send file name
	
	// send file content
	while (offset < byteCount) {
		readCnt = fstream.get(bufForTemp, BUF_SIZE);
		err = send(bufForTemp, readCnt);
		if (err == -1) return false;
		offset += readCnt;
	}
	return true;
}

std::shared_ptr<Socket> Socket::__receivingLoop()
{

	while (true) {
		//recv packet header
		const PacketHeader* const ph = getHeader();
		
		if (ph == nullptr) {
			return shared_from_this();
		}

		// protocol processing(recv content 포함)
		switch (ph->getProtocolType()) {

		case ProtocolType::CONNECTION_CHECK:
			Debugger::popMessageBox(_T("CONNECTION_CHECK"));
			sendMSG(_T("Hello Response : Hello Server"), ProtocolType::CONNECTION_RESPONSE);
			break;

		case ProtocolType::CONNECTION_RESPONSE:
			Debugger::popMessageBox(recvMSG());
			break;

		case ProtocolType::OBJECT:
			recvObj(ph->getObjectType(), ph->getByteCount());
			break;
		
		case ProtocolType::FILE_REQUEST:
			dbFileRequestAccepted();
			break;
		
		case ProtocolType::FILE_RESPONSE:
			recvFile(string("c:\\network_test\\received\\dummy.txt"), ph->getByteCount()); // 경로? 파일명? 확장자?

		case ProtocolType::LOGIN:
			loginRequestAccepted();
			break;
		}


	}

}

bool Socket::loginRequestAccepted()
{
	ServerDBManager &dbManger = System::getSystemContents().getServerDBManager();

	// 로딩이 되어 있지 않으면 로드 시도. 로드 실패 시 false 반환
	if (!dbManger.isLoaded())
		if (! dbManger.load(Constant::DB::ROOT_DIR, false)) {
			recvMSG();
			sendHeader(PacketHeader(ProtocolType::DB_ERROR, ObjectType::MAX, 0));
			return false;
		}
	
	// id 검증
	const AccountDBManager &accountMgr = dbManger.getAccountManager();

	const tstring id = recvMSG();
	std::shared_ptr<Account> account = accountMgr.getAccount(id);
	if (account == nullptr) {
		sendHeader(PacketHeader(ProtocolType::NOK, ObjectType::MAX, 0));
		return false;
	}
	else sendHeader(PacketHeader(ProtocolType::OK, ObjectType::MAX, 0));

	// password 검증
	getHeader();
	const tstring password = recvMSG();
	if (password == account->password)  
	{
		sendHeader(PacketHeader(ProtocolType::OK, ObjectType::MAX, 0));
	}
	else {
		sendHeader(PacketHeader(ProtocolType::NOK, ObjectType::MAX, 0));
		return false;
	}

	// UserInfo 전송
	sendObj(*account, ObjectType::USER_INFO);

	return true;
}

bool Socket::dbFileRequestAccepted()
{
	ServerDBManager &dbManger = System::getSystemContents().getServerDBManager();

	// 로딩이 되어 있지 않으면 로드 시도. 로드 실패 시 false 반환
	if (!dbManger.isLoaded())
		if (!dbManger.load(Constant::DB::ROOT_DIR, false)) {
			recvMSG();
			sendHeader(PacketHeader(ProtocolType::DB_ERROR, ObjectType::MAX, 0));
			return false;
		}

	// Recv name
	const tstring name = recvMSG();

	// Recv ServerDBSectionType
	ServerFileDBSectionType sectionType;
	recv(bufForTemp, BUF_SIZE);
	memcpy(&sectionType, bufForTemp, sizeof(sectionType));

	// Send EXISTENT (OK, NOK)
	const FileDBManager &fileMgr = dbManger.getFileManager();

	const tstring path = fileMgr.getPath(sectionType, name);
	if (path == _T("")) { // 파일 존재하지 않음 
		sendHeader(PacketHeader(ProtocolType::NOK, ObjectType::MAX, 0));
		return false;
	}
	//파일 존재함
	sendHeader(PacketHeader(ProtocolType::OK, ObjectType::MAX, 0));
	
	// Send File
	sendFile(Parser::tstring$string(path));

	// Send Protocol End
	sendHeader(PacketHeader(ProtocolType::PROTOCOL_SUCCESS, ObjectType::MAX, 0));
	
	return true;
}
