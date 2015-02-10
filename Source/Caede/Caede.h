

#pragma once

#include "Engine.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

//////////////////////////////////////////////////////////////////////////

DEFINE_LOG_CATEGORY_STATIC(LogCaede, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeInventory, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeSlot, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeEquipment, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeItem, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeHUD, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeVitality, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeInteraction, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeInput, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeCharacter, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeNPC, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeQuest, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeDialog, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeWidget, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogCaedeCamera, Log, All);

#define CAEDE_NAME "Caede"

#define CAEDE_VERSION "v0.4.2"

#define CAEDE_ENGINE_VERSION "UE 4.4.1"

#define CAEDE_STATE "Development"

//////////////////////////////////////////////////////////////////////////

// TODO: make component pointer private and add the getters!