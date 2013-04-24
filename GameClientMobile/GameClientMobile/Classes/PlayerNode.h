//
//  PlayerNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#ifndef __GameClientMobile__PlayerNode__
#define __GameClientMobile__PlayerNode__

class PlayerNode : public ActorNode
{
public:
    PlayerNode(flownet::ActorID actorID);
    PlayerNode(flownet::Actor* actor);
    virtual ~PlayerNode();

    virtual CCSprite* LoadHighLightImage();
};

#endif /* defined(__GameClientMobile__PlayerNode__) */
