//
//  ChatMessage.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/22/13.
//
//

#ifndef __GameClientMobile__ChatMessage__
#define __GameClientMobile__ChatMessage__

struct ChatMessage
{
    flownet::ActorID m_SenderID;
    flownet::STRING m_SenderName;
    flownet::STRING m_Message;
    
    ChatMessage(flownet::ActorID senderID, flownet::STRING senderName, flownet::STRING message);
    ~ChatMessage();
};

typedef flownet::Deque<ChatMessage*>::type ChatMessageDeque;

#endif /* defined(__GameClientMobile__ChatMessage__) */
