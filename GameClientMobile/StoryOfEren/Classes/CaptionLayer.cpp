//
//  CaptionLayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/24/13.
//
//

#include "Headers.pch"

CaptionLayer::CaptionLayer() : m_StageType(StageType_NONE), m_CaptionLabel(nullptr), m_NameLabel(nullptr), m_CharacterImage(nullptr), m_CaptionInfoList(), m_Tier(0), m_CaptionInfoListIndex(0), m_CaptionIndex(0)
{

}

CaptionLayer::~CaptionLayer()
{
    CC_SAFE_RELEASE(this->m_CaptionLabel);
    CC_SAFE_RELEASE(this->m_NameLabel);
    CC_SAFE_RELEASE(this->m_CharacterImage);
    
    std::for_each(this->m_CaptionInfoList.begin(), this->m_CaptionInfoList.end(), [](CaptionInfo* info){
        delete info;
    });
    
    this->m_CaptionInfoList.clear();
}

bool CaptionLayer::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    // TODO : for test loaded tutorial script
    bool result = this->LoadCaptionInfoFromFile(this->GetCaptionInfoFileName(this->m_StageType).c_str(), this->m_Tier);
    
    if(!result) return false;
    
    CCSprite* background = CCSprite::create("ui/textbox/background.png");
    background->setAnchorPoint(CCPointLowerLeft);
    this->addChild(background);
    // set position of background? if needed
    CCSize backgroundSize = background->getContentSize();
    CCSize captionLabelSize = backgroundSize;
    captionLabelSize.width -= 20;
    captionLabelSize.height -= 20;
    
    this->m_CaptionLabel = CCLabelTTF::create("", "thonburi", 20, captionLabelSize, kCCTextAlignmentLeft);
    this->m_CaptionLabel->retain();
    this->m_CaptionLabel->setAnchorPoint(CCPointLowerLeft);
    this->m_CaptionLabel->setPosition(ccp(20, 0));
    
    // set position of caption label
    background->addChild(this->m_CaptionLabel);
    background->setZOrder(1);
    
    
    // set position of nameBox
    CCSprite* nameBox = CCSprite::create("ui/textbox/namebox.png");
    nameBox->setAnchorPoint(CCPointLowerLeft);
    nameBox->setPosition(ccp(20, backgroundSize.height - 20));
    this->addChild(nameBox);
    CCSize nameBoxSize = nameBox->getContentSize();
    nameBoxSize.width -= 10;
    nameBoxSize.height -= 10;
    
    this->m_NameLabel = CCLabelTTF::create("", "thonburi", 20, nameBoxSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    this->m_NameLabel->retain();
    this->m_NameLabel->setAnchorPoint(CCPointLowerLeft);
    
    nameBox->addChild(this->m_NameLabel);
    nameBox->setZOrder(2);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 10, true);
    this->setTouchEnabled(true);
    
    return true;
}

CaptionLayer* CaptionLayer::create(const flownet::StageType stageType, const int tier)
{
    CaptionLayer* captionLayer = new CaptionLayer();
    captionLayer->m_StageType = stageType;
    captionLayer->m_Tier = tier;
    
    if(captionLayer && captionLayer->init())
    {
        captionLayer->autorelease();
        return captionLayer;
    }
    else
    {
        delete captionLayer;
        captionLayer = nullptr;
        return nullptr;
    }
}

bool CaptionLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);

    int captionInfoListSize = this->m_CaptionInfoList.size();
    ASSERT_DEBUG(this->m_CaptionInfoListIndex <= captionInfoListSize);
    
    if(captionInfoListSize == this->m_CaptionInfoListIndex)
    {
        // NOTE : the caption is ended
        this->runAction(CCCallFunc::create(this, callfunc_selector(CaptionLayer::CaptionEnded)));
        return true;
    }
    
    CaptionInfo* currentCaptionInfo = this->m_CaptionInfoList[this->m_CaptionInfoListIndex];
    ASSERT_DEBUG(currentCaptionInfo);
    
    int captionListSize = currentCaptionInfo->m_CaptionList.size();
    ASSERT_DEBUG(this->m_CaptionIndex <= captionListSize);
    
    std::string currentCaption = currentCaptionInfo->m_CaptionList[this->m_CaptionIndex++];
    
    
    // NOTE : display the character illust
    if(this->m_CharacterImage)
    {
        this->m_CharacterImage->removeFromParent();
        this->m_CharacterImage->release();
    }
    
    std::string characterImageFileName = "illust/";
    
    if(currentCaptionInfo->m_Name.compare("player") == 0) {
        Player* actor = GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID());
        if(!actor || actor->GetGender() == Gender_Female)
        {
            characterImageFileName += "blank";
        }
        else
        {
            characterImageFileName += "blank";
        }
    }
    else {
        characterImageFileName += currentCaptionInfo->m_Name;
    }
    
    characterImageFileName += ".png";
    
    this->m_CharacterImage = CCSprite::create(characterImageFileName.c_str());
    this->m_CharacterImage->retain();
    this->m_CharacterImage->setZOrder(0);
    
    CCPoint characterPosition;
    if(currentCaptionInfo->m_CharacterPosition == CaptionCharacterPosition_LEFT)
    {
        const CCPoint leftSideIllustPosition = ccp(100, 160);
        characterPosition = leftSideIllustPosition;
    }
    else
    {
        const CCPoint rightSideIllustPosition = ccp(380, 160);
        characterPosition = rightSideIllustPosition;
    }
    this->m_CharacterImage->setPosition(characterPosition);
    
    this->addChild(this->m_CharacterImage);
    // NOTE : end of displaying character illust
    
    // NOTE : process the reserved keyword for text
    this->m_NameLabel->setString(this->ReplaceReservedKeyword(currentCaptionInfo->m_DisplayName).c_str());
    this->m_CaptionLabel->setString(this->ReplaceReservedKeyword(currentCaption).c_str());
    
    if(captionListSize == this->m_CaptionIndex)
    {
        this->m_CaptionInfoListIndex++;
        this->m_CaptionIndex = 0;
    }
    
    return true;
}

void CaptionLayer::OnLoad()
{
    BaseLayer::OnLoad();
    // NOTE : for initialize force call ccTouchEnded
    this->ccTouchBegan(nullptr, nullptr);
}

std::string CaptionLayer::GetCaptionInfoFileName(flownet::StageType stageType)
{
    std::string captionFileName = "script/";
    
    switch (stageType) {
    case flownet::StageType_Intro:
        captionFileName += "intro";
        break;
    default:
        break;
    }
    
    captionFileName += ".plist";
    
    return captionFileName;
}

// TODO : make return type of caption info
bool CaptionLayer::LoadCaptionInfoFromFile(const char * fileName, int tier)
{
    CaptionCharacterPosition m_CharacterPosition;
    std::string m_Name;
    std::string m_DisplayName;
    std::string m_State;
    CaptionList m_CaptionList;
    std::string m_AfterEvent;


    CCDictionary* dict = CCDictionary::createWithContentsOfFile(fileName);
    if(dict->count() == 0) return false;
    
    CCDictionary* captionsDict = static_cast<CCDictionary*>(dict->objectForKey("captions"));
    std::stringstream ss;
    ss << tier;
    std::string s = ss.str().c_str();
    
    CCArray* captionInfos = static_cast<CCArray*>(captionsDict->objectForKey(s.c_str()));
    
    CCObject* object;
    CCARRAY_FOREACH(captionInfos, object)
    {
        CCDictionary* captionInfoDict = static_cast<CCDictionary*>(object);
        // TO DO : multi region support comes here
        
        CaptionInfo* captionInfo = new CaptionInfo();
        captionInfo->m_CharacterPosition = static_cast<CaptionCharacterPosition>(atoi(static_cast<CCString*>(captionInfoDict->objectForKey("position"))->getCString()));
        captionInfo->m_Name = static_cast<CCString*>(captionInfoDict->objectForKey("name"))->getCString();
        captionInfo->m_DisplayName = static_cast<CCString*>(captionInfoDict->objectForKey("display_name_kr"))->getCString();
        captionInfo->m_State = static_cast<CCString*>(captionInfoDict->objectForKey("state"))->getCString();
        captionInfo->m_State = static_cast<CCString*>(captionInfoDict->objectForKey("state"))->getCString();
        CCArray* captions = static_cast<CCArray*>(captionInfoDict->objectForKey("caption_kr"));
        CCObject* captionObject;
        CCARRAY_FOREACH(captions, captionObject)
        {
            captionInfo->m_CaptionList.push_back(static_cast<CCString*>(captionObject)->getCString());
        }
        
        if(captionInfoDict->objectForKey("after_event"))
        {
            captionInfo->m_AfterEvent = static_cast<CCString*>(captionInfoDict->objectForKey("after_event"))->getCString();
        }
        
        this->m_CaptionInfoList.push_back(captionInfo);
    }
    delete dict;
    
    return true;
}

void CaptionLayer::CaptionEnded()
{
    this->removeFromParent();
}

std::string CaptionLayer::ReplaceReservedKeyword(std::string contentString)
{
    // NOTE : current reserved keywords are $PlayerName
    Vector<std::string>::type reservedKeywordList = {"$PlayerName", };

    Player* player = GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID());
    ASSERT_DEBUG(player);
    
    std::string::size_type foundIndex = std::string::npos;
    while((foundIndex = contentString.find("$PlayerName")) != std::string::npos)
    {
        contentString = contentString.replace(foundIndex, std::string("$PlayerName").size() , player->GetPlayerName().c_str());
    }

    return contentString;
}