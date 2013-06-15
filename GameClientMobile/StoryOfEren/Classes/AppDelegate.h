//
//  GameClientMobileAppDelegate.h
//  GameClientMobile
//
//  Created by Sinhyub Kim on 2/27/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "CCApplication.h"

#include "GameClientHeaders.pch"

/**
@brief    The cocos2d Application.

The reason to implement with private inheritance is to hide some interface details of CCDirector.
*/
class  AppDelegate : private cocos2d::CCApplication, flownet::GameClientRPCInterface
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function is called when the application enters the background
    @param  the pointer of the application instance
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function is called when the application enters the foreground
    @param  the pointer of the application instance
    */
    virtual void applicationWillEnterForeground();
    
    bool InitializeConnection();
    bool DisconnectCSAndConnectCFConnection() const;
    bool DisconnectCFAndConnectCSConnection() const;
    
    virtual void ShowSystemMessage(const flownet::STRING& message) const override;
    
    // flownet RPC Receiver Interface
    #include "GameSCOverrideProtocolHandlerDeclaration.hpp"
    #include "GameFCOverrideProtocolHandlerDeclaration.hpp"
};

#endif // _APP_DELEGATE_H_

