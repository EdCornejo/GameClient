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
    PlayerNode();
    virtual ~PlayerNode();
    
    virtual bool init();
    
    static PlayerNode* create(flownet::ActorID);

    virtual void ChangeWand(flownet::ItemType itemType);
    virtual void ChangeHat(flownet::ItemType itemType);
    virtual void ChangeRobe(flownet::ItemType itemType);
    virtual void ChangeCloak(flownet::ItemType itemType);
    virtual void ChangeShoes(flownet::ItemType itemType);
    virtual void ChangeRing(flownet::ItemType itemType);
};

#endif /* defined(__GameClientMobile__PlayerNode__) */
