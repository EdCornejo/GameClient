//
//  ChatMessage.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/22/13.
//
//

#include "GameClientHeaders.pch"

ChatMessage::ChatMessage(flownet::ActorID senderID, flownet::STRING senderName, flownet::STRING message): m_SenderID(senderID), m_SenderName(senderName), m_Message(message)
{

}

ChatMessage::~ChatMessage()
{

}