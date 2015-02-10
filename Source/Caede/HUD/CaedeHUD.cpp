

#include "Caede.h"
#include "CaedeHUD.h"

#include "Character/CaedeCharacter.h"
#include "Character/Slots/WeaponSlot.h"
#include "Character/InventoryComponent.h"

#include "Items/Item.h"
#include "Items/WeaponItem.h"
#include "Items/QuestItem.h"
#include "Items/HerbItem.h"

#include "Dialogs/DialogPawn.h"
#include "Dialogs/Dialog.h"
#include "Dialogs/DialogComponent.h"

#define LOCTEXT_NAMESPACE "CAEDE HUD"

//////////////////////////////////////////////////////////////////////////

#define CIRCLE_OFFSET (-PI/2)

//////////////////////////////////////////////////////////////////////////

ACaedeHUD::ACaedeHUD(const class FObjectInitializer& PCIP)
: Super(PCIP)
{

	SubtitleStack = TArray<FDialogStateLine>();
	DialogTopics = TArray<FDialogTopic>();
	//static ConstructorHelpers::FObjectFinder<UTexture2D> LowHealthOverlayTextureOb(TEXT("Texture2D'/Game/UI/HUD/LowHealthOverlay.LowHealthOverlay'"));
	//static ConstructorHelpers::FObjectFinder<UTexture2D> InventoryMainTetxtureOb(TEXT("Texture2D'/Game/UI/HUD/InventoryMain.InventoryMain'"));

	// Fonts
	static ConstructorHelpers::FObjectFinder<UFont> SubtitlesFontOb(TEXT("Font'/Game/Textures/Fonts/Subtitles28.Subtitles28'"));
	static ConstructorHelpers::FObjectFinder<UFont> NormalFontOb(TEXT("Font'/Game/Textures/Fonts/HUDFont.HUDFont'"));
	// Textures
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDTextureOb(TEXT("Texture2D'/Game/Textures/HUD/HUDTexturesMain.HUDTexturesMain'"));

	//static ConstructorHelpers::FObjectFinder<UTexture2D> DialogTopicsOb(TEXT("Texture2D'/Game/Assets/HUD/DialogTopicsMain.DialogTopicsMain'"));

	HUDTextureMain = HUDTextureOb.Object;
	//DialogTopicsMainTetxture = DialogTopicsOb.Object;

	/*HitNotifyTexture = HitTextureOb.Object;
	HUDMainTexture = HUDMainTextureOb.Object;
	LowHealthOverlayTexture = LowHealthOverlayTextureOb.Object;
	InventoryMainTetxture = InventoryMainTetxtureOb.Object;*/

	SubtitlesFont = SubtitlesFontOb.Object;
	NormalFont = NormalFontOb.Object;
	DialogTopicsFont = SubtitlesFontOb.Object;
	StatusesFont = SubtitlesFontOb.Object;
	//InventoryFont = SubtitlesFontOb.Object;
	//WeaponsFont = SubtitlesFontOb.Object;
	//NPCInfoFont = SubtitlesFontOb.Object;
	//ItemInfoFont = SubtitlesFontOb.Object;
	//InventoryModeFont = SubtitlesFontOb.Object;

	SubtitlesBG = UCanvas::MakeIcon(HUDTextureMain, 0, 0, 1024, 220);
	TopicsBG = UCanvas::MakeIcon(HUDTextureMain, 0, 256, 256, 512);

	Offset = 20.0f;
	SubtitleTextColor = HUD_SUBTITLE_TEXT_COLOR;
	SubtitleNameColor = HUD_SUBTITLE_NAME_COLOR;
	DialogsTopicsColor = HUD_DIALOG_TOPICS_UNSELECTED_COLOR;
	StatusesFontColor = HUD_SUBTITLE_TEXT_COLOR;
	HUDDark = HUD_DARK_COLOR;
	HUDLight = HUD_LIGHT_COLOR;

	//InventoryModeScale = 1.0f;
	//InventoryScale = 1.0f;

	PrimaryFontScale = 1.0f;
	SecondaryFontScale = 0.8f;
	TertiaryFontScale = 0.75f;

	PrimaryAndSecondaryStatuses.X = 0.5f;
	PrimaryAndSecondaryStatuses.Y = 0.825f;

	TertiaryStatuses.X = 0.9f;
	TertiaryStatuses.Y = 0.8f;

	PrimarySize.X = 0.2f;
	PrimarySize.Y = 0.004f;

	SecondarySize.X = 0.095f;
	SecondarySize.Y = 0.004f;

	TertiarySize.X = 0.15f;
	TertiarySize.Y = 0.004f;

	BarPadding.X = 0.005f;
	BarPadding.Y = 0.01f;

	RelativeCrossPosition.X = 0.5f;
	RelativeCrossPosition.X = 0.5f;

	ShadowedFont.bEnableShadow = true;

	bIsHUDVisible = true;

	MarkedTopic = 0;

	CaedeGameName = CAEDE_NAME;
	CaedeGameVersion = CAEDE_VERSION;
	CaedeGameState = CAEDE_STATE;
	CaedeGameEngine = CAEDE_ENGINE_VERSION;

	bDebugModeEnabled = false;
}

//////////////////////////////////////////////////////////////////////////

void ACaedeHUD::DrawHUD()
{
	Super::DrawHUD();
	ScaleUI = Canvas->ClipY / 1080.0f;

	if (!bIsHUDVisible) return;

	ACaedeCharacter* MyPawn = Cast<ACaedeCharacter>(GetOwningPawn());
	//if (MyPawn && MyPawn->IsAlive() && MyPawn->Health < MyPawn->GetMaxHealth() * MyPawn->GetLowHealthPercentage())
	//{
	//	const float AnimSpeedModifier = 1.0f + 5.0f * (1.0f - MyPawn->Health/(MyPawn->GetMaxHealth() * MyPawn->GetLowHealthPercentage()));
	//	int32 EffectValue = 32 + 72 * (1.0f - MyPawn->Health/(MyPawn->GetMaxHealth() * MyPawn->GetLowHealthPercentage()));
	//	PulseValue += GetWorld()->GetDeltaSeconds() * AnimSpeedModifier;
	//	float EffectAlpha = FMath::Abs(FMath::Sin(PulseValue));

	//	float AlphaValue = ( 1.0f / 255.0f ) * ( EffectAlpha * EffectValue );

	//	FCanvasTileItem TileItem( FVector2D( 0, 0 ), LowHealthOverlayTexture->Resource, FVector2D( Canvas->ClipX, Canvas->ClipY ), FLinearColor( 1.0f, 0.0f, 0.0f, AlphaValue ) );
	//	TileItem.BlendMode = SE_BLEND_Translucent;
	//	Canvas->DrawItem( TileItem );
	//}

	// net mode
	if (GetWorld()->GetNetMode() != NM_Standalone)
	{
		FString NetModeDesc = (GetWorld()->GetNetMode() == NM_Client) ? TEXT("Client") : TEXT("Server");
		DrawDebugInfoString(NetModeDesc, Canvas->OrgX + Offset*ScaleUI, Canvas->OrgY + 5 * Offset*ScaleUI, true, true, FColor::Magenta);
	}

	float InitialX, InitialY;
	float CurrentX, CurrentY;

	FString TestDesc = CAEDE_NAME;
	TestDesc.Append(CAEDE_VERSION);
	FVector2D DescSize(0.0f, 0.0f);
	Canvas->StrLen(SubtitlesFont, TestDesc, DescSize.X, DescSize.Y);

	float TextScale = 1.f;
	const float topPadding = 30.0f * ScaleUI * TextScale;
	const float rightPadding = 50.0f * ScaleUI * TextScale;
	const float Tab = 20.0f * ScaleUI * TextScale;
	const float LinePadding = 10.0f * ScaleUI * TextScale;
	InitialX = Canvas->ClipX - (rightPadding + DescSize.X) * TextScale;
	InitialY = (topPadding) * TextScale;
	CurrentX = InitialX;
	CurrentY = InitialY;

	DrawGameInfos(CurrentX, CurrentY, TextScale, Tab, InitialX, LinePadding);

	if (MyPawn)
	{
		DrawPickingCross();
		DrawMouseCross();
		DrawSubtitles();
		DrawDialogTopics();
		DrawCharacterAttributes();
		//DrawItemWorldInfos();
		//DrawNPCWorldInfos();

		if (bDebugModeEnabled)
		{
			DrawDebug();
		}
	}

	//DrawMatchTimerAndPosition();

	//if (MatchState == EMatchState::Playing)
	//{
}

//////////////////////////////////////////////////////////////////////////

void ACaedeHUD::BeginPlay()
{
	CenterCrossIcon = UCanvas::MakeIcon(
		HUDTextureMain, CenterCrossUV.X, CenterCrossUV.Y,
		CenterCrossSize.X, CenterCrossSize.Y);
	MouseCrossIcon = UCanvas::MakeIcon(
		HUDTextureMain, MouseCrossUV.X, MouseCrossUV.Y,
		MouseCrossSize.X, MouseCrossSize.Y);
}

//////////////////////////////////////////////////////////////////////////

bool ACaedeHUD::SetMousePosition(const APlayerController* playerController, const int32 x, const int32 y)
{
	if (!playerController) return false;

	// Get Player
	const ULocalPlayer* localPlayer =
		Cast<ULocalPlayer>(playerController->Player);

	// PlayerController::Player is UPlayer
	if (!localPlayer) return false;

	// get view port ptr
	const UGameViewportClient* playerViewportClient =
		Cast<UGameViewportClient>(localPlayer->ViewportClient);

	if (!playerViewportClient) return false;

	FViewport* playerViewport = playerViewportClient->Viewport;

	if (!playerViewport) return false;

	playerViewport->SetMouse(x, y);

	return true;
}

bool ACaedeHUD::GetMousePosition(const APlayerController* playerController, int32& x, int32& y)
{
	if (!playerController) return false;

	// Get Player
	const ULocalPlayer* localPlayer =
		Cast<ULocalPlayer>(playerController->Player);

	// PlayerController::Player is UPlayer
	if (!localPlayer) return false;

	// get view port ptr
	const UGameViewportClient* playerViewportClient =
		Cast<UGameViewportClient>(localPlayer->ViewportClient);

	if (!playerViewportClient) return false;

	FViewport* playerViewport = playerViewportClient->Viewport;

	if (!playerViewport) return false;

	x = playerViewport->GetMouseX();
	y = playerViewport->GetMouseY();

	return true;
}

//////////////////////////////////////////////////////////////////////////

void ACaedeHUD::DrawDebug()
{

	ACaedeCharacter* characterPawn = Cast<ACaedeCharacter>(GetOwningPawn());
	const APlayerController* playerController = Cast<APlayerController>(OwningCharacter->GetController());

	if (!playerController) return;

	// Get Player
	const ULocalPlayer* localPlayer =
		Cast<ULocalPlayer>(playerController->Player);

	// PlayerController::Player is UPlayer
	if (!localPlayer) return;

	// get view port ptr
	const UGameViewportClient* playerViewportClient =
		Cast<UGameViewportClient>(localPlayer->ViewportClient);

	if (!playerViewportClient) return;

	FViewport* playerViewport = playerViewportClient->Viewport;

	if (!playerViewport) return;

	int32 x = playerViewport->GetMouseX();
	int32 y = playerViewport->GetMouseY();

	int32 midX = Canvas->ClipX / 2;
	int32 midY = Canvas->ClipY / 2;

	//////////////////////////////////////////////////////////////////////////

	// Relative mouse
	float relX = 0;// characterPawn->Fighting->GetAttackMove().Position.X;
	float relY = 0;// characterPawn->Fighting->GetAttackMove().Position.Y;

	// Mouse-Line
	FCanvasLineItem line(FVector(midX, midY, 0), FVector(x, y, 0));
	line.LineThickness = 1.5f;
	Canvas->DrawItem(line);

	FCanvasLineItem sectorLine;
	float endX, endY;

	// Dialog Topic Sectors
	if (bDebugDialogs)
	{
		float sectors = (float)DialogTopics.Num();
		for (int32 i = 0; i < DialogTopics.Num(); i++)
		{
			endX = midX + (1000.0f * FMath::Cos((-PI / (sectors)) + i * (2.0f*PI / sectors)));
			endY = midY + (1000.0f * FMath::Sin((-PI / (sectors)) + i * (2.0f*PI / sectors)));
			sectorLine = FCanvasLineItem(FVector(midX, midY, 0), FVector(endX, endY, 0));
			sectorLine.LineThickness = 2.0f;
			Canvas->DrawItem(sectorLine);
		}
	}

	if (bDebugInventory)
	{
		//int32 Number = DisplayedInventories.Num();
		//float sectors = (float)Number;
		//for (int32 i = 0; i < Number; i++)
		//{
		//	endX = midX + (1000.0f * FMath::Cos((-PI / (sectors)) + i * (2.0f*PI / sectors)));
		//	endY = midY + (1000.0f * FMath::Sin((-PI / (sectors)) + i * (2.0f*PI / sectors)));
		//	sectorLine = FCanvasLineItem(FVector(midX, midY, 0), FVector(endX, endY, 0));
		//	sectorLine.LineThickness = 2.0f;
		//	Canvas->DrawItem(sectorLine);
		//}
	}

	//// Fighting Sectors
	//if (!characterPawn->HasDialogPartner())
	//{
	//	float sectors = (float)6;
	//	for (int32 i = 0; i < sectors; i++)
	//	{
	//		endX = midX + (1000.0f * FMath::Cos(CIRCLE_OFFSET + i * (2.0f*pi / sectors)));
	//		endY = midY + (1000.0f * FMath::Sin(CIRCLE_OFFSET + i * (2.0f*pi / sectors)));
	//		sectorLine = FCanvasLineItem(FVector(midX, midY, 0), FVector(endX, endY, 0));
	//		Canvas->DrawItem(sectorLine);
	//	}
	//}

}

//////////////////////////////////////////////////////////////////////////

void ACaedeHUD::DrawDebugInfoString(const FString& Text, float PosX, float PosY, bool bAlignLeft, bool bAlignTop, const FColor& TextColor)
{
	float SizeX, SizeY;
	Canvas->StrLen(NormalFont, Text, SizeX, SizeY);

	const float UsePosX = bAlignLeft ? PosX : PosX - SizeX;
	const float UsePosY = bAlignTop ? PosY : PosY - SizeY;

	const float BoxPadding = 5.0f;

	FColor DrawColor(HUDDark.R, HUDDark.G, HUDDark.B, HUDDark.A * 0.2f);
	const float X = UsePosX - BoxPadding * ScaleUI;
	const float Y = UsePosY - BoxPadding * ScaleUI;
	FCanvasTileItem TileItem(FVector2D(X, Y), FVector2D((SizeX + BoxPadding * 2.0f) * ScaleUI, (SizeY + BoxPadding * 2.0f) * ScaleUI), DrawColor);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);

	FCanvasTextItem TextItem(FVector2D(UsePosX, UsePosY), FText::FromString(Text), NormalFont, TextColor);
	TextItem.FontRenderInfo = ShadowedFont;
	TextItem.Scale = FVector2D(ScaleUI, ScaleUI);
	TextItem.EnableShadow(FLinearColor::Black);
	Canvas->DrawItem(TextItem);
}

//////////////////////////////////////////////////////////////////////////

void ACaedeHUD::SelectCharacterStatuses()
{
	OwningCharacter = Cast<ACaedeCharacter>(GetOwningPawn());
	if (OwningCharacter == NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("The owning character is invalid!"));
		return;
	}
}

//////////////////////////////////////////////////////////////////////////

int32 ACaedeHUD::GetQuadrantFromAngle(float angle, int32 Quadrants, float Offset)
{
	float pi = PI;
	float sectors = (float)Quadrants;

	if (angle < (pi / (sectors)) + Offset)
	{
		//////////////////////////////////////////////////////////////////////////
		// DEBUG
		//
		if (bDebugModeEnabled)
		{
			FString dQ = FString("Quadrant ");
			dQ.AppendInt(Quadrants-1);
			DrawDebugInfoString(dQ, 20, 220, true, true, SubtitleTextColor);
		}
		//////////////////////////////////////////////////////////////////////////
		return Quadrants - 1;
	}

	for (int32 i = 0; i < Quadrants; i++)
	{
		if (angle >= ((pi / (sectors)) + Offset + (i*(pi * 2) / sectors))
			&& angle < ((pi / (sectors)) + Offset + ((i + 1)*(pi * 2) / sectors)))
		{
			//////////////////////////////////////////////////////////////////////////
			// DEBUG
			//
			if (bDebugModeEnabled)
			{
				FString dQ = FString("Quadrant ");
				dQ.AppendInt(i);
				DrawDebugInfoString(dQ, 20, 220, true, true, SubtitleTextColor);
			}
			//////////////////////////////////////////////////////////////////////////
			return i;
		}
	}
	return -1;
}

float ACaedeHUD::CalculateHUDAiming(const APlayerController* playerController)
{
	if (playerController == NULL)
	{
		UE_LOG(LogCaedeHUD, Error, TEXT("PlayerController is NULL! Check the usage of the function!"));
		return -1;
	}
	if (Canvas == NULL)
	{
		UE_LOG(LogCaedeHUD, Error, TEXT("Canvas is NULL! Check the usage of the function!"));
		return -1;
	}

	// Get Player
	const ULocalPlayer* localPlayer =
		Cast<ULocalPlayer>(playerController->Player);

	// PlayerController::Player is UPlayer
	if (!localPlayer) return false;

	// get view port ptr
	const UGameViewportClient* playerViewportClient =
		Cast<UGameViewportClient>(localPlayer->ViewportClient);

	if (!playerViewportClient) return false;

	FViewport* playerViewport = playerViewportClient->Viewport;

	if (!playerViewport) return false;

	int32 x = playerViewport->GetMouseX();
	int32 y = playerViewport->GetMouseY();

	int32 midX = Canvas->ClipX / 2;
	int32 midY = Canvas->ClipY / 2;

	float angleInRad = FMath::Atan2(float(y - midY), float(x - midX)) + PI;

	//////////////////////////////////////////////////////////////////////////
	// DEBUG
	if (bDebugModeEnabled)
	{
		FString dAiming = FString("AimAngle ");
		dAiming += FString::SanitizeFloat(angleInRad);
		//dAiming.AppendInt(((int32) FMath::RadiansToDegrees(angleInRad) ) );
		DrawDebugInfoString(dAiming, 20, 60, true, true, SubtitleTextColor);
	}


	//FString dX = FString("X ");
	//dX.AppendInt(x);
	//DrawDebugInfoString(dX, 20, 100, true, true, SubtitleTextColor);

	//FString dY = FString("Y ");
	//dY.AppendInt(y);
	//DrawDebugInfoString(dY, 20, 140, true, true, SubtitleTextColor);

	//FCanvasLineItem line(FVector(midX, midY, 0), FVector(x,y,0));
	//Canvas->DrawItem(line);
	//////////////////////////////////////////////////////////////////////////

	return angleInRad;
}

bool ACaedeHUD::ProjectWorldToScreen(FVector worldPosition, FVector2D& screenPosition)
{
	if (!PlayerOwner) return false;

	// Get Player
	ULocalPlayer* localPlayer =
		Cast<ULocalPlayer>(PlayerOwner->Player);

	// PlayerController::Player is UPlayer
	if (!localPlayer) return false;

	// get view port ptr
	const UGameViewportClient* playerViewportClient =
		Cast<UGameViewportClient>(localPlayer->ViewportClient);

	if (!playerViewportClient) return false;

	FViewport* playerViewport = playerViewportClient->Viewport;

	if (!playerViewport) return false;

	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
		playerViewport,
		GetWorld()->Scene,
		playerViewportClient->EngineShowFlags)
		.SetRealtimeUpdate(true));

	FVector ViewLocation;
	FRotator ViewRotation;
	FSceneView* SceneView = localPlayer->CalcSceneView(&ViewFamily, /*out*/ ViewLocation, /*out*/ ViewRotation, playerViewport);

	if (SceneView)
	{
		FVector2D MyResult;
		SceneView->WorldToPixel(worldPosition, screenPosition);
		return true;
	}
	return false;
}

float ACaedeHUD::CalculateAmingDirection()
{
	const APlayerController* playerController = Cast<APlayerController>(GetOwningPawn()->GetController());

	return CalculateHUDAiming(playerController);

}

int32 ACaedeHUD::GetMarkedTopicFromAimingAngle(float angle)
{
	int32 Quadrants = DialogTopics.Num();
	return GetQuadrantFromAngle(angle, Quadrants, 0);

}

//////////////////////////////////////////////////////////////////////////

void ACaedeHUD::AddDialogSubtitle(FDialogStateLine subtitle)
{
	ACaedeCharacter* Char = Cast<ACaedeCharacter>(this->GetOwningPawn());
	if (Char != NULL && Char->HasDialogPartner())
	{
		SubtitleStack.Add(subtitle);
		UE_LOG(LogCaedeHUD, Display, TEXT("Appended DialogSubtitle for %s!"), *Char->CurrentPartner->GetName());
		return;
	}
	UE_LOG(LogCaedeHUD, Error, TEXT("Failed to add subtitle to HUD! The Main Character Pawn might be talking to someone other or is invalid..."), *GetName());
}

void ACaedeHUD::FlushSubtitles()
{
	SubtitleStack.Empty();
	UE_LOG(LogCaedeHUD, Display, TEXT("Flushed Subtitles!"));
}

//////////////////////////////////////////////////////////////////////////

void ACaedeHUD::AddDialogTopic(FDialogTopic Topic)
{
	// This happens if there is nothing given in the Blueprint call to this function
	if (Topic.Color.A < 1) Topic.Color = HUD_DIALOG_TOPICS_UNSELECTED_COLOR;

	ACaedeCharacter* Char = Cast<ACaedeCharacter>(this->GetOwningPawn());
	if (Char != NULL && Char->HasDialogPartner())
	{
		if (Topic.Dialog != NULL && Topic.Dialog->IsDialogActive)
		{
			MarkedTopic = DialogTopics.Add(Topic);
			UE_LOG(LogCaedeHUD, Display, TEXT("Appended DialogTopic for %s!"), *Char->GetName());
			return;
		}
		else
		{
			UE_LOG(LogCaedeHUD, Log, TEXT("Dialog topic %s is not active!"), *Topic.Topic);
			return;
		}
	}
	UE_LOG(LogCaedeHUD, Error, TEXT("Failed to add DialogTopic to HUD! The Main Character Pawn might be talking to someone other or is invalid..."));
}

void ACaedeHUD::SelectDialogTopic()
{
	ACaedeCharacter* Char = Cast<ACaedeCharacter>(this->GetOwningPawn());
	if (Char != NULL && Char->HasDialogPartner() && DialogTopics.Num() > 0)
	{
		UE_LOG(LogCaedeHUD, Display, TEXT("Selected Dialog Topic [%d - %s]!"), MarkedTopic, *DialogTopics[MarkedTopic].Topic);
		MarkedTopic = FMath::Abs(MarkedTopic % DialogTopics.Num());
		FlushSubtitles();
		Char->CurrentPartner->DialogComponent->SetDialogTopic(DialogTopics[MarkedTopic].Dialog);
		Char->CurrentPartner->DialogComponent->StartDialogTopic();
		FlushDialogTopics();
	}
}

void ACaedeHUD::FlushDialogTopics()
{
	DialogTopics.Empty();
	UE_LOG(LogCaedeHUD, Display, TEXT("Flushed Dialog Topics!"));
}

//////////////////////////////////////////////////////////////////////////

//void ACaedeHUD::SetHoveredNPC(FVector WorldOffset, class ACaedeNPC* NPC, bool Visible)
//{
//	NPCInfoWorldOffset = WorldOffset;
//	HoveredNPC = NPC;
//	bIsNPCInfoVisible = Visible;
//}
//
//void ACaedeHUD::SetHoveredItem(FVector WorldOffset, class AItem* Item, bool Visible)
//{
//	ItemInfoWorldOffset = WorldOffset;
//	HoveredItem = Item;
//	bIsItemInfoVisible = Visible;
//}

//////////////////////////////////////////////////////////////////////////
// Draw Utils

void ACaedeHUD::DrawIconToCanvas_Internal(FCanvasIcon icon, FVector2D position, float scale, bool verticalCenter, bool horizontalCenter)
{
	DrawIconToCanvas(icon, position, scale, verticalCenter, horizontalCenter);
}

void ACaedeHUD::DrawIconToCanvas(FCanvasIcon icon, FVector2D position, float scale, bool verticalCenter, bool horizontalCenter)
{
	FVector2D size = FVector2D(icon.UL, icon.VL) * scale * ScaleUI;
	if (verticalCenter)
	{
		position.Y -= size.Y * 0.5f;
	}
	if (horizontalCenter)
	{
		position.X -= size.X * 0.5f;
	}
	Canvas->DrawIcon(icon, position.X, position.Y, scale * ScaleUI);
}

FCanvasTextItem ACaedeHUD::DrawTextToCanvas_Internal(FVector2D position, float& scale, UFont* font, FText text, FColor& color, bool shadow, FLinearColor shadowColor, bool verticalCenter, bool horizontalCenter)
{
	float width, height;
	Canvas->TextSize(font, text.ToString(), width, height, ScaleUI * scale);
	if (verticalCenter)
	{
		position.Y -= height * 0.5f;
	}
	if (horizontalCenter)
	{
		position.X -= width * 0.5f;
	}

	FCanvasTextItem TextItem(position, text, font, color);
	TextItem.Scale = FVector2D(ScaleUI * scale, ScaleUI * scale);
	if (shadow)
	{
		TextItem.FontRenderInfo = ShadowedFont;
		TextItem.EnableShadow(shadowColor);
	}
	Canvas->DrawItem(TextItem);
	return TextItem;
}

void ACaedeHUD::DrawTextToCanvas(UFont* font, FText text, float scale, FVector2D position, FColor textColor, bool shadow, FLinearColor shadowColor, bool verticalCenter, bool horizontalCenter)
{
	DrawTextToCanvas_Internal(position, scale, font, text, textColor, shadow, shadowColor, verticalCenter, horizontalCenter);
}

FCanvasTileItem ACaedeHUD::DrawTileToCanvas_Internal(FVector2D position, FVector2D pixelSize, const FLinearColor& color, float scale, bool verticalCenter, bool horizontalCenter)
{
	FVector2D size = pixelSize * scale * ScaleUI;
	if (verticalCenter)
	{
		position.Y -= size.Y * 0.5f;
	}
	if (horizontalCenter)
	{
		position.X -= size.X * 0.5f;
	}
	FCanvasTileItem Tile(position, pixelSize * scale * ScaleUI, color);
	Canvas->DrawItem(Tile);
	return Tile;
}

void ACaedeHUD::DrawTileToCanvas(FVector2D position, FVector2D pixelSize, float scale, const FLinearColor tileColor, bool verticalCenter, bool horizontalCenter)
{
	DrawTileToCanvas_Internal(position, pixelSize, tileColor, scale, verticalCenter, horizontalCenter);
}

//////////////////////////////////////////////////////////////////////////
// Draw Methods
//////////////////////////////////////////////////////////////////////////

void ACaedeHUD::DrawPickingCross()
{
	if (!bDrawCenterCross) return;


	float PosX = (Canvas->ClipX * RelativeCrossPosition.X);
	float PosY = (Canvas->ClipY * RelativeCrossPosition.Y);
	float scale = ScaleUI * CenterCrossScale;

	Canvas->DrawIcon(
		CenterCrossIcon,
		PosX - CenterCrossOffset.X * scale,
		PosY - CenterCrossOffset.Y * scale, scale);
	
	//DrawTileToCanvas(FVector2D(PosX - 2, PosY - 2), FVector2D(4, 4), FColor::Yellow);
}

void ACaedeHUD::DrawMouseCross()
{
	if (!bDrawMouseCross) return;

	int32 x, y;
	float scale = ScaleUI * MouseCrossScale;
	GetMousePosition(PlayerOwner, x, y);

	Canvas->DrawIcon(
		MouseCrossIcon,
		x - MouseCrosssOffset.X * scale,
		y - MouseCrosssOffset.Y * scale, scale);

	//DrawTileToCanvas(FVector2D(x, y), FVector2D(5, 5), FColor::Cyan);
}

void ACaedeHUD::DrawCharacterAttributes()
{
	if (bIsCharacterAttributesVisible 
		&& OwningCharacter != NULL 
		&& OwningCharacter->Vitality != NULL)
	{

		if (PlayerOwner == NULL)
		{
			return;
		}

		UVitalityComponent* vitality = OwningCharacter->Vitality;

		float primaryX = Canvas->ClipX * PrimaryAndSecondaryStatuses.X;
		float primaryY = Canvas->ClipY * PrimaryAndSecondaryStatuses.Y;

		float primaryWidth = PrimarySize.X;
		float primaryHeight = PrimarySize.Y;

		float secondaryWidth = SecondarySize.X;
		float secondaryHeight = SecondarySize.Y;

		float tertiaryWidth = TertiarySize.X;
		float tertiaryHeight = TertiarySize.Y;

		float barPaddingX = Canvas->ClipX * 0.005f;
		float barPaddingY = Canvas->ClipY * 0.01f;

		float currentX = primaryX;
		float currentY = primaryY;
		float secondaryX;
		float secondaryY;

		//////////////////////////////////////////////////////////////////////////

		// Primary Statuses

		FSimpleStatusBar healthBar = FSimpleStatusBar(FVector2D(currentX, currentY), FText::FromString("Health"), primaryHeight, primaryWidth);
		healthBar.Update(Canvas, vitality->Health / vitality->MaxHealth);
		healthBar.Draw(Canvas, StatusesFont, StatusesFontColor, HealthBarColor, BackgroundBarColor, 1.0f, PrimaryFontScale);

		currentY += (healthBar.GetSize(Canvas).Y) + barPaddingY;

		FSimpleStatusBar staminaBar = FSimpleStatusBar(FVector2D(currentX, currentY), FText::FromString("Stamina"), primaryHeight, primaryWidth);
		staminaBar.Update(Canvas, vitality->Stamina / vitality->MaxStamina);
		staminaBar.Draw(Canvas, StatusesFont, StatusesFontColor, StaminaBarColor, BackgroundBarColor, 1.0f, PrimaryFontScale);

		currentY += (staminaBar.GetSize(Canvas).Y) + barPaddingY;

		//////////////////////////////////////////////////////////////////////////

		// Secondary I Statuses

		secondaryX = currentX = primaryX - (staminaBar.GetSize(Canvas).X) * 0.5f;
		secondaryY = currentY;

		FSimpleStatusBar starvationBar = FSimpleStatusBar(FVector2D(currentX, currentY), FText::FromString("Starvation"), secondaryHeight, secondaryWidth);
		starvationBar.Update(Canvas, vitality->Starvation / vitality->MaxStarvation);
		starvationBar.Draw(Canvas, StatusesFont, StatusesFontColor, StarvationBarColor, BackgroundBarColor, 1.0f, SecondaryFontScale, false);

		currentY += (starvationBar.GetSize(Canvas).Y) + barPaddingY;

		FSimpleStatusBar thirstBar = FSimpleStatusBar(FVector2D(currentX, currentY), FText::FromString("Thirst"), secondaryHeight, secondaryWidth);
		thirstBar.Update(Canvas, vitality->Thirst / vitality->MaxThirst);
		thirstBar.Draw(Canvas, StatusesFont, StatusesFontColor, ThirstBarColor, BackgroundBarColor, 1.0f, SecondaryFontScale, false);

		currentY += (thirstBar.GetSize(Canvas).Y) + barPaddingY;

		//////////////////////////////////////////////////////////////////////////

		// Secondary II Statuses
		
		currentX = primaryX + barPaddingX;
		currentY = secondaryY;

		FSimpleStatusBar shitBar = FSimpleStatusBar(FVector2D(currentX, currentY), FText::FromString("Shit"), secondaryHeight, secondaryWidth);
		shitBar.Update(Canvas, vitality->Shit / vitality->MaxShit);
		shitBar.Draw(Canvas, StatusesFont, StatusesFontColor, ShitBarColor, BackgroundBarColor, 1.0f, SecondaryFontScale, false);

		currentY += (shitBar.GetSize(Canvas).Y) + barPaddingY;

		FSimpleStatusBar urinBar = FSimpleStatusBar(FVector2D(currentX, currentY), FText::FromString("Urin"), secondaryHeight, secondaryWidth);
		urinBar.Update(Canvas, vitality->Urin / vitality->MaxUrin);
		urinBar.Draw(Canvas, StatusesFont, StatusesFontColor, UrinBarColor, BackgroundBarColor, 1.0f, SecondaryFontScale, false);

		//////////////////////////////////////////////////////////////////////////

		// Tertiary Statuses

		float tertiaryX = Canvas->ClipX * TertiaryStatuses.X;
		float tertiaryY = Canvas->ClipY * TertiaryStatuses.Y;

		currentX = tertiaryX;
		currentY = tertiaryY;

		FSimpleStatusBar alcoholBar = FSimpleStatusBar(FVector2D(currentX, currentY), FText::FromString("Alcohol"), tertiaryHeight, tertiaryWidth);
		alcoholBar.Update(Canvas, vitality->Alcohol / vitality->MaxAlcohol);
		alcoholBar.Draw(Canvas, StatusesFont, StatusesFontColor, AlcoholBarColor, BackgroundBarColor, 1.0f, TertiaryFontScale);
		currentY += (alcoholBar.GetSize(Canvas).Y) + barPaddingY;

		FSimpleStatusBar drugBar = FSimpleStatusBar(FVector2D(currentX, currentY), FText::FromString("Drugs"), tertiaryHeight, tertiaryWidth);
		drugBar.Update(Canvas, vitality->Drug / vitality->MaxDrug);
		drugBar.Draw(Canvas, StatusesFont, StatusesFontColor, DrugBarColor, BackgroundBarColor, 1.0f, TertiaryFontScale);

		currentY += (drugBar.GetSize(Canvas).Y) + barPaddingY;

		FSimpleStatusBar poisonBar = FSimpleStatusBar(FVector2D(currentX, currentY), FText::FromString("Poison"), tertiaryHeight, tertiaryWidth);
		poisonBar.Update(Canvas, vitality->Poison / vitality->MaxPoison);
		poisonBar.Draw(Canvas, StatusesFont, StatusesFontColor, PoisonBarColor, BackgroundBarColor, 1.0f, TertiaryFontScale);

	}
}

void ACaedeHUD::DrawSubtitles()
{
	if (bIsSubtitlesVisible && SubtitleStack.Num() > 0)
	{
		if (PlayerOwner == NULL)
		{
			return;
		}

		DrawCinematicTiles();

		float TextScale = 0.8f;

		float SubtitlesPosX = (Canvas->ClipX * 0.1);
		float SubtitlesPosY = (Canvas->ClipY * 0.865);
		//Canvas->DrawIcon(InventoryBg, SubtitlesPosX, SubtitlesPosY,	ScaleUI)

		const FString KilledDesc = TEXT("Test");
		FVector2D DescSize(0.0f, 0.0f);

		const float GameTime = GetWorld()->GetTimeSeconds();
		const float LinePadding = 6.0f * ScaleUI * TextScale;
		const float SpeakerSpace = 40.0f * ScaleUI * TextScale;
		const float InitialX = SubtitlesPosX;
		const float InitialY = SubtitlesPosY;

		// draw messages
		float CurrentY = InitialY;
		Canvas->StrLen(SubtitlesFont, KilledDesc, DescSize.X, DescSize.Y);

		float SubtitlesBGPosY = SubtitlesPosY - (SubtitlesBG.VL) * ScaleUI;

		//Canvas->DrawIcon(SubtitlesBG, 0, SubtitlesBGPosY, (Canvas->ClipX/SubtitlesBG.UL));//Canvas->ClipY/(SubtitlesBG.VL-Offset));

		FDialogStateLine Subtitle;

		for (int32 i = 0; i < SubtitleStack.Num(); i++)
		{
			Subtitle = SubtitleStack[i];

			float CurrentX = InitialX;
			FVector2D SubtitleSize(0.0f, 0.0f);
			if (Subtitle.DisplayName)
			{
				FCanvasTextItem TextItem(FVector2D(CurrentX, CurrentY), FText::FromString(Subtitle.Name), SubtitlesFont, SubtitleNameColor);
				TextItem.FontRenderInfo = ShadowedFont;
				TextItem.Scale = FVector2D(ScaleUI * TextScale, ScaleUI * TextScale);
				TextItem.EnableShadow(FLinearColor::Black);
				Canvas->DrawItem(TextItem);

				CurrentX += (TextItem.DrawnSize.X + SpeakerSpace) * TextScale;

			}
			FCanvasTextItem TextItem(FVector2D(CurrentX, CurrentY), FText::FromString(Subtitle.Subtitle), SubtitlesFont, SubtitleTextColor);
			TextItem.FontRenderInfo = ShadowedFont;
			TextItem.Scale = FVector2D(ScaleUI * TextScale, ScaleUI * TextScale);
			TextItem.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(TextItem);

			CurrentX += TextItem.DrawnSize.X * TextScale;

			CurrentY += (DescSize.Y + LinePadding) * TextScale * ScaleUI;
		}
	}
}

void ACaedeHUD::DrawCinematicTiles()
{
	FCanvasTileItem TopTile(FVector2D(0, 0), FVector2D(Canvas->ClipX, Canvas->ClipY * 0.15f), FColor::Black);
	Canvas->DrawItem(TopTile);

	FCanvasTileItem BottomTile(FVector2D(0, Canvas->ClipY - (Canvas->ClipY * 0.15f)), FVector2D(Canvas->ClipX, Canvas->ClipY * 0.15f), FColor::Black);
	Canvas->DrawItem(BottomTile);
}

void ACaedeHUD::DrawDialogTopics()
{
	if (bIsDialogTopicsVisible && DialogTopics.Num() > 0)
	{
		if (PlayerOwner == NULL)
		{
			return;
		}

		DrawCinematicTiles();

		float Radius = 40;

		float DialogTopicsPosX = Canvas->ClipX * 0.5f;
		float DialogTopicsPosY = Canvas->ClipY * 0.915f;
		//Canvas->DrawIcon(InventoryBg, DialogTopicsPosX, DialogTopicsPosY,	ScaleUI)

		const FString KilledDesc = TEXT("Test");
		FVector2D DescSize(0.0f, 0.0f);

		const float GameTime = GetWorld()->GetTimeSeconds();
		const float LinePadding = 6.0f;
		const float BoxPadding = 10.0f;
		const float MaxLineX = 300.0f;
		const float InitialX = DialogTopicsPosX;//DialogTopicsPosX + (InventoryBg.UL - Offset * 3.0f) * ScaleUI;
		const float InitialY = DialogTopicsPosY;//DialogTopicsPosY + (InventoryBg.VL - Offset * 2.5f) * ScaleUI;

		// draw messages
		float CurrentY = InitialY;
		float CurrentX = InitialX;

		Canvas->StrLen(DialogTopicsFont, KilledDesc, DescSize.X, DescSize.Y);

		float TopicsBGScale = 0.75f;
		float TextScale = 0.75f;

		//////////////////////////////////////////////////////////////////////////

		CurrentAmingAngle = CalculateAmingDirection();

		MarkedTopic = GetMarkedTopicFromAimingAngle(CurrentAmingAngle);

		//////////////////////////////////////////////////////////////////////////

		int32 Number = DialogTopics.Num();
		FDialogTopic DialogTopic;

		for (int32 i = 0; i < Number; i++)
		{
			DialogTopic = DialogTopics[i];

			FVector2D SubtitleSize(0.0f, 0.0f);
			FVector2D TopicSize(0.0f, 0.0f);

			Canvas->StrLen(DialogTopicsFont, DialogTopic.Topic, TopicSize.X, TopicSize.Y);

			CurrentX = InitialX + (3.0f * Radius * ScaleUI * FMath::Cos(CIRCLE_OFFSET + i*(PI * 2) / Number)) - TopicSize.X * 0.5f * ScaleUI;
			CurrentY = InitialY + (Radius * ScaleUI * FMath::Sin(CIRCLE_OFFSET + i*(PI * 2) / Number)) - TopicSize.Y * 0.5f * ScaleUI;

			FColor TopicColor;
			if (i == MarkedTopic)
			{
				TopicColor = HUD_DIALOG_TOPICS_SELECTED_COLOR;
			}
			else
			{
				TopicColor = DialogTopic.Color;
			}

			FCanvasTextItem TextItem(FVector2D(CurrentX, CurrentY),
				FText::FromString(DialogTopic.Topic), DialogTopicsFont, TopicColor);
			TextItem.FontRenderInfo = ShadowedFont;
			TextItem.Scale = FVector2D(ScaleUI * TextScale, ScaleUI * TextScale);
			TextItem.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(TextItem);

		}
	}
}

//void ACaedeHUD::DrawNPCWorldInfos()
//{
//	if (bDrawNPCInfo && HoveredNPC != NULL && bIsNPCInfoVisible)
//	{
//		FVector2D ScreenPosition;
//		ProjectWorldToScreen(HoveredNPC->GetActorLocation() + NPCInfoWorldOffset, ScreenPosition);
//
//		DrawTextToCanvas_Internal(ScreenPosition, ScaleUI, NPCInfoFont, HoveredNPC->LocalizedNPCName, NPCInfoColor, true, FLinearColor::Black, true, true);
//	}
//}
//
//void ACaedeHUD::DrawItemWorldInfos()
//{
//	if (bDrawItemInfo && HoveredItem != NULL && bIsItemInfoVisible)
//	{
//		float padding = 5;
//		float emptyScale = 0.5f * ScaleUI;
//
//		FVector2D ScreenPosition;
//
//		ProjectWorldToScreen(HoveredItem->InfoAnchor->GetComponentLocation() + ItemInfoWorldOffset, ScreenPosition);
//
//		FCanvasTextItem itemName = DrawTextToCanvas_Internal(
//			ScreenPosition, ScaleUI, ItemInfoFont, 
//			HoveredItem->LocalizedItemName, ItemInfoColor, 
//			true, FLinearColor::Black, true, true);
//
//		if (HoveredItem->IsEmpty())
//		{
//			DrawTextToCanvas_Internal(
//				FVector2D(ScreenPosition.X, ScreenPosition.Y + padding + itemName.DrawnSize.Y),
//				emptyScale, ItemInfoFont, FText::FromString("Empty"), ItemInfoColor, true, FLinearColor::Black, true, true);
//		}
//	}
//}

void ACaedeHUD::DrawGameInfos(float &CurrentX, float &CurrentY, float TextScale, const float Tab, float InitialX, const float LinePadding)
{
	FCanvasTextItem TextItem = DrawTextToCanvas_Internal(
		FVector2D(CurrentX, CurrentY), TextScale, SubtitlesFont, FText::FromString(CaedeGameName), SubtitleTextColor, true);

	CurrentX += (TextItem.DrawnSize.X + Tab) * TextScale;

	TextItem = DrawTextToCanvas_Internal(
		FVector2D(CurrentX, CurrentY), TextScale, SubtitlesFont, FText::FromString(CaedeGameVersion), SubtitleTextColor, true);

	CurrentX = InitialX;
	CurrentY += (TextItem.DrawnSize.Y + LinePadding) * TextScale;

	TextItem = DrawTextToCanvas_Internal(
		FVector2D(CurrentX, CurrentY), TextScale, SubtitlesFont, FText::FromString(CaedeGameState), SubtitleTextColor, true);

	CurrentY += (TextItem.DrawnSize.Y + LinePadding) * TextScale;

	TextItem = DrawTextToCanvas_Internal(
		FVector2D(CurrentX, CurrentY), TextScale, SubtitlesFont, FText::FromString(CaedeGameEngine), SubtitleTextColor, true);
}

//////////////////////////////////////////////////////////////////////////

void ACaedeHUD::ToggleHUD()
{
	bIsHUDVisible = !bIsHUDVisible;
}

void ACaedeHUD::SetSubtitlesVisible(bool visible)
{
	bIsSubtitlesVisible = visible;
}

void ACaedeHUD::SetDialogTopicsVisible(bool visible)
{
	bIsDialogTopicsVisible = visible;
}

void ACaedeHUD::ToggleDebugMode()
{
	bDebugModeEnabled = !bDebugModeEnabled;
}

void ACaedeHUD::SetCharacterAttributesVisible(bool visible)
{
	bIsCharacterAttributesVisible = visible;
}

#undef LOCTEXT_NAMESPACE

