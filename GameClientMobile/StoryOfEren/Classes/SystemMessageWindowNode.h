//
//  SystemMessageWindowNode.h
//  StoryOfEren
//
//  Created by SungJinYoo on 6/15/13.
//
//

#ifndef __StoryOfEren__SystemMessageWindowNode__
#define __StoryOfEren__SystemMessageWindowNode__

class SystemMessageWindowNode : public CCNode
{
private:
    typedef flownet::Deque<CCLabelTTF*>::type ChatLogLabelDeque;
    
    ChatLogLabelDeque m_ChatLogLabelList;
    
public:
    SystemMessageWindowNode();
    virtual ~SystemMessageWindowNode();
    
    virtual bool init();
    
    CREATE_FUNC(SystemMessageWindowNode);
    
private:
    void ClearMessages();
    
public:
    void MessageReceived(std::string message);
};

#endif /* defined(__StoryOfEren__SystemMessageWindowNode__) */
