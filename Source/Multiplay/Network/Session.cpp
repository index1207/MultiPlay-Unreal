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
		
	gen::PacketId id = gen::PacketId::None;
	std::memcpy(&id, buffer.data(), sizeof(unsigned short));
	id = static_cast<gen::PacketId>(ntohs(static_cast<u_short>(id)));
	
	PacketQue.Enqueue(gen::PacketHandler::parsePacket(id, buffer));
}

void FSession::Send(std::span<char> data)
{
	if (!Socket->send(data))
		OnDisconnected();
}

void FSession::Send(Packet* pkt)
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
	while(true)
	{
		TSharedPtr<Packet> Packet;
		if (!PacketQue.Dequeue(Packet))
			return;
		if (Packet != nullptr)
			Packet->executeHandler(AsShared());
	}
}