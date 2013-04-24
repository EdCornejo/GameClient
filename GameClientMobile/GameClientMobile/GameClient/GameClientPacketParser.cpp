//
//  GameClientPacketParser.cpp
//  GameClient
//
//  Created by Sinhyub Kim on 2/13/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

GameClientPacketParser::GameClientPacketParser(PacketHandler* packetHandler):PacketParser(packetHandler)
{

}

GameClientPacketParser::~GameClientPacketParser()
{

}
    
BasePacket* GameClientPacketParser::CreatePacket(INT protocol, const INT64 bodySize)
{
    return new GamePacket(protocol, bodySize);
}

} // namespace flownet
