//
//  EffectLayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#ifndef __GameClientMobile__EffectLayer__
#define __GameClientMobile__EffectLayer__

class EffectLayer : public BaseLayer
{
public:
    virtual bool init();
    CREATE_FUNC(EffectLayer);

    virtual void update(float deltaTime);
    
public:
    void AddMovingEffect(CCPoint statingPoint, CCPoint destinationPoint);
    void AddMagicGuideEffect(SpellType skillType, CCPoint startingPoint, CCPoint destinationPoint);
    void AddMagicEffect(SpellType skillType, CCPoint startingPoint, CCPoint destinationPoint);
};

#endif /* defined(__GameClientMobile__EffectLayer__) */
