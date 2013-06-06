//
//  ChattingNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/21/13.
//
//

#ifndef __GameClientMobile__ChattingNode__
#define __GameClientMobile__ChattingNode__

class ChattingNodeTextFieldDelegate : public CCObject, public CCTextFieldDelegate
{
public:
    virtual bool init();
    CREATE_FUNC(ChattingNodeTextFieldDelegate);
    
    virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
};

class ChattingNode : public CCNode, public CCTouchDelegate
{
    typedef flownet::Deque<CCLabelTTF*>::type ChatLogLabelDeque;

private:
    CCNode* m_HidingPart;
    ChattingNodeTextFieldDelegate* m_Delegate;
    CCSprite* m_Background;
    CCTextFieldTTF* m_TextField;
    CCLabelTTF* m_ChatLog;
    ChatLogLabelDeque m_ChatLogLabelList;

public:
    ChattingNode();
    virtual ~ChattingNode();
    
    virtual bool init();
    
    CREATE_FUNC(ChattingNode);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
private:
    void ToggleChat(CCObject* sender);
    void ShowChat();
    void HideChat();
    
public:
    void MessageReceived(flownet::STRING senderName, flownet::STRING message);
};

#endif /* defined(__GameClientMobile__ChattingNode__) */
