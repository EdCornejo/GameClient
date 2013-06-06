//
//  MonsterNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#ifndef __GameClientMobile__MonsterNode__
#define __GameClientMobile__MonsterNode__

class MonsterNode : public ActorNode
{
public:
    MonsterNode();
    virtual ~MonsterNode();

    virtual bool init();
    
    static MonsterNode* create(flownet::ActorID actorID);
};

#endif /* defined(__GameClientMobile__MonsterNode__) */
