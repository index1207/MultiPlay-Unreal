#pragma once
#include "generated/ClientPacketHandler.gen.hpp"
#include "net/Socket.hpp"

#include "Templates/SharedPointer.h"

class FSession : public TSharedFromThis<FSession>
{
	friend class FIoThread;
public:
	FSession(net::Socket* socket);
	virtual ~FSession() = default;
public:
	void OnConnected();
	void OnDisconnected();
	void OnReceive(std::span<char> buffer, int32 length);
public:
	void Send(std::span<char> data);
	void Send(sv::Packet* pkt);
public:
	net::Socket* GetHandle() const;
	void Flush();
private:
	net::Socket* Socket;
	char m_buffer[4096];
	TQueue<TFunction<bool(TSharedPtr<FSession>)>> HandlerQue;
};
