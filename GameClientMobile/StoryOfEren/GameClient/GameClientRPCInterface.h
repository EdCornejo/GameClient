//
//  GameClientRPCInterface.h
//  GameClientMobile
//
//  Created by Sinhyub Kim on 2/28/13.
//
//

#ifndef GameClientMobile_GameClientRPCInterface_h
#define GameClientMobile_GameClientRPCInterface_h

namespace flownet
{

// Pure virtual class, just define interface
class GameClientRPCInterface
{
protected:
    GameClientRPCInterface(){}
public:
    virtual ~GameClientRPCInterface(){}
    
    virtual void ShowSystemMessage(const STRING& message) const = 0;

public:
    #include "GameSCVirtualProtocolHandlerDeclaration.hpp"
    #include "GameFCVirtualProtocolHandlerDeclaration.hpp"
};

} // namespace


#endif
