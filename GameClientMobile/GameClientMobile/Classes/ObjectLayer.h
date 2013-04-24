//
//  ObjectLayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#ifndef __GameClientMobile__ObjectLayer__
#define __GameClientMobile__ObjectLayer__

class ObjectLayer : public BaseLayer
{
public:
    virtual bool init();
    CREATE_FUNC(ObjectLayer);
    
    virtual void update(float deltaTime);
};

#endif /* defined(__GameClientMobile__ObjectLayer__) */
