//
//  NPCNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/28/13.
//
//

#ifndef __GameClientMobile__NPCNode__
#define __GameClientMobile__NPCNode__

class NPCNode : public ActorNode
{
public:
    NPCNode();
    virtual ~NPCNode();
    
    virtual bool init() override;
    
    static NPCNode* create(flownet::ActorID actorID);
};

#endif /* defined(__GameClientMobile__NPCNode__) */
