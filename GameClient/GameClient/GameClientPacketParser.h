//
//  GameClientPacketParser.h
//  GameClient
//
//  Created by Sinhyub Kim on 2/13/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__GameClientPacketParser__
#define __GameClient__GameClientPacketParser__

namespace flownet
{

class GameClientPacketParser : public PacketParser
{
private:

public:
    GameClientPacketParser(PacketHandler* packetHandler);
    virtual ~GameClientPacketParser();

protected:
    virtual BasePacket*     CreatePacket(INT protocol, const INT64 bodySize) override;    

};

} // namespace flownet

#endif /* defined(__GameClient__GameClientPacketParser__) */
