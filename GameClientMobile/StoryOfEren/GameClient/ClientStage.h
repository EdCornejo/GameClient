//
//  File.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#ifndef __GameClientMobile__File__
#define __GameClientMobile__File__

namespace flownet
{
    
class ClientStage : public Stage
{
public:
    ClientStage(const Stage& stage, const STRING gameObjectName="ClientStage");
    virtual ~ClientStage();

};
    
} // namespace flownet

#endif /* defined(__GameClientMobile__File__) */
