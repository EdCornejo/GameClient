//
//  EquipmentNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/9/13.
//
//

#include "Headers.pch"

EquipmentNode::EquipmentNode() : m_IsOpen(false), m_SlideButton(nullptr), m_Body(nullptr), m_ActorNode(nullptr), m_ItemSlotNodeList(), m_TrackingItemSlotNode(nullptr), m_HighlightedItemSlotNode(nullptr), m_TrackingItemSlotTouchedTime(0) {}


EquipmentNode::~EquipmentNode()
{
    if(this->m_SlideButton)
    {
        this->m_SlideButton->release();
        this->m_SlideButton = nullptr;
    }
    if(this->m_Body)
    {
        this->m_Body->release();
        this->m_Body = nullptr;
    }
    if(this->m_ActorNode)
    {
        this->m_ActorNode->release();
        this->m_ActorNode = nullptr;
    }
    
    std::for_each(this->m_ItemSlotNodeList.begin(), this->m_ItemSlotNodeList.end(), [this](ItemSlotNode* node){
        node->release();
    });
    
    this->m_ItemSlotNodeList.clear();
}

bool EquipmentNode::init()
{
    this->m_SlideButton = CCSprite::create("ui/inventory/equipment_slide_button.png");
    this->m_SlideButton->retain();
    this->m_Body = CCSprite::create("ui/inventory/equipment_background.png");
    this->m_Body->retain();

    this->m_SlideButton->setAnchorPoint(CCPointMidLeft);
    this->m_SlideButton->setPosition(ccp(-1, -this->m_Body->getContentSize().height / 2)); // set position of slide button here

    this->addChild(this->m_SlideButton);
    
    this->m_Body->setAnchorPoint(CCPointUpperRight);
    
    this->addChild(this->m_Body);
    

    // for changing order of render
    
    // add ItemSlotNodes to body
    ItemSlotNode* slotForSize = ItemSlotNode::create(ItemType_None, ItemID_None, InventorySlot_None);
    CCRect slotRect = GetRect(slotForSize);
    
    const float ItemSlotSizeY = slotRect.size.height;
    
    delete slotForSize;
    
    // NOTE : first initialize inventory slot with empty items
    const int perRow = 2;
    for(int i = 0; i < EquipmentSlot_Max; i++)
    {
        ItemSlotNode* slotNode = ItemSlotNode::create(ItemType_None, ItemID_None, static_cast<EquipmentSlot>(i));
       
        slotNode->setAnchorPoint(CCAnchorPointMid);
        CCPoint slotPosition;
        slotPosition.x = ItemSlotPositionX + (148 * (i % perRow));
        slotPosition.y = ItemSlotPositionY + ((ItemSlotMargin + ItemSlotSizeY) * (i / perRow));
        
        slotNode->setPosition(slotPosition);
        if(i % perRow == 0)
        {
            slotNode->setAnchorPoint(CCPointUpperRight);
        }
        this->m_ItemSlotNodeList.push_back(slotNode);
        this->m_Body->addChild(slotNode);
    }
    // end of add ItemSlotNodes to body
    
    // Initialize with actor's inventory
    this->Update();
    
    return true;
}

EquipmentNode* EquipmentNode::create()
{
    EquipmentNode* newNode = new EquipmentNode();
    if(newNode && newNode->init())
    {
        newNode->autorelease();
        return newNode;
    }
    else
    {
        delete newNode;
        newNode = nullptr;
        return nullptr;
    }
}

bool EquipmentNode::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    
    CCRect slideButtonRect = GetRectForAnchorMidLeft(this->m_SlideButton);
    slideButtonRect.origin = this->convertToWorldSpace(slideButtonRect.origin);
    if(slideButtonRect.containsPoint(touch->getLocation()))
    {
        this->Slide();
        return true;
    }
    
    ItemSlotNode* selectedItemSlot = this->FindSelectedItemSlotNode(touch->getLocation());
    if(this->m_HighlightedItemSlotNode && selectedItemSlot != this->m_HighlightedItemSlotNode)
    {
        this->m_HighlightedItemSlotNode->ResetHighlight();
    }
    
    if(selectedItemSlot)
    {
        this->m_TrackingItemSlotTouchedTime = GameClient::Instance().GetClientTimer().Check();
        this->m_TrackingItemSlotNode = selectedItemSlot;
        return true;
    }
    
    CCRect bodyRect = GetRectForAnchorUpperRight(this->m_Body);
    // TO DO : change here ! or change anchor points
    bodyRect.origin = this->m_Body->getParent()->convertToWorldSpace(bodyRect.origin);
    if(bodyRect.containsPoint(touch->getLocation()))
    {
        return true;
    }
    
    return false;
}

void EquipmentNode::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    // if there is a tracking node
    if(this->m_TrackingItemSlotNode)
    {
        ItemSlotNode* selectedItemSlot = this->FindSelectedItemSlotNode(touch->getLocation());
        
        GameClient& client = GameClient::Instance();
        
        // if the selected item was tracking node itself
        if(selectedItemSlot == this->m_TrackingItemSlotNode)
        {
            // if the selected item was highlighted node use it
            if(selectedItemSlot == this->m_HighlightedItemSlotNode)
            {
                this->m_HighlightedItemSlotNode->ResetHighlight();
                this->m_HighlightedItemSlotNode = nullptr;
                
                client.GetClientObject().SendCSRequestUnEquipItem(client.GetClientStage()->GetStageID(), client.GetMyActorID(), this->m_TrackingItemSlotNode->GetEquipmentSlot());
            }
            // if the selected item was not highlighted highlight it
            else
            {
                // if ItemType is not None highlight
                if(this->m_TrackingItemSlotNode->GetItemType() != ItemType_None)
                {
                    this->m_HighlightedItemSlotNode = this->m_TrackingItemSlotNode;
                    this->m_HighlightedItemSlotNode->Highlight();
                }
            }
        }

        this->m_TrackingItemSlotNode->ResetMoving(); // Reset to ResetMoving, ResetHighlight
        this->m_TrackingItemSlotNode = nullptr;
    }
}

ItemSlotNode* EquipmentNode::FindSelectedItemSlotNode(cocos2d::CCPoint touchLocation)
{
    ItemSlotNode* selectedItemSlot = nullptr;
        // NOTE : check for the touch is over the another slot
    for(int i = 0; i < this->m_ItemSlotNodeList.size(); ++i){
        ItemSlotNode* itemSlot = this->m_ItemSlotNodeList[i];
        CCRect itemSlotRect = GetRect(itemSlot);
        itemSlotRect.origin = this->m_Body->convertToWorldSpace(itemSlotRect.origin);
        if(itemSlotRect.containsPoint(touchLocation))
        {
            selectedItemSlot = itemSlot;
            break;
        }
    }
    
    return selectedItemSlot;
}

void EquipmentNode::Update()
{
    // NOTE : if ActorNode unloaded then make it
    ActorID myActorID = GameClient::Instance().GetMyActorID();
    if(myActorID == ActorID_None) return;
    
    Actor* actor = GameClient::Instance().GetClientStage()->FindPlayer(myActorID);
    if(actor && !this->m_ActorNode)
    {
        this->m_ActorNode = PlayerNode::create(GameClient::Instance().GetMyActorID());
        this->m_ActorNode->setAnchorPoint(CharacterAnchorPoint);
        this->m_ActorNode->setPosition(this->m_Body->getContentSize().width / 2, 30);
        this->m_ActorNode->retain();
        this->m_Body->addChild(this->m_ActorNode);
    }
    
    // TODO : 매번 여기서 갱신을 해주는게 나중에 속도 문제가 있을 수 있다. 추후에 성능향상을 위한다면 이곳도 리팩토링의 대상이 된다
    if(this->m_ActorNode)
    {
        this->m_ActorNode->Reload();
    }
    
    // NOTE : update EquipmentSlots with new information
    if(actor)
    {
        actor->GetEquipmentList().ForAllItemSlots([this, actor](EquipmentSlot equipmentSlot, ItemID itemID){
            if(itemID == ItemID_None)
            {
                this->m_ItemSlotNodeList[equipmentSlot]->ChangeItemTypeAndItemID(ItemType_None, ItemID_None);
            }
            else
            {
                const Item* item = actor->GetStash().FindItem(itemID);
                this->m_ItemSlotNodeList[equipmentSlot]->ChangeItemTypeAndItemID(item->GetItemType(), item->GetItemID());
            }
        });
    }
}

void EquipmentNode::OnScrollButtonClicked()
{
    if(!this->m_IsOpen)
    {
        this->Slide();
    }
}

void EquipmentNode::Slide()
{
    if(this->m_IsOpen)
    {
        CCLOG("slide in");

        CCPoint slidePosition = CCPoint(PositionX, PositionY);
        CCEaseOut* moveIn = CCEaseOut::create(CCMoveTo::create(0.2, slidePosition), 3.f);
        moveIn->setTag(ActionType_UI);
        this->stopActionByTag(ActionType_UI);
        this->runAction(moveIn);
    }
    else
    {
        CCLOG("slide out");
        CCPoint slidePosition = CCPoint(PositionX + this->m_Body->getTextureRect().size.width, PositionY);
        CCEaseOut* moveOut = CCEaseOut::create(CCMoveTo::create(0.2, slidePosition), 3.f);
        moveOut->setTag(ActionType_UI);
        this->stopActionByTag(ActionType_UI);
        this->runAction(moveOut);
        // TO DO : slide out
    }
    
    this->m_IsOpen = !this->m_IsOpen;
}