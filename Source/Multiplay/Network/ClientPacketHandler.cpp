#include "Session.h"
#include "generated/ClientPacketHandler.gen.hpp"

#include "Managers/Manager.h"
#include "Managers/Network.h"

using namespace gen;

bool gen::PacketHandler::LoginResPacketHandler(TSharedPtr<FSession> Session, TSharedPtr<LoginRes> Packet)
{
	EnterGameReq EnterGamePkt;
	EnterGamePkt.playerIdx = 0;
	Session->Send(&EnterGamePkt);
	return false;
}
bool gen::PacketHandler::EnterGameResPacketHandler(TSharedPtr<FSession> Session, TSharedPtr<EnterGameRes> Packet)
{
	UManager::Net()->HandleSpawn(*Packet);
	return false;
}

bool gen::PacketHandler::LeaveGameResPacketHandler(TSharedPtr<FSession> session, TSharedPtr<LeaveGameRes> packet)
{
	return false;
}

bool gen::PacketHandler::SpawnNotifyPacketHandler(TSharedPtr<FSession> session, TSharedPtr<SpawnNotify> packet)
{
	UManager::Net()->HandleSpawn(*packet);
	return false;
}

bool gen::PacketHandler::DespawnNotifyPacketHandler(TSharedPtr<FSession> session, TSharedPtr<DespawnNotify> packet)
{
	UManager::Net()->HandleDespawn(*packet);
	return false;
}

bool gen::PacketHandler::MoveResPacketHandler(TSharedPtr<FSession> session, TSharedPtr<MoveRes> packet)
{
	UManager::Net()->HandleMove(*packet);
	return false;
}
