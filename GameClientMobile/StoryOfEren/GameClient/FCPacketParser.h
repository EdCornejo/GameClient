//
//  FCPacketParser.h
//  GameClient
//
//  Created by Kim Sinhyub on 4/30/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__FCPacketParser__
#define __GameClient__FCPacketParser__

namespace flownet
{

class FCPacketParser : public PacketParser
{
private:

public:
    FCPacketParser(PacketHandler* packetHandler);
    virtual ~FCPacketParser();

protected:
    virtual BasePacket*     CreatePacket(INT protocol, const INT64 bodySize) override;    

};

} // namespace flownet

#endif /* defined(__GameClient__FCPacketParser__) */
