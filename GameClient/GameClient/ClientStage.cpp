//
//  ClientStage.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/19/13.
//
//

#include "GameClientHeaders.pch"

namespace flownet
{

ClientStage::ClientStage(const Stage& stage,const STRING gameObjectName):Stage(stage, gameObjectName)
{
    PlayerMap tempPlayerMap = this->m_PlayerMap;
    MonsterMap tempMonsterMap = this->m_MonsterMap;
    
    this->m_PlayerMap.clear();
    this->m_MonsterMap.clear();
    
    std::for_each(tempPlayerMap.begin(), tempPlayerMap.end(), [this,&stage](PlayerMap::value_type& playerInfoPair){
        Player* player = playerInfoPair.second;
        
        ASSERT_DEBUG(player != nullptr);
        
        ClientPlayer* clientPlayer = new ClientPlayer(*player);
        
        this->AddPlayer(clientPlayer->GetActorID(), clientPlayer);
       
        delete player;
    });
    tempPlayerMap.clear();
    
    // NOTE : replacing Monster object to ClientMonster object
    std::for_each(tempMonsterMap.begin(), tempMonsterMap.end(), [this](MonsterMap::value_type& monsterInfoPair){
        Monster* monster = monsterInfoPair.second;
        
        ASSERT_DEBUG(monster != nullptr);
        
        ClientMonster* clientMonster = new ClientMonster(*monster);
        this->AddMonster(clientMonster->GetActorID(), clientMonster);
        
        delete monster;
    });
    tempMonsterMap.clear();
}

ClientStage::~ClientStage()
{
    std::for_each(m_PlayerMap.begin(), m_PlayerMap.end(), [](PlayerMap::value_type& valuePair)
    {
        delete valuePair.second;
    });
    m_PlayerMap.clear();
    std::for_each(m_MonsterMap.begin(), m_MonsterMap.end(), [](MonsterMap::value_type& valuePair)
    {
        delete valuePair.second;
    });
    m_MonsterMap.clear();
}

} // namespace flownet
