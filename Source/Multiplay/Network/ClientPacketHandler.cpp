#include "generated/ClientPacketHandler.gen.hpp"

#include "Managers/Manager.h"
#include "Managers/Network.h"

#include "Network/Session.h"

auto gen::PacketHandler::LoginResPacketHandler(TSharedPtr<Session> Session, TSharedPtr<LoginRes> Packet) -> void
{
	for (auto& Player : Packet->playerList)
	{
	}

	gen::EnterGameReq EnterGamePkt;
	EnterGamePkt.playerIdx = 0;
	Session->Send(&EnterGamePkt);
}
void gen::PacketHandler::EnterGameResPacketHandler(TSharedPtr<Session> Session, TSharedPtr<EnterGameRes> Packet)
{
	UManager::Net()->HandleSpawn(*Packet);
}

void gen::PacketHandler::LeaveGameResPacketHandler(TSharedPtr<Session> session, TSharedPtr<LeaveGameRes> packet)
{
}

void gen::PacketHandler::SpawnNotifyPacketHandler(TSharedPtr<Session> session, TSharedPtr<SpawnNotify> packet)
{
	UManager::Net()->HandleSpawn(*packet);
}

void gen::PacketHandler::DespawnNotifyPacketHandler(TSharedPtr<Session> session, TSharedPtr<DespawnNotify> packet)
{
	UManager::Net()->HandleDespawn(*packet);
}

void gen::PacketHandler::MoveResPacketHandler(TSharedPtr<Session> session, TSharedPtr<MoveRes> packet)
{
	UManager::Net()->HandleMove(*packet);
}
