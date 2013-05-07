//
//  FCPacketParser.cpp
//  GameClient
//
//  Created by Kim Sinhyub on 4/30/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

FCPacketParser::FCPacketParser(PacketHandler* packetHandler):PacketParser(packetHandler)
{

}

FCPacketParser::~FCPacketParser()
{

}
    
BasePacket* FCPacketParser::CreatePacket(INT protocol, const INT64 bodySize)
{
    return new GamePacket(protocol, bodySize);
}

} // namespace flownet
