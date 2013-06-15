//
//  SystemMessageWindowNode.cpp
//  StoryOfEren
//
//  Created by SungJinYoo on 6/15/13.
//
//

#include "Headers.pch"

SystemMessageWindowNode::SystemMessageWindowNode():m_ChatLogLabelList() {}

SystemMessageWindowNode::~SystemMessageWindowNode(){}

bool SystemMessageWindowNode::init()
{
    if(!CCNode::init()) return false;
    
    const CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    const CCSize contentSize = CCSize(0, winSize.height / 4); // screen size / 4
    this->setContentSize(contentSize);
    this->setAnchorPoint(CCPointUpperLeft);
    this->setPosition(ccp(0, winSize.height));

    return true;
}

void SystemMessageWindowNode::ClearMessages()
{
    std::for_each(this->m_ChatLogLabelList.begin(), this->m_ChatLogLabelList.end(), [](CCLabelTTF* label){
        label->removeFromParent();
        label->release();
    });
    
    this->m_ChatLogLabelList.clear();
}

void SystemMessageWindowNode::MessageReceived(std::string message)
{
    CCSize systemMessageBoxSize = this->getContentSize();

    int chatLogHeightLeft = systemMessageBoxSize.height;
    
    CCLabelTTF* newChatLogLabel = CCLabelTTF::create(message.c_str(), "thonburi", 10, CCSize(440, 0), kCCTextAlignmentLeft); // NOTE : height is autosized
    newChatLogLabel->setAnchorPoint(ccp(0.5, 1));
    newChatLogLabel->retain();
    
    CCFadeOut* fadeOut = CCFadeOut::create(3);
    newChatLogLabel->runAction(fadeOut);
    
    this->addChild(newChatLogLabel);
    this->m_ChatLogLabelList.push_back(newChatLogLabel);
    
    int index = 0;
    // NOTE : repositioning the labels
    for(index = this->m_ChatLogLabelList.size() - 1; index >= 0; index--)
    {
        if(chatLogHeightLeft <= 0)
        {
            break;
        }
        CCLabelTTF* label = this->m_ChatLogLabelList[index];
        int labelHeight = label->getContentSize().height;
        label->setPosition(ccp(240, chatLogHeightLeft));
        chatLogHeightLeft -= labelHeight;
    }
    // NOTE : remove oversize labels
    int numberOfLabelsToRemove = index;
    for(int i = 0; i <= numberOfLabelsToRemove; i++)
    {
        CCLabelTTF* labelToRelease = this->m_ChatLogLabelList.front();
        labelToRelease->removeFromParentAndCleanup(true);
        labelToRelease->release();
    }
    for(int i = 0; i <= numberOfLabelsToRemove; i++)
    {
        this->m_ChatLogLabelList.pop_front();
    }
    
    CCDelayTime* delay = CCDelayTime::create(3);
    CCCallFunc* clearMessages = CCCallFunc::create(this, callfunc_selector(SystemMessageWindowNode::ClearMessages));
    CCSequence* sequence = CCSequence::create(delay, clearMessages, NULL);
    sequence->setTag(ActionType_UI);
    this->stopActionByTag(ActionType_UI);
    this->runAction(sequence);
}
