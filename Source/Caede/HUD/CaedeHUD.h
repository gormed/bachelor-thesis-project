

#pragma once

#include "GameFramework/HUD.h"

#include "Character/EquipmentComponent.h"
#include "Character/CaedeCharacter.h"

#include "Dialogs/DialogState.h"
#include "Dialogs/Dialog.h"

#include "HUD/StausBar.h"

#include "CaedeHUD.generated.h"

//////////////////////////////////////////////////////////////////////////

#define HUD_SUBTITLE_TEXT_COLOR FColor(125,125,255,255)

#define HUD_SUBTITLE_NAME_COLOR FColor(75,75,255,255)

#define HUD_DIALOG_TOPICS_SELECTED_COLOR FColor(155,75,255,255)

#define HUD_DARK_COLOR FColor::Black

#define HUD_LIGHT_COLOR FColor::White

//////////////////////////////////////////////////////////////////////////

struct FSimpleStatusBar;

//////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FSelectedItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	FText ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	FText ItemStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	FText InventoryType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	FText InventoryStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	class AItem* Item;

	inline bool operator==(const FSelectedItem& a) const
	{
		if (a.Item == Item)
		{
			return true;
		}
		return false;
	}
};

//////////////////////////////////////////////////////////////////////////

class UInteractionBaseWidget;
class UQuickInventoryBaseWidget;

//////////////////////////////////////////////////////////////////////////

/**
 * 
 */
UCLASS()
class CAEDE_API ACaedeHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

public:

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "HUD|Widgets")
	UInteractionBaseWidget* InteractionWidget;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "HUD|Widgets")
	UQuickInventoryBaseWidget* QuickInventoryWidget;

	//////////////////////////////////////////////////////////////////////////

	/// main HUD update loop
	virtual void DrawHUD() override;

	//////////////////////////////////////////////////////////////////////////

	// Util

	/// Set the players mouse position
	UFUNCTION(BlueprintCallable, Category = "HUD|Utils", meta = (FriendlyName = "Set Mouse Position"))
	bool SetMousePosition(const APlayerController* playerController, const int32 x, const int32 y);

	/// Retrieve the current mouse position
	UFUNCTION(BlueprintCallable, Category = "HUD|Utils", meta = (FriendlyName = "Get Mouse Position"))
	bool GetMousePosition(const APlayerController* PlayerController, int32& x, int32& y);

	UFUNCTION(BlueprintCallable, Category = "HUD|Utils", meta = (FriendlyName = "Get Quadrant From Angle"))
	int32 GetQuadrantFromAngle(float angle, int32 Quadrants, float Offset);

	UFUNCTION(BlueprintCallable, Category = "HUD|Utils", meta = (FriendlyName = "Calculate HUD Aiming"))
	float CalculateHUDAiming(const APlayerController* playerController);

	UFUNCTION(BlueprintCallable, Category = "HUD|Utils", meta = (FriendlyName = "Project World To Screen"))
	bool ProjectWorldToScreen(FVector worldPosition, FVector2D& screenPosition);

	UFUNCTION(BlueprintCallable, Category = "HUD|Utils", meta = (FriendlyName = "Draw Icon To Canvas"))
	void DrawIconToCanvas(FCanvasIcon icon, FVector2D position, float scale, bool verticalCenter, bool horizontalCenter);

	UFUNCTION(BlueprintCallable, Category = "HUD|Utils", meta = (FriendlyName = "Draw Text To Canvas"))
	void DrawTextToCanvas(UFont* font, FText text, float scale, FVector2D position, FColor textColor, bool shadow, FLinearColor shadowColor, bool verticalCenter, bool horizontalCenter);
	
	UFUNCTION(BlueprintCallable, Category = "HUD|Utils", meta = (FriendlyName = "Draw Tile To Canvas"))
	void DrawTileToCanvas(FVector2D position, FVector2D pixelSize, float scale, const FLinearColor tileColor, bool verticalCenter, bool horizontalCenter);

	//////////////////////////////////////////////////////////////////////////

	// Toggling the different HUD Categories

	UFUNCTION(BlueprintCallable, Category = "HUD", meta = (FriendlyName = "Toggle HUD"))
	void ToggleHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD|Inventory", meta = (FriendlyName = "Set Character Attributes Visible"))
	void SetCharacterAttributesVisible(bool visible);

	UFUNCTION(BlueprintCallable, Category = "HUD|Subtitles", meta = (FriendlyName = "Set Subtitles Visible"))
	void SetSubtitlesVisible(bool visible);

	UFUNCTION(BlueprintCallable, Category = "HUD|Dialogs", meta = (FriendlyName = "Set Dialog Topics Visible"))
	void SetDialogTopicsVisible(bool visible);

	//////////////////////////////////////////////////////////////////////////
	// Observe the sources of informations to display
	//////////////////////////////////////////////////////////////////////////

	// Character

	UFUNCTION(BlueprintCallable, Category = "HUD|Character", meta = (FriendlyName = "Select Character Statuses"))
	void SelectCharacterStatuses();

	// Dialogs

	UFUNCTION(BlueprintCallable, Category = "HUD|Dialogs", meta = (FriendlyName = "Add Dialog Topic"))
	void AddDialogTopic(FDialogTopic Topic);

	UFUNCTION(BlueprintCallable, Category = "HUD|Dialogs", meta = (FriendlyName = "Select Marked Dialog Topic"))
	void SelectDialogTopic();

	UFUNCTION(BlueprintCallable, Category = "HUD|Dialogs", meta = (FriendlyName = "Flush Dialog Topics"))
	void FlushDialogTopics();

	//// Character Infos

	//UFUNCTION(BlueprintCallable, Category = "HUD|Hovered", meta = (FriendlyName = "Set Hovered NPC"))
	//void SetHoveredNPC(FVector WorldOffset, class ACaedeNPC* NPC, bool Visible);

	//// Hovered Item Infos

	//UFUNCTION(BlueprintCallable, Category = "HUD|Hovered", meta = (FriendlyName = "Set Hovered Items"))
	//void SetHoveredItem(FVector WorldOffset, class AItem* Item, bool Visible);

	//////////////////////////////////////////////////////////////////////////

	// Clears displayed content

	UFUNCTION(BlueprintCallable, Category = "HUD|Subtitles", meta = (FriendlyName = "Append Dialog Subtitle"))
	void AddDialogSubtitle(FDialogStateLine subtitle);

	UFUNCTION(BlueprintCallable, Category = "HUD|Subtitles", meta = (FriendlyName = "Flush Dialog Subtitles"))
	void FlushSubtitles();

	//////////////////////////////////////////////////////////////////////////

#pragma region HUD General

	//////////////////////////////////////////////////////////////////////////

	// HUD General

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|General")
	FColor HUDDark;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|General")
	FColor HUDLight;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|General")
	UFont* NormalFont;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "HUD|General")
	bool bDebugModeEnabled;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "HUD|General")
	bool bDebugDialogs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "HUD|General")
	bool bDebugInventory;

	/** UI scaling factor for other resolutions than Full HD */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "HUD|General")
	float ScaleUI;

#pragma endregion HUD General

#pragma region Subtitles

	//////////////////////////////////////////////////////////////////////////

	// Subtitles

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Subtitles")
	FColor SubtitleTextColor;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Subtitles")
	FColor SubtitleNameColor;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Subtitles")
	UFont* SubtitlesFont;

#pragma endregion Subtitles

#pragma region Dialogs

	//////////////////////////////////////////////////////////////////////////

	// Dialogs

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Dialog")
	FColor DialogsTopicsColor;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Dialog")
	UFont* DialogTopicsFont;

#pragma endregion Dialogs

#pragma region Statuses

	//////////////////////////////////////////////////////////////////////////

	// Statuses HUD Bar Colors

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses")
	FColor BackgroundBarColor;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses")
	FColor HealthBarColor;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses")
	FColor StaminaBarColor;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses")
	FColor StarvationBarColor;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses")
	FColor ThirstBarColor;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses")
	FColor ShitBarColor;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses")
	FColor UrinBarColor;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses")
	FColor DrugBarColor;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses")
	FColor AlcoholBarColor;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses")
	FColor PoisonBarColor;

	// Statuses Bars Dimension and Padding

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Align")
	FVector2D BarPadding;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Align")
	FVector2D PrimarySize;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Align")
	FVector2D SecondarySize;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Align")
	FVector2D TertiarySize;

	// Statuses HUD Font scaling

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Fonts")
	UFont* StatusesFont;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Fonts")
	FColor StatusesFontColor;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Fonts")
	FColor StatusesFontBorderColor;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Fonts")
	float PrimaryFontScale;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Fonts")
	float SecondaryFontScale;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Fonts")
	float TertiaryFontScale;

	//////////////////////////////////////////////////////////////////////////

	// Statuses Positions

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Position")
	FVector2D PrimaryAndSecondaryStatuses;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Statuses|Position")
	FVector2D TertiaryStatuses;

#pragma endregion Statuses

#pragma region NPC Infos

	//////////////////////////////////////////////////////////////////////////

	// NPC Info Colors

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "NPC Info")
	bool bDrawNPCInfo;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "NPC Info")
	FColor NPCInfoColor;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "NPC Info")
	float NPCInfoFontScale;

#pragma endregion NPC Infos

#pragma region Item Info

	//////////////////////////////////////////////////////////////////////////

	// Item Info 

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Info")
	bool bDrawItemInfo;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Info")
	FColor ItemInfoColor;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Info")
	float ItemInfoFontScale;

#pragma endregion Item Info

#pragma region Cursors

	//////////////////////////////////////////////////////////////////////////

	// Cursors

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Cursors")
	bool bDrawCenterCross;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cursors")
	FVector2D CenterCrossOffset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cursors")
	FVector2D CenterCrossUV;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cursors")
	FVector2D CenterCrossSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cursors")
	FVector2D RelativeCrossPosition;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cursors")
	float CenterCrossScale;

	FCanvasIcon CenterCrossIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Cursors")
	bool bDrawMouseCross;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cursors")
	FVector2D MouseCrosssOffset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cursors")
	FVector2D MouseCrossUV;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cursors")
	FVector2D MouseCrossSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cursors")
	float MouseCrossScale;

	FCanvasIcon MouseCrossIcon;

#pragma endregion Cursors

#pragma region Data References

	//////////////////////////////////////////////////////////////////////////

	// Hold data to display

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|Subtitles")
	TArray<FDialogStateLine> SubtitleStack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|Dialogs")
	TArray<FDialogTopic> DialogTopics;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|Inventory")
	TArray<FSelectedItem> DisplayedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|Character")
	ACaedeCharacter* OwningCharacter;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|Hovered")
	//ACaedeNPC* HoveredNPC;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|Hovered")
	//FVector NPCInfoWorldOffset;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|Hovered")
	//AItem* HoveredItem;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|Hovered")
	//FVector ItemInfoWorldOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|Inventory")
	TArray<UInventoryComponent*> DisplayedInventories;

#pragma endregion Data References

	//////////////////////////////////////////////////////////////////////////

	// Caede Game Infos

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Game Info")
	FString CaedeGameName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Game Info")
	FString CaedeGameVersion;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Game Info")
	FString CaedeGameState;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUD|Game Info")
	FString CaedeGameEngine;

	//////////////////////////////////////////////////////////////////////////

	// Enable/Disable HUD Debugging Mode
	void ToggleDebugMode();

	//////////////////////////////////////////////////////////////////////////

	FCanvasTileItem DrawTileToCanvas_Internal(FVector2D position, FVector2D pixelSize, const FLinearColor& color, float scale = 1.0f, bool verticalCenter = false, bool horizontalCenter = false);

	FCanvasTextItem DrawTextToCanvas_Internal(FVector2D position, float& scale, UFont* font, FText text, FColor& color, bool shadows = false, FLinearColor shadowColor = FLinearColor::Black, bool verticalCenter = false, bool horizontalCenter = false);

	void DrawIconToCanvas_Internal(FCanvasIcon icon, FVector2D position, float scale, bool verticalCenter = false, bool horizontalCenter = false);

protected:

	void DrawDebug();

	//////////////////////////////////////////////////////////////////////////

	// draws the different ui things

	void DrawSubtitles();

	void DrawDialogTopics();

	void DrawCinematicTiles();

	void DrawPickingCross();

	void DrawMouseCross();

	void DrawCharacterAttributes();

	void DrawItemWorldInfos();

	void DrawNPCWorldInfos();

	/// Draw game and engine infos
	void DrawGameInfos(float &CurrentX, float &CurrentY, float TextScale, const float Tab, float InitialX, const float LinePadding);

	//////////////////////////////////////////////////////////////////////////
	
	// Aiming

	float CalculateAmingDirection();

	int32 GetMarkedTopicFromAimingAngle(float angle);

	float CurrentAmingAngle;

	//////////////////////////////////////////////////////////////////////////

	// the current selected dialog topic index
	int32 MarkedTopic;

	//////////////////////////////////////////////////////////////////////////

	// visibility flags for the different ui

	bool bIsHUDVisible;

	bool bIsSubtitlesVisible;

	bool bIsDialogTopicsVisible;

	bool bIsCharacterAttributesVisible;

	bool bIsNPCInfoVisible;

	bool bIsItemInfoVisible;

	//////////////////////////////////////////////////////////////////////////

	// Aiming calculation

	float lastDelta;
	float totalAngle;

	//////////////////////////////////////////////////////////////////////////

	/** General offset for HUD elements */
	float Offset;

	/** FontRenderInfo enabling casting shadows */
	FFontRenderInfo ShadowedFont;

	UTexture2D* HUDTextureMain;

	FCanvasIcon SubtitlesBG;

	FCanvasIcon TopicsBG;

	//////////////////////////////////////////////////////////////////////////

	/** temporary helper for drawing text-in-a-box */
	void DrawDebugInfoString(const FString& Text, float PosX, float PosY, bool bAlignLeft, bool bAlignTop, const FColor& TextColor);
	
	virtual void BeginPlay() override;

};
