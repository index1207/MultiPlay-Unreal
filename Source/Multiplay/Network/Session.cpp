#include "Session.h"

#include "generated/ClientPacketHandler.gen.hpp"

FSession::FSession(net::Socket* socket) : Socket(socket)
{
	ZeroMemory(&m_buffer, sizeof(m_buffer));
}

void FSession::OnConnected()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("ON Connected to server."));
}
void FSession::OnDisconnected()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Orange, TEXT("ON Disconnected to server."));
}

void FSession::OnReceive(std::span<char> buffer, int32 length)
{
	if (length <= 2)
		return;
	auto Handler = gen::PacketHandler::getHandler(buffer);
	HandlerQue.Enqueue(Handler);
}

void FSession::Send(std::span<char> data)
{
	if (!Socket->send(data))
		OnDisconnected();
}

void FSession::Send(sv::Packet* pkt)
{
	pkt->write();
	Send(pkt->data());
}

net::Socket* FSession::GetHandle() const
{
	return this->Socket;
}

void FSession::Flush()
{
	while (!HandlerQue.IsEmpty())
	{
		TFunction<bool(TSharedPtr<FSession>)> Handler;
		if (HandlerQue.Dequeue(Handler))
		{
			if (Handler)
				bool Result = Handler(AsShared());
		}
	}
}