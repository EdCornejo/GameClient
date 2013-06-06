//
//  EffectLayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#include "Headers.pch"

bool EffectLayer::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    
    return true;
}

void EffectLayer::update(float deltaTime)
{

}

void AddMovingEffect(CCPoint statingPoint, CCPoint destinationPoint)
{
    // TO DO : when moved add moving effect to layer
}

void AddMagicGuideEffect(SpellType spellType, CCPoint startingPoint, CCPoint destinationPoint)
{
    // TO DO : when magic fires set GuideLine for magic for testing
}

void AddMagicEffect(SpellType spellType, CCPoint startingPoint, CCPoint destinationPoint)
{
    // TO DO : when magic fires add MagicEffect from player(startingPoint) to monster(destinationPoint)
}
