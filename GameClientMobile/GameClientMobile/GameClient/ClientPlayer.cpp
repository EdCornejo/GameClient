//
//  ClientPlayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/19/13.
//
//

#include "GameClientHeaders.pch"

namespace flownet
{
    
ClientPlayer::ClientPlayer(ActorID playerID, const Gender gender, const POINT spawnPosition, const STRING& gameObjectName):Player(playerID,gender,spawnPosition,gameObjectName)
{
    
}

ClientPlayer::ClientPlayer(const Player& player,const STRING& gameObjectName):Player(player,gameObjectName)
{
    
}

ClientPlayer::~ClientPlayer()
{
    
}
    
bool ClientPlayer::IsStateMoving()
{
    return this->GetActorState() == ActorState_Moving;
}

bool ClientPlayer::IsStateAttacking()
{
    return this->GetActorState() == ActorState_Attacking;
}

bool ClientPlayer::IsStateCasting()
{
    return this->GetActorState() == ActorState_Casting;
}
    
bool ClientPlayer::IsStateIdle()
{
    return this->GetActorState() == ActorState_Idle;
}
    
void ClientPlayer::ChangeToMovingState()
{
//    CCLOG("changed to moving state");
    this->SetActorState(ActorState_Moving);
}
    
void ClientPlayer::ChangeToIdleState()
{
//    CCLOG("changed to idle state");
    this->SetActorState(ActorState_Idle);
}

void ClientPlayer::ChangeToCastingState()
{
//    CCLOG("changed to casting state");
    this->SetActorState(ActorState_Casting);
}

void ClientPlayer::ChangeToAttackingState()
{
//    CCLOG("changed to attacking state");
    this->SetActorState(ActorState_Attacking);
}

void ClientPlayer::ChangeToDeadState()
{
    CCLOG("changed to dead state");
    this->SetActorState(ActorState_Dead);
}

void ClientPlayer::OnAttacked(FLOAT damageAmount)
{
    this->m_HealthPoint -= damageAmount;
    if(this->m_HealthPoint < 0)
    {
        this->m_HealthPoint = 0;
    }
}

BOOL ClientPlayer::UseItem(Stage* stage, const ItemID itemID)
{
    Item* item = m_Stash.FindItem(itemID);
    if( item == nullptr )
    {
//        ASSERT_DEBUG(item!=nullptr);
        return false;
    }
    
    BOOL result = item->IsUsable(stage, this);
    if( result == false )
    {
        return false;
    }
    
    ItemGroup itemGroup = ItemGroup_None;
    InventorySlot inventorySlot = InventorySlot_None;
    
    BOOL findResult = m_Inventory.FindInventorySlot(itemID, itemGroup, inventorySlot);
    if( findResult == false )
    {
        return false;
    }
    
    result = m_Inventory.UnregisterStashItem(itemID, itemGroup, inventorySlot);
    if( result == false )
    {
        return false;
    }
    
    // NOTE : Do NOT Call Item->Use from Client Player
    //          Item Effects are notifing from server
    //item->Use(stage, this);
    m_Stash.DeleteAndRemoveItem(item->GetItemID());

    return true;
}

BOOL ClientPlayer::EquipItem(Stage* stage, const EquipmentSlot equipmentSlot, const ItemID itemID)
{
    Item* item = m_Stash.FindItem(itemID);
    if( item == nullptr )
    {
        ASSERT_DEBUG(item!=nullptr);
        return false;
    }
    
    BOOL result = item->IsEquipable(equipmentSlot);
    if( result == false )
    {
        ASSERT_DEBUG(result == true);
        return false;
    }
    
    InventorySlot equipItemInventorySlot = InventorySlot_None;
    ItemGroup itemGroup = ItemGroup_None;
    
    BOOL isEquipItemFromInventory = m_Inventory.FindInventorySlot(itemID, itemGroup, equipItemInventorySlot);
    
    const ItemID unequipItemID = m_EquipmentList.UnequipItem(equipmentSlot);
    
    // NOTE : Do NOT Call Item->Use from Client Player
    //          Item Effects are notifing from server
//    Item* unequipedItem = m_Stash.FindItem(unequipItemID);
//    unequipedItem->UnEquip(stage, this);
//    
    result = m_EquipmentList.EquipItem(equipmentSlot, itemID);
    // NOTE : Do NOT Call Item->Use from Client Player
    //          Item Effects are notifing from server
//    if( result == true)
//    {
//        Item* equipItem = m_Stash.FindItem(unequipItemID);
//        equipItem->Equip(stage, this);
//    }
    
    if( result == false )
    {
        ASSERT_DEBUG(result == false);
        return false;
    }

    if( isEquipItemFromInventory == true )
    {
        m_Inventory.UnregisterStashItem(itemID, ItemGroup_Equipment, equipItemInventorySlot);
        if( unequipItemID != ItemID_None )
        {
            m_Inventory.RegisterStashItem(unequipItemID, ItemGroup_Equipment, equipItemInventorySlot);
        }
    }
    else
    {
        m_Inventory.RegisterStashItemToEmptySlot(unequipItemID);
    }
    
    return true;
}

ItemID ClientPlayer::UnEquipItem(Stage* stage, const EquipmentSlot equipmentSlot)
{
    InventorySlot emptyInventorySlot = InventorySlot_None;
    
    BOOL emptyResult = m_Inventory.FindEmptyInventorySlot(ItemGroup_Equipment, emptyInventorySlot);
    if( emptyResult == false )
    {
        return ItemID_None;
    }
    
    const ItemID unequipedItem = m_EquipmentList.UnequipItem(equipmentSlot);
    if( unequipedItem == ItemID_None)
    {
        return ItemID_None;
    }
    else
    {
        // NOTE : Do NOT Call Item->Use from Client Player
        //          Item Effects are notifing from server
        //    Item* unequipedItem = m_Stash.FindItem(unequipItemID);
        //    unequipedItem->UnEquip(stage, this);
    }
    
    BOOL result = m_Inventory.RegisterStashItemToEmptySlot(unequipedItem);
    if( result == false )
    {
        ASSERT_DEBUG(result == true);
        
    }
    
    return unequipedItem;
}


//void ClientPlayer::RequestFireSpell(const ActorID targetID)
//{
//    if( this->SpellStackEmpty() )
//    {
//        return;
//    }
//    
//    GameClient::Instance().GetClientObject().SendCSRequestFireSpell(GameClient::Instance().GetClientStage()->GetStageID(), this->GetActorID(), this->PopSpell(), targetID);
//}

} // namespace flownet

