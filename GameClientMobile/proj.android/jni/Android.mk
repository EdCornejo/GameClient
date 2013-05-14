LOCAL_PATH := $(call my-dir)

BOOST_DIR := $(LOCAL_PATH)/../../../../flownet/BoostMobile/android
BOOST_SRC_DIR := ../../../../flownet/BoostMobile/android
FLOWNET_DIR := $(LOCAL_PATH)/../../../../flownet/flownet
FLOWNET_SRC_DIR := ../../../../flownet/flownet
GAMESHARED_DIR := $(LOCAL_PATH)/../../../../GameShared/GameShared
GAMESHARED_SRC_DIR := ../../../../GameShared/GameShared
GAMECLIENT_DIR := $(LOCAL_PATH)/../../GameClientMobile
GAMECLIENT_SRC_DIR := ../../GameClientMobile
LIB_DIR := $(LOCAL_PATH)/../../GameClientMobile/libs

# boost
include $(CLEAR_VARS)
LOCAL_MODULE := boost_regex
LOCAL_MODULE_FILENAME := libboost_regex
LOCAL_SRC_FILES := $(BOOST_SRC_DIR)/lib/libboost_regex-gcc-mt-1_53.a
LOCAL_EXPORT_C_INCLUDES := $(BOOST_DIR)/include
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := boost_system
LOCAL_MODULE_FILENAME := libboost_system
LOCAL_SRC_FILES := $(BOOST_SRC_DIR)/lib/libboost_system-gcc-mt-1_53.a
LOCAL_EXPORT_C_INCLUDES := $(BOOST_DIR)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := boost_thread
LOCAL_MODULE_FILENAME := libboost_thread
LOCAL_SRC_FILES := $(BOOST_SRC_DIR)/lib/libboost_thread-gcc-mt-1_53.a
LOCAL_EXPORT_C_INCLUDES := $(BOOST_DIR)/include
include $(PREBUILT_STATIC_LIBRARY)

# end of boost

# flownet 
include $(CLEAR_VARS)

LOCAL_PCH := $(FLOWNET_DIR)/flownetPublicHeaders.gch

LOCAL_CPPFLAGS += -include $(LOCAL_PCH)
LOCAL_CPPFLAGS += -std=c++11

LOCAL_MODULE := flownet
LOCAL_MODULE_FILENAME := libflownet
LOCAL_SRC_FILES := \
$(FLOWNET_SRC_DIR)/BaseLock.cpp \
$(FLOWNET_SRC_DIR)/BasePacket.cpp \
$(FLOWNET_SRC_DIR)/ClientObject.cpp \
$(FLOWNET_SRC_DIR)/ClientConnection.cpp \
$(FLOWNET_SRC_DIR)/ClientTaskWorkerRoutine.cpp \
$(FLOWNET_SRC_DIR)/Deserializer.cpp \
$(FLOWNET_SRC_DIR)/FlownetObject.cpp \
$(FLOWNET_SRC_DIR)/LockFreeQueue.cpp \
$(FLOWNET_SRC_DIR)/NetworkObject.cpp \
$(FLOWNET_SRC_DIR)/NetworkObjectManager.cpp \
$(FLOWNET_SRC_DIR)/NetworkWorkerRoutine.cpp \
$(FLOWNET_SRC_DIR)/PacketHandler.cpp \
$(FLOWNET_SRC_DIR)/PacketParser.cpp \
$(FLOWNET_SRC_DIR)/PacketSizeLogger.cpp \
$(FLOWNET_SRC_DIR)/ScheduledTask.cpp \
$(FLOWNET_SRC_DIR)/ScheduledTaskWorkerRoutine.cpp \
$(FLOWNET_SRC_DIR)/ScopedLock.cpp \
$(FLOWNET_SRC_DIR)/Serializer.cpp \
$(FLOWNET_SRC_DIR)/ServerAcceptor.cpp \
$(FLOWNET_SRC_DIR)/SystemTimer.cpp \
$(FLOWNET_SRC_DIR)/TaskWorkerRoutine.cpp \
$(FLOWNET_SRC_DIR)/ThreadController.cpp \
$(FLOWNET_SRC_DIR)/WorkerRoutine.cpp \
$(FLOWNET_SRC_DIR)/WorkerThread.cpp \
$(FLOWNET_SRC_DIR)/flownetBoostUtility.cpp
LOCAL_EXPORT_C_INCLUDES := $(FLOWNET_DIR)

LOCAL_C_INCLUDES := $(BOOST_DIR)/include
LOCAL_C_INCLUDES += $(FLOWNET_DIR)

LOCAL_WHOLE_STATIC_LIBRARIES := libboost_thread

include $(BUILD_STATIC_LIBRARY)
# end of flownet

# gameshared

include $(CLEAR_VARS)
LOCAL_PCH := $(GAMESHARED_DIR)/GameSharedHeaders.pch

LOCAL_CPPFLAGS += -include $(LOCAL_PCH)
LOCAL_CPPFLAGS += -std=c++11

LOCAL_MODULE := gameshared
LOCAL_MODULE_FILENAME := libgameshared
LOCAL_SRC_FILES := \
$(GAMESHARED_SRC_DIR)/Actor.cpp \
$(GAMESHARED_SRC_DIR)/ActorData.cpp \
$(GAMESHARED_SRC_DIR)/EquipmentList.cpp \
$(GAMESHARED_SRC_DIR)/GameObject.cpp \
$(GAMESHARED_SRC_DIR)/GamePacket.cpp \
$(GAMESHARED_SRC_DIR)/GameShared.cpp \
$(GAMESHARED_SRC_DIR)/Inventory.cpp \
$(GAMESHARED_SRC_DIR)/Item.cpp \
$(GAMESHARED_SRC_DIR)/ItemAbilityData.cpp \
$(GAMESHARED_SRC_DIR)/ItemData.cpp \
$(GAMESHARED_SRC_DIR)/ItemDataDictionary.cpp \
$(GAMESHARED_SRC_DIR)/ItemFactory.cpp \
$(GAMESHARED_SRC_DIR)/Monster.cpp \
$(GAMESHARED_SRC_DIR)/MonsterData.cpp \
$(GAMESHARED_SRC_DIR)/MonsterDataDictionary.cpp \
$(GAMESHARED_SRC_DIR)/Player.cpp \
$(GAMESHARED_SRC_DIR)/ScheduledTick.cpp \
$(GAMESHARED_SRC_DIR)/ScheduledTickQueue.cpp \
$(GAMESHARED_SRC_DIR)/SpellDictionary.cpp \
$(GAMESHARED_SRC_DIR)/Stage.cpp \
$(GAMESHARED_SRC_DIR)/Stash.cpp

LOCAL_EXPORT_C_INCLUDES := $(GAMESHARED_DIR)

LOCAL_C_INCLUDES := $(BOOST_DIR)/include
LOCAL_C_INCLUDES += $(FLOWNET_DIR)
LOCAL_C_INCLUDES += $(GAMESHARED_DIR)

include $(BUILD_STATIC_LIBRARY)
# end of gameshared

# gameclient 

include $(CLEAR_VARS)

LOCAL_PCH := $(GAMECLIENT_DIR)/Classes/Headers.pch
LOCAL_PCH2 := $(GAMECLIENT_DIR)/GameClient/GameClientHeaders.pch

LOCAL_CPPFLAGS += -include $(LOCAL_PCH)
LOCAL_CPPFLAGS += -include $(LOCAL_PCH2)
LOCAL_CPPFLAGS += -std=c++11

LOCAL_MODULE := gameclient
LOCAL_MODULE_FILENAME := libgameclient
LOCAL_SRC_FILES := \
$(GAMECLIENT_SRC_DIR)/Classes/AccountScene.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/ActorLayer.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/ActorNode.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/AppDelegate.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/BackgroundImageLoader.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/BackgroundLayer.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/BaseLayer.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/BaseScene.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/BattleScene.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/CaptionLayer.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/ClientDirector.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/EffectLayer.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/HeartbeatLayer.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/InventoryNode.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/ItemImageLoader.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/ItemNode.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/ItemSlotNode.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/LoadingScene.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/MenuBarNode.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/MonsterNode.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/ObjectLayer.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/PlayerNode.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/PointConverter.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/SpellAnimationLoader.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/SpellImageLoader.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/SpellNode.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/SpellQuickSlotNode.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/SplashScene.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/StageCreateScene.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/StageScene.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/StashNode.cpp \
$(GAMECLIENT_SRC_DIR)/Classes/UILayer.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/ClientMonster.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/ClientPlayer.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/ClientStage.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/DelayChecker.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/GameClient.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/GameClientObject.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/GameClientPacketHandler.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/GameClientPacketParser.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/GameClientScheduledTasks.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/GameClientTasks.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/HeartbeatGenerator.cpp \
$(GAMECLIENT_SRC_DIR)/GameClient/RenderingTaskWorkerRoutine.cpp

LOCAL_EXPORT_C_INCLUDES := $(GAMECLIENT_DIR)
LOCAL_EXPORT_C_INCLUDES += $(GAMECLIENT_DIR)/Classes
LOCAL_EXPORT_C_INCLUDES += $(GAMECLIENT_DIR)/GameClient

LOCAL_C_INCLUDES := $(BOOST_DIR)/include
LOCAL_C_INCLUDES += $(FLOWNET_DIR)
LOCAL_C_INCLUDES += $(GAMESHARED_DIR)
LOCAL_C_INCLUDES += $(LIB_DIR)/spine-c/src
LOCAL_C_INCLUDES += $(LIB_DIR)/spine-c/include
LOCAL_C_INCLUDES += $(LIB_DIR)/spine-cocos2dx/src
LOCAL_C_INCLUDES += $(LIB_DIR)/cocos2dx
LOCAL_C_INCLUDES += $(LIB_DIR)/cocos2dx/include
LOCAL_C_INCLUDES += $(LIB_DIR)/cocos2dx/kazmath/include
LOCAL_C_INCLUDES += $(LIB_DIR)/cocos2dx/platform/android
LOCAL_C_INCLUDES += $(LIB_DIR)/CocosDenshion/include
LOCAL_C_INCLUDES += $(LIB_DIR)/extensions
LOCAL_C_INCLUDES += $(LIB_DIR)/extensions/CCBReader
LOCAL_C_INCLUDES += $(LIB_DIR)/extensions/GUI/CCControlExtension
LOCAL_C_INCLUDES += $(LIB_DIR)/extensions/GUI/CCScrollView
LOCAL_C_INCLUDES += $(LIB_DIR)/extensions/network
LOCAL_C_INCLUDES += $(LIB_DIR)/extensions/LocalStorage
LOCAL_C_INCLUDES += $(GAMECLIENT_DIR)
LOCAL_C_INCLUDES += $(GAMECLIENT_DIR)/Classes
LOCAL_C_INCLUDES += $(GAMECLIENT_DIR)/GameClient

include $(BUILD_STATIC_LIBRARY)

# end of gameclient

# game dynamic lib for android

include $(CLEAR_VARS)

LOCAL_PCH := $(LOCAL_PATH)/../../GameClientMobile/GameClient/GameClientHeaders.pch
LOCAL_PCH2 := $(LOCAL_PATH)/../../GameClientMobile/Classes/Headers.pch

LOCAL_CPPFLAGS += -include $(LOCAL_PCH)
LOCAL_CPPFLAGS += -include $(LOCAL_PCH2)
LOCAL_CPPFLAGS += -std=c++11
LOCAL_CPPFLAGS += -lc++

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp

LOCAL_C_INCLUDES := $(BOOST_DIR)/include
LOCAL_C_INCLUDES += $(FLOWNET_DIR)
LOCAL_C_INCLUDES += $(GAMESHARED_DIR)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../GameClientMobile
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../GameClientMobile/libs/spine-c/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../GameClientMobile/libs/spine-cocos2dx/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../GameClientMobile/Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../GameClientMobile/GameClient


LOCAL_WHOLE_STATIC_LIBRARIES := libboost_regex libboost_system libflownet libgameshared cocos2dx_static libspineC libspineCocos2d cocosdenshion_static cocos_extension_static libgameclient

include $(BUILD_SHARED_LIBRARY)

$(call import-module, spine-c) \
$(call import-module, spine-cocos2dx) \
$(call import-module, CocosDenshion/android) \
$(call import-module, cocos2dx) \
$(call import-module, extensions)
