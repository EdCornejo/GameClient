//
//  ChattingNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/21/13.
//
//

#include "Headers.pch"


bool ChattingNodeTextFieldDelegate::init()
{
    return true;
}

bool ChattingNodeTextFieldDelegate::onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen)
{
    if(*text == '\n')
    {
        if(strlen(pSender->getString()) != 0)
        {
            GameClient::Instance().GetClientObject().SendCSRequestSendMessageToStagePlayers(GameClient::Instance().GetClientStage()->GetStageID(), GameClient::Instance().GetMyActorID(), pSender->getString());
            pSender->setString("");
        }
    }
    return false;
}

ChattingNode::ChattingNode() : m_HidingPart(nullptr), m_Delegate(nullptr), m_Background(nullptr), m_TextField(nullptr), m_ChatLog(nullptr), m_ChatLogLabelList() {}

ChattingNode::~ChattingNode()
{
    if(this->m_HidingPart)
    {
        this->m_HidingPart->release();
        this->m_HidingPart = nullptr;
    }
    if(this->m_Delegate)
    {
        this->m_Delegate->release();
        this->m_Delegate = nullptr;
    }
    if(this->m_Background)
    {
        this->m_Background->release();
        this->m_Background = nullptr;
    }
    if(this->m_TextField)
    {
        this->m_TextField->release();
        this->m_TextField = nullptr;
    }
    if(this->m_ChatLog)
    {
        this->m_ChatLog->release();
        this->m_ChatLog = nullptr;
    }
    
    std::for_each(m_ChatLogLabelList.begin(), m_ChatLogLabelList.end(), [this](CCLabelTTF* node){
        node->removeFromParentAndCleanup(true);
        node->release();
    });
    m_ChatLogLabelList.clear();
}

bool ChattingNode::init()
{
    if(!CCNode::init()) return false;
    
    // TO DO : replace thonburi to our font
    this->m_HidingPart = CCNode::create();
    this->m_HidingPart->retain();
    this->m_HidingPart->setVisible(false);
    
    this->m_Background = CCSprite::create("ui/chatting/background.png");
    this->m_Background->retain();
    this->m_Background->setOpacity(150);
    this->m_Background->setAnchorPoint(CCPointUpperLeft);
    this->m_Background->setPosition(ccp(0, 320));
    this->m_HidingPart->addChild(this->m_Background);

    this->m_Delegate = ChattingNodeTextFieldDelegate::create();
    this->m_Delegate->retain();

    CCSize backgroundSize = this->m_Background->getContentSize();

    CCSprite* textBox = CCSprite::create("ui/chatting/textbox.png");
    textBox->setPosition(ccp(240, backgroundSize.height-24));
    this->m_Background->addChild(textBox);
    
    this->m_TextField = CCTextFieldTTF::textFieldWithPlaceHolder("", CCSize(440, 24), kCCTextAlignmentLeft, "thonburi", 20);
    this->m_TextField->retain();
    this->m_TextField->setDelegate(this->m_Delegate);
    this->m_TextField->setPosition(ccp(240, backgroundSize.height - 24));
    
    this->m_Background->addChild(this->m_TextField);
    
    
    ChatMessageDeque& chatLog = GameClient::Instance().GetChatMessageLog();
    
    std::for_each(chatLog.begin(), chatLog.end(), [this](ChatMessage* message){
        this->MessageReceived(message->m_SenderName, message->m_Message);
    });
    
    CCSprite* chatButtonNormal = CCSprite::create("ui/chatting/chat_button.png");
    CCSprite* chatButtonActive = CCSprite::create("ui/chatting/chat_button.png");
    chatButtonActive->setColor(ccWHITE);
    
    CCMenuItemSprite* chatButton = CCMenuItemSprite::create(chatButtonNormal, chatButtonActive, this, menu_selector(ChattingNode::ToggleChat));
    CCMenu* menu = CCMenu::create(chatButton, NULL);
    menu->setPosition(ccp(450, 30));
    menu->setTouchPriority(kCCMenuHandlerPriority - 3);
    
    this->addChild(this->m_HidingPart);
    this->addChild(menu);
    
    return true;
}

bool ChattingNode::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    CCRect textFieldRect = GetRect(this->m_TextField);
    textFieldRect.origin = this->m_Background->convertToWorldSpace(textFieldRect.origin);
    if(this->m_HidingPart->isVisible() && textFieldRect.containsPoint(touch->getLocation()))
    {
        this->m_TextField->attachWithIME();
        return true;
    }

    return true;
}

void ChattingNode::ToggleChat(CCObject* sender)
{
    this->m_HidingPart->isVisible() ? this->HideChat() : this->ShowChat();
}

void ChattingNode::ShowChat()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 2, true);
    this->m_HidingPart->setVisible(true);
    this->m_TextField->attachWithIME();
}

void ChattingNode::HideChat()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    this->m_TextField->setString("");
    this->m_TextField->detachWithIME();
    this->m_HidingPart->setVisible(false);
}

void ChattingNode::MessageReceived(flownet::STRING senderName, flownet::STRING message)
{
    CCSize chatLogBoxSize = this->m_Background->getContentSize();
    chatLogBoxSize.height -= 38; // height of textfield

    int chatLogHeightLeft = chatLogBoxSize.height;
    
    std::string visualString = senderName.c_str();
    visualString += " : ";
    visualString += message.c_str();
    
    CCLabelTTF* newChatLogLabel = CCLabelTTF::create(visualString.c_str(), "thonburi", 20, CCSize(440, 0), kCCTextAlignmentLeft); // NOTE : height is autosized
    newChatLogLabel->setAnchorPoint(ccp(0.5, 1));
    newChatLogLabel->retain();
    this->m_Background->addChild(newChatLogLabel);
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

}